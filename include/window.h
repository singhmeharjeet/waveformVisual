#pragma once

#include <SDL.h>

#include <memory>

class Window {
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;

	public:
	Window(const char* title, int width, int height, SDL_WindowFlags flags)
		: m_window(SDL_CreateWindow(title, width, height, flags), SDL_DestroyWindow) {
		if (!m_window) {
			throw std::runtime_error(SDL_GetError());
		}
	}

	static void init() {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "\nSDL_Init Error: %s\n", SDL_GetError());
			return;
		}
	}
	static void quit() {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_Quit();
	}
};