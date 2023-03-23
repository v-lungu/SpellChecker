#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <map>
#include <sys/types.h>
#include <numeric>
#include <set>

void parseText(std::string const& name, std::map<std::string, int>& WORDS)
{
	std::ifstream t(name + ".txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string text = buffer.str();

	std::regex word_regex("(\\w+)");
	auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_regex);
	auto words_end = std::sregex_iterator();

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
}

void parseMap(std::string const& name, std::map<std::string, int>& WORDS)
{
	std::string word;
	std::string count;
	std::ifstream mapFile(name + "_map.txt");
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
}

float P(std::string const& word, std::map<std::string, int>& WORDS) 
{
	const float N = std::accumulate(std::begin(WORDS), std::end(WORDS), 0,
		[](const int previous, const std::pair<const std::string, int> p)
		{ return previous + p.second; });

	return WORDS[word] / N;
}

std::set<std::string> known(std::set<std::string> const& words, std::map<std::string, int> const& WORDS)
{
	std::set<std::string> known;
	std::set<std::string>::iterator it;

	for (it = words.begin(); it != words.end(); it++) {
		if (WORDS.find(*it) != WORDS.end()) {
			known.insert(*it);
		}
	}

	return known;
}

std::set<std::string> edits1(std::string const& word) {
	std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
	int len = word.length();
	std::set<std::string> ret;

	std::set < std::pair<std::string, std::string>> split;
	for (int i = 0; i < len; i++) {
		std::pair<std::string, std::string> parts = std::make_pair(word.substr(0, i), word.substr(i));
		split.insert(parts);
	}

	std::set <std::pair<std::string, std::string>>::iterator it;
	for (it = split.begin(); it != split.end(); it++) {
		std::string del = it->first + it->second.substr(1);
		ret.insert(del);

		if (it->second.length() > 1) {
			std::string trans = it->first + it->second.substr(1, 1) + it->second.substr(0, 1) + it->second.substr(2);
			ret.insert(trans);
		}

		std::string::iterator it2;
		for (it2 = alphabet.begin(); it2 != alphabet.end(); it2++) {
			std::string rep = it->first + *it2 + it->second.substr(1);
			ret.insert(rep);
		}

		for (it2 = alphabet.begin(); it2 != alphabet.end(); it2++) {
			std::string ins = it->first + *it2 + it->second;
			ret.insert(ins);
		}
	}

	return ret;
}

std::set<std::string> candidates(std::string const& word, std::map<std::string, int> const& WORDS)
{
	std::set<std::string> words;
	words.insert(word);
	std::set<std::string> found1 = known(words, WORDS);
	if (!found1.empty()) {
		return found1;
	}

	words.clear();
	words = edits1(word);
	std::set<std::string> found2 = known(words, WORDS);
	if (!found2.empty()) {
		return found2;
	}

	words.clear();
	words.insert(word);
	return words;
}

std::string correction(std::string const& word, std::map<std::string, int>& WORDS) {
	auto comparison = [&](std::string const& a, std::string const& b)
	{
		return P(a, WORDS) < P(b, WORDS);
	};

	std::set<std::string> cand = candidates(word, WORDS);
	auto max = std::max_element(cand.begin(), cand.end(), comparison);

	return *max;
}

int main(int argc, char* argv[])
{
	std::string word = argv[1];

	std::string name = "big";
	struct stat buffer;
	std::map<std::string, int> WORDS;

	if (stat((name + "_map.txt").c_str(), &buffer) == -1) {
		// Create the file
		parseText(name, WORDS);
	} else {
		parseMap(name, WORDS);
	}

	auto cor = correction(word, WORDS);
	std::cout << cor << std::endl;
}