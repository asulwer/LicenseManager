#pragma once

#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

class RandomStringGenerator
{
public:
	RandomStringGenerator();
	~RandomStringGenerator() {};

	string Generate(string pattern);
	string Generate(int min, int max);
	string Generate(int fixed);	
	
	int MaxUpper;
	int MaxLower;
	int MaxNumeric;
    int MaxSpecial;
	
private:
	string GenerateString(int length);
	string PatternDrivenAlgo(string Pattern);
	string SimpleGenerateAlgo(int length);
	string GenerateAlgoWithLimits(int length);
	void AddExistingString(string s);
	string GetRandomCharFromArray(vector<string> arr, vector<string> items);
	bool find(string val, string list);
	
	string m_pattern;
	vector<string> m_upper;
	vector<string> m_lower;
	vector<string> m_numeric;
	vector<string> m_special;
	vector<string> m_general;
	vector<string> ExistingStrings;
};
