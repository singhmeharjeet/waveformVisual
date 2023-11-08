## How to make the executable file? ###

#### Step 0: Prerequisites

1) Your system should have `git` installed.
2) Your system should have `cmake` installed.
3) Your system should have `make` installed.

#### Step 1: Git clone

```bash
git clone "https://github.com/singhmeharjeet/waveformVisual" . --recurse-submodules -j8 
```

- This command will clone the project into current directory and will also clone the required submodules.
- The `-j8` flag is optional and is used to speed up the cloning process.
- This command will download `SDL`, `SDL_image`, `imgui` and `tinyfiledialogs` submodules, so it will take some time. It
  is around 500mb.

#### Step 2: Build the project

```bash
cd build
cmake ..
make
```

- The first time running these command will take a minute or two, but after that it will be fast.

#### Step 3: Run the executable (inside the build folder)
```bash
./src/image
``` 

Now, the executable will be found in the `./build/src/` folder by the name of `image`

## Technologies Used

1. SDL3 - For windowing and rendering the GUI.
2. ImGui - For creating the GUI
3. AudioFile - For parsing the .wav format filesystem.
4. TinyFileDialogs - For opening the file whose waveform is to be visualized.
