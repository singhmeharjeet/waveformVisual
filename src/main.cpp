#include <SDL.h>

#include <string>

#include "AudioFile.h"
#include "read_file.h"
#include "tinyfiledialogs.cpp"

void read_file(const std::string&);
int main() {
	// Init a system
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "\nSDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

	int width = 800;
	int height = 600;
	auto windowFlags = SDL_WINDOW_RESIZABLE;
	auto rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	auto window = SDL_CreateWindow("Waveform Visualizer", width, height, windowFlags);
	auto renderer = SDL_CreateRenderer(window, NULL, rendererFlags);

	auto modal = tinyfd_openFileDialog("Select a .wav file to display", "", 1, (const char*[]){"*.wav"}, "WAV files", 0);
	read_file(modal);

	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				SDL_DestroyWindow(window);
				SDL_QuitSubSystem(SDL_INIT_VIDEO);
				SDL_Quit();
				return 0;
			}
		}
	}

	return 0;
}
