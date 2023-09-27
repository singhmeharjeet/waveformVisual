# Project Name

This a C++ project to visualize the waveform of an audio file.

## Table of Contents

-   [Installation](#installation)
-   [Technologies Used](#technologies-used)
-   [License](#license)

## Installation

Follow these steps to install and set up the project:

### 1. Clone or Download

You can clone the project using the following command:

```bash
git clone https://github.com/singhmeharjeet/waveformVisual.git
```

Alternatively, you can download the ZIP and extract it to your desired location.

### 2. Navigate to the Build Folder

Navigate to the project's build folder using the terminal:

```bash
cd build
```

### 3. Build the Project

In the terminal, run the following commands to build the project:

#### 3.1. Configure the Project

Use `cmake` to configure the project:

```bash
cmake ..
```

#### 3.2. Build the Project

Compile the project using `make`:

```bash
make
```

#### 3.3. Run the Project

```bash
./src/waveform
```

## Technologies Used

1. SDL3 - For windowing and rendering the GUI.
2. ImGui - For creating the GUI
3. AudioFile - For parsing the .wav format filesystem.
4. TinyFileDialogs - For opening the file whose waveform is to be visualized.
