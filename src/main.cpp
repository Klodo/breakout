#include <SDL2/SDL.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

/*************/
/*** types ***/
/*************/

struct Color {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0;
};

struct Brick {
    SDL_Rect rect;
    int health = 0;

    Brick() = default;
    Brick(int x, int y, int w, int h, int start_health)
        : rect({x, y, w, h}), health(start_health) {}
};

struct Paddle {
    enum class State { NEUTRAL, MOVING_LEFT, MOVING_RIGHT };

    SDL_Rect rect;
    State state = State::NEUTRAL;
    int speed;

    void update() {
        switch (state) {
        case State::NEUTRAL: {
            break;
        }
        case State::MOVING_LEFT: {
            rect.x -= speed;
            if (rect.x < 0) {
                rect.x = 0;
            }
            break;
        }
        case State::MOVING_RIGHT: {
            rect.x += speed;
            if (rect.x > 640 - rect.w) {
                rect.x = 640 - rect.w;
            }
            break;
        }
        }
    }

    Paddle() = default;
    Paddle(int x, int y, int w, int h, int speed)
        : rect({x, y, w, h}), speed(speed) {}
};

/*****************************/
/*** function declarations ***/
/*****************************/
bool init_sdl();
void clean_sdl();
void init_game();
void handle_events();
void handle_keydown(const SDL_KeyboardEvent& event);
void handle_keyup(const SDL_KeyboardEvent& event);
void handle_mouse_button_down(const SDL_MouseButtonEvent& event);
void render();
void render_rect(const SDL_Rect& rect, Color color);

/*****************/
/*** constants ***/
/*****************/
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

constexpr int NUM_BRICK_COLUMNS = 13;
constexpr int NUM_BRICK_ROWS = 10;

constexpr Color BRICK_COLORS[NUM_BRICK_ROWS] = {
    {200, 200, 200, 255},
    {180, 180, 180, 255},
    {160, 160, 160, 255},
    {140, 140, 140, 255},
    {120, 120, 120, 255},
    {100, 100, 100, 255},
    {80, 80, 80, 255},
    {60, 60, 60, 255},
    {40, 40, 40, 255},
    {20, 20, 20, 255},
};

constexpr int BRICK_WIDTH = 40;
constexpr int BRICK_HEIGHT = 20;

constexpr Color PADDLE_COLOR = {100, 0, 40, 255};
constexpr int PADDLE_WIDTH = 100;
constexpr int PADDLE_HEIGHT = 20;
constexpr int PADDLE_SPEED = 5;

/***************/
/*** globals ***/
/***************/
bool g_is_running = true;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
std::vector<Brick> g_bricks;
Paddle g_paddle;

/****************************/
/*** function definitions ***/
/****************************/
int main(int argc, char** argv) {
    if (init_sdl() == false) {
        return EXIT_FAILURE;
    }

    init_game();

    while (g_is_running) {
        handle_events();
        g_paddle.update();
        render();
    }

    clean_sdl();
}

bool init_sdl() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init(SDL_INIT_EVERYTHING) != 0\n";
        return false;
    }

    g_window = SDL_CreateWindow("Breakout",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH,
                                WINDOW_HEIGHT,
                                SDL_WINDOW_SHOWN);

    if (g_window == nullptr) {
        std::cout << "g_window == nullptr\n";
        return false;
    }

    g_renderer = SDL_CreateRenderer(
        g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (g_renderer == nullptr) {
        std::cout << "g_renderer == nullptr\n";
        return false;
    }

    return true;
}

void clean_sdl() {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

void init_game() {
    // init bricks
    int offset_x = (WINDOW_WIDTH - (NUM_BRICK_COLUMNS * (BRICK_WIDTH + 1))) / 2;
    int offset_y = 20;

    for (int row = 0; row < NUM_BRICK_ROWS; ++row) {
        for (int col = 0; col < NUM_BRICK_COLUMNS; ++col) {
            g_bricks.emplace_back(offset_x + col * (BRICK_WIDTH + 1),
                                  offset_y + row * (BRICK_HEIGHT + 1),
                                  BRICK_WIDTH,
                                  BRICK_HEIGHT,
                                  NUM_BRICK_ROWS - row);
        }
    }

    // init paddle
    g_paddle = Paddle((WINDOW_WIDTH / 2) - (PADDLE_WIDTH / 2),
                      WINDOW_HEIGHT - 30 - PADDLE_HEIGHT,
                      PADDLE_WIDTH,
                      PADDLE_HEIGHT,
                      PADDLE_SPEED);
}

void handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN: {
            handle_keydown(event.key);
            break;
        }
        case SDL_KEYUP: {
            handle_keyup(event.key);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            handle_mouse_button_down(event.button);
            break;
        }
        case SDL_QUIT: {
            g_is_running = false;
            break;
        }
        default: { break; }
        }
    }
}

void handle_keydown(const SDL_KeyboardEvent& event) {
    switch (event.keysym.scancode) {
    case SDL_SCANCODE_LEFT: {
        g_paddle.state = Paddle::State::MOVING_LEFT;
        break;
    }
    case SDL_SCANCODE_RIGHT: {
        g_paddle.state = Paddle::State::MOVING_RIGHT;
        break;
    }
    case SDL_SCANCODE_ESCAPE: {
        g_is_running = false;
        break;
    }
    default: { break; }
    }
}

void handle_keyup(const SDL_KeyboardEvent& event) {
    switch (event.keysym.scancode) {
    case SDL_SCANCODE_LEFT: {
        if (g_paddle.state == Paddle::State::MOVING_LEFT) {
            g_paddle.state = Paddle::State::NEUTRAL;
        }
        break;
    }
    case SDL_SCANCODE_RIGHT: {
        if (g_paddle.state == Paddle::State::MOVING_RIGHT) {
            g_paddle.state = Paddle::State::NEUTRAL;
        }
        break;
    }
    default: { break; }
    }
}

void handle_mouse_button_down(const SDL_MouseButtonEvent& event) {
    std::cout << '(' << event.x << ", " << event.y << ")\n";
}

void render() {
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(g_renderer);

    // render bricks
    for (const auto& brick : g_bricks) {
        render_rect(brick.rect, BRICK_COLORS[brick.health - 1]);
    }

    // render paddle
    render_rect(g_paddle.rect, PADDLE_COLOR);

    SDL_RenderPresent(g_renderer);
}

void render_rect(const SDL_Rect& rect, Color color) {
    SDL_SetRenderDrawColor(g_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(g_renderer, &rect);
}
