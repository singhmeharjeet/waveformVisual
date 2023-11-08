#include <SDL3/SDL.h>
#include <stdio.h>

#include "AudioFile.h"
#include "huffman.cpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "myfont.cpp"
#include "read_file.h"
#include "tinyfiledialogs.cpp"

#define WIDTH  1200
#define HEIGHT 800

// Main code
int main(int, char**) {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	// Enable native IME.
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	SDL_Window* window = SDL_CreateWindow("Waveform Visualizer", WIDTH, HEIGHT, window_flags);
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

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(MyFont_compressed_data, MyFont_compressed_size, 18.0f);
	IM_ASSERT(font != nullptr);
	// Our state
	static bool show_upload_button = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	bool done = false;

	char const* filepath;
	AudioFile<float> waveFile;
	float entropy;
	float avg_code_length;

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

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
		ImGui::Begin("Waveform", &show_upload_button, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		if (filepath == nullptr) {
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "Select a file to continue");
			if (ImGui::Button("Choose file")) {
				show_upload_button = false;
				filepath = tinyfd_openFileDialog("Select a .wav file to display", "", 1, (const char*[]){"*.wav"}, "WAV files", 0);
				waveFile = read_file(filepath);
				entropy = get_entropy(waveFile);
				avg_code_length = get_average_code_length(waveFile);
			}
		} else {
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "Selected File");
			ImGui::Text("%s", filepath);
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "File Info");
			ImGui::Text("Bit Depth: %d", waveFile.getBitDepth());
			ImGui::Text("Sample Rate: %d", waveFile.getSampleRate());
			ImGui::Text("Num Channels: %d", waveFile.getNumChannels());
			ImGui::Text("Length in Seconds: %f", waveFile.getLengthInSeconds());
			ImGui::Text("Entropy: %f", entropy);
			ImGui::Text("Avg Word Length: %f", avg_code_length);

			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 255.0f, 1.0f), "Waveform");

			static float graphHeight = ImGui::GetContentRegionAvail().y / 2.5f;
			static float graphWidth = ImGui::GetContentRegionAvail().x;
			static float offset = graphHeight / 2.0f;
			static float dx = graphWidth / waveFile.getNumSamplesPerChannel();

			ImVec2 p0 = ImGui::GetCursorScreenPos();

			ImGui::BeginChild("graph1", ImVec2(graphWidth, graphHeight), true, 0);
			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Left Channel").x);
			ImGui::Text("Left Channel");

			// Display waveform
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			for (int n = 0; n + 1 < waveFile.getNumSamplesPerChannel(); n++) {
				float t0 = n * dx;
				float t1 = t0 + dx;

				float sample0 = waveFile.samples[0][n];
				float sample1 = waveFile.samples[0][n + 1];

				ImVec2 p1(p0.x + t0, p0.y + (sample0 * graphHeight) + offset);
				ImVec2 p2(p0.x + t1, p0.y + (sample1 * graphHeight) + offset);

				draw_list->AddLine(p1, p2, IM_COL32(200, 0, 200, 200), 1.0f);
			}
			ImGui::EndChild();

			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			p0 = ImGui::GetCursorScreenPos();

			ImGui::BeginChild("graph2", ImVec2(graphWidth, graphHeight), true, 0);
			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Right Channel").x);
			ImGui::Text("Right Channel");

			for (int n = 0; n + 1 < waveFile.getNumSamplesPerChannel(); n++) {
				float t0 = n * dx;
				float t1 = t0 + dx;

				float sample0 = waveFile.samples[1][n];
				float sample1 = waveFile.samples[1][n + 1];

				ImVec2 p1(p0.x + t0, p0.y + (sample0 * graphHeight) + offset);
				ImVec2 p2(p0.x + t1, p0.y + (sample1 * graphHeight) + offset);

				draw_list->AddLine(p1, p2, IM_COL32(200, 0, 200, 200), 1.0f);
			}
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
