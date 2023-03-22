#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <map>
#include <sys/types.h>


std::map<std::string, int> parseText(std::string name) 
{
	std::ifstream t(name + ".txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string text = buffer.str();

	std::regex word_regex("(\\w+)");
	auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_regex);
	auto words_end = std::sregex_iterator();

	std::map<std::string, int> WORDS;
	std::ofstream mapFile(name + "_map.txt");

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		std::smatch match = *i;
		std::string match_str = match.str();
		std::transform(match_str.begin(), match_str.end(), match_str.begin(), ::tolower);

		WORDS[match_str]++;
	}

	for (auto pair : WORDS) {
		mapFile << pair.first << ":" << pair.second << std::endl;
	}
	mapFile.close();

	return WORDS;
}

std::map<std::string, int> parseMap(std::string name)
{
	std::string word;
	std::string count;
	std::ifstream mapFile(name + "_map.txt");
	std::map<std::string, int> WORDS;
	int i = 1;

	while (i == 1) {
		if (getline(mapFile, word, ':')) {
			getline(mapFile, count);

			WORDS[word] = std::stoi(count);
		}
		else {
			i = 0;
		}
	}

	return WORDS;
}

int main()
{
	std::string name = "big";
	struct stat buffer;
	std::map<std::string, int> WORDS;

	if (stat((name + "_map.txt").c_str(), &buffer) == -1) {
		// Create the file
		WORDS = parseText(name);
	} else {
		WORDS = parseMap(name);
	}
}