#include <SDL3/SDL.h>
#include <stdio.h>

#include "AudioFile.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "read_file.h"
#include "tinyfiledialogs.cpp"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

// Main code
int main(int, char**) {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0) {
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	// Enable native IME.
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	SDL_Window* window = SDL_CreateWindow("Waveform Visualizer", 800, 600, window_flags);
	if (window == nullptr) {
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	// ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	auto font = io.Fonts->AddFontFromFileTTF("../assests/Roboto-Medium.ttf", 18.0f);
	IM_ASSERT(font != nullptr);

	// Our state
	static bool show_upload_button = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	bool done = false;

	AudioFile<float> waveFile;
	// waveFile = read_file("/Users/mehar/Desktop/Education/Sem 12/365/Project1try4/assets/test-audio.wav");

	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("", &show_upload_button, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		if (waveFile.getFileName().empty()) {
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "Select a file to continue");
			if (ImGui::Button("Choose file")) {
				show_upload_button = false;
				auto filepath = tinyfd_openFileDialog("Select a .wav file to display", "", 1, (const char*[]){"*.wav"}, "WAV files", 0);
				waveFile = read_file(filepath);
			}
		} else {
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "Selected File");
			ImGui::Text("%s", waveFile.getFileName().c_str());
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "File Info");
			ImGui::Text("Bit Depth: %d", waveFile.getBitDepth());
			ImGui::Text("Sample Rate: %d", waveFile.getSampleRate());
			ImGui::Text("Num Channels: %d", waveFile.getNumChannels());
			ImGui::Text("Length in Seconds: %f", waveFile.getLengthInSeconds());

			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "Waveform");
			static float audioLengthInSeconds = waveFile.getLengthInSeconds();
			static float maxAmplitudeHeight = 300.0f;
			static float targetWidth = 795.0f;
			static int initialWaveformWidth = 795;
			static float scaleAmplitude = maxAmplitudeHeight / 1.8f;  // Divide by 2 to ensure it fits within maxAmplitudeHeight
			static float waveformHeight = maxAmplitudeHeight;		  // Set the height to the maximum amplitude height
			static ImVec2 p0 = ImGui::GetCursorScreenPos();

			// Begin the scrollable area with an explicitly defined content size
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
			ImGui::BeginChild("scrolling", ImVec2(780, waveformHeight), true, ImGuiWindowFlags_HorizontalScrollbar);

			// Display waveform
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			static float dx = (targetWidth / static_cast<float>(waveFile.getNumSamplesPerChannel())) * 2;
			static float halfMaxAmplitudeHeight = maxAmplitudeHeight / 2.0f;

			for (int n = 0; n < waveFile.getNumSamplesPerChannel(); n++) {
				float t0 = n * dx;
				float t1 = (n + 1) * dx;

				float sample0 = waveFile.samples[0][n];
				float sample1 = waveFile.samples[0][n + 1];

				// Scale the amplitude to fit within maxAmplitudeHeight
				float scaledSample0 = sample0 * scaleAmplitude * 1.5;
				float scaledSample1 = sample1 * scaleAmplitude * 1.5;

				ImVec2 p1(p0.x + t0, p0.y + halfMaxAmplitudeHeight - scaledSample0);
				ImVec2 p2(p0.x + t1, p0.y + halfMaxAmplitudeHeight - scaledSample1);

				draw_list->AddLine(p1, p2, IM_COL32(200, 0, 200, 200), 1.0f);
			}

			ImGui::PopStyleVar();  // ImGuiStyleVar_FrameRounding
			ImGui::PopStyleVar();  // ImGuiStyleVar_FramePadding
			ImGui::EndChild();
		}
		ImGui::End();

		// Rendering
		ImGui::Render();
		// SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}

	// Cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
