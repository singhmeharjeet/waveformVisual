#include <AudioFile.h>

#include <map>
#include <queue>
#include <string>

using std::string;

struct HuffmanNode {
	float data;
	int freq;
	HuffmanNode *left, *right;
	HuffmanNode(float data, int freq) {
		this->data = data;
		this->freq = freq;
		left = right = nullptr;
	}
};

struct Compare {
	bool operator()(HuffmanNode *l, HuffmanNode *r) {
		return (l->freq > r->freq);
	}
};

std::map<float, int> set_frequency_map(const AudioFile<float> &wavefile) {
	std::map<float, int> freq;
	for (auto channel = 0; channel < wavefile.getNumChannels(); channel++) {
		for (auto sample = 0; sample < wavefile.getNumSamplesPerChannel(); sample++) {
			float value = wavefile.samples[channel][sample];
			if (freq.find(value) == freq.end()) {
				freq[value] = 1;
			} else {
				freq[value]++;
			}
		}
	}
	return freq;
}
void populate_codes(HuffmanNode *root, const string& str, std::map<float, string> &code) {
	if (!root) {
		return;
	}

	if (root->data != -1) {
		code[root->data] = str;
	}

	populate_codes(root->left, str + "0", code);
	populate_codes(root->right, str + "1", code);
}

std::map<float, string> set_code_map(const std::map<float, int> &freq, const AudioFile<float> &wavefile) {
	// Initialize priority queue for huffman tree
	std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, Compare> pq;

	for (auto it : freq) {
		pq.push(new HuffmanNode(it.first, it.second));
	}

	// Make huffman tree
	while (pq.size() != 1) {
		HuffmanNode *left = pq.top();
		pq.pop();
		HuffmanNode *right = pq.top();
		pq.pop();

		auto *top = new HuffmanNode(-1, left->freq + right->freq);
		top->left = left;
		top->right = right;
		pq.push(top);
	}

	// Traverse the huffman tree and store codes in code
	std::map<float, string> code;
	populate_codes(pq.top(), "", code);
	return code;
}

// Public Functions
float get_entropy(const AudioFile<float> &wavefile) {
	std::map<float, int> freq = set_frequency_map(wavefile);

	float entropy = 0;
	for (auto & it : freq) {
		float prob = (float)it.second / wavefile.getNumSamplesPerChannel();
		entropy += prob * log2(prob);
	}
	return -entropy;
}

float get_average_code_length(const AudioFile<float> &wavefile) {
	float avg = 0;
	std::map<float, int> freq = set_frequency_map(wavefile);
	std::map<float, string> code = set_code_map(freq, wavefile);

	assert(code.size() == freq.size());

	for (auto &it: code) {
		const auto size = it.second.size();
		const auto probability = (float) freq.at(it.first) / (float)wavefile.getNumSamplesPerChannel();
		avg += size * probability;
	}

	return avg;

}
