#pragma once

#include <vector>
#include <ctime>
#include <algorithm>
#include <string>

class RandomStringGenerator
{
public:
	RandomStringGenerator();
	~RandomStringGenerator() {};

	std::string Generate(std::string pattern);
	std::string Generate(int min, int max);
	std::string Generate(int fixed);
	
	int MaxUpper;
	int MaxLower;
	int MaxNumeric;
    int MaxSpecial;
	
private:
	std::string GenerateString(int length);
	std::string PatternDrivenAlgo(std::string Pattern);
	std::string SimpleGenerateAlgo(int length);
	std::string GenerateAlgoWithLimits(int length);
	void AddExistingString(std::string s);
	std::string GetRandomCharFromArray(std::vector<std::string> arr, std::vector<std::string> items);
	bool find(std::string val, std::string list);
	
	std::string m_pattern;
	std::vector<std::string> m_upper;
	std::vector<std::string> m_lower;
	std::vector<std::string> m_numeric;
	std::vector<std::string> m_special;
	std::vector<std::string> m_general;
	std::vector<std::string> ExistingStrings;
};
