#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>
#include <thread>

/*****************************/
/*** function declarations ***/
/*****************************/
void handle_events();
void handle_keydown(const SDL_KeyboardEvent& event);
void render();

/*****************/
/*** constants ***/
/*****************/
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

/***************/
/*** globals ***/
/***************/
bool g_is_running = true;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;

/****************************/
/*** function definitions ***/
/****************************/
int main(int argc, char** argv) {
    std::cout << "breakout\n";

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init(SDL_INIT_EVERYTHING) != 0\n";
        return EXIT_FAILURE;
    }

    g_window = SDL_CreateWindow("Breakout",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH,
                                WINDOW_HEIGHT,
                                SDL_WINDOW_SHOWN);

    if (g_window == nullptr) {
        std::cout << "g_window == nullptr\n";
        return EXIT_FAILURE;
    }

    g_renderer = SDL_CreateRenderer(
        g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (g_renderer == nullptr) {
        std::cout << "g_renderer == nullptr\n";
        return EXIT_FAILURE;
    }

    while (g_is_running) {
        handle_events();
    }

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

void handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN: {
            handle_keydown(event.key);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            // handle_mouse_button_down(event.button);
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
    switch (event.keysym.sym) {
    case SDLK_ESCAPE: {
        g_is_running = false;
        break;
    }
    default: { break; }
    }
}

void render() {

}
