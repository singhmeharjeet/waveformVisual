#include <iostream>

#include "AudioFile.h"

AudioFile<float> read_file(const std::string& inputFilePath) {
	//---------------------------------------------------------------
	// 2. Create an AudioFile object and load the audio file

	AudioFile<float> a;
	bool loadedOK = a.load(inputFilePath);

	/** If you hit this assert then the file path above
	 probably doesn't refer to a valid audio file */
	assert(loadedOK);

	//---------------------------------------------------------------
	// 3. Let's apply a gain to every audio sample

	float gain = 0.5f;

	for (int i = 0; i < a.getNumSamplesPerChannel(); i++) {
		for (int channel = 0; channel < a.getNumChannels(); channel++) {
			a.samples[channel][i] = a.samples[channel][i] * gain;
		}
	}

	std::cout << "Bit Depth: " << a.getBitDepth() << std::endl;
	std::cout << "Sample Rate: " << a.getSampleRate() << std::endl;
	std::cout << "Num Channels: " << a.getNumChannels() << std::endl;
	std::cout << "Length in Seconds: " << a.getLengthInSeconds() << std::endl;
	std::cout << std::endl;

	//---------------------------------------------------------------
	// 4. Write audio file to disk

	std::string outputFilePath = "quieter-audio-file.wav";	// change this to somewhere useful for you
	a.save(outputFilePath, AudioFileFormat::Aiff);

	return a;
}