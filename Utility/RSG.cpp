#include "Stdafx.h"
#include "RSG.h"

RandomStringGenerator::RandomStringGenerator()
{
	srand((unsigned int)time(NULL));

	MaxUpper = 0;
	MaxLower = 0;
	MaxNumeric = 0;
	MaxSpecial = 0;

	m_pattern = "";

	std::string upper[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
	m_upper.assign(upper, end(upper));
	
	std::string lower[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
	std::vector<std::string> vlower;
	m_lower.assign(lower, end(lower));
	
	std::string numeric[] = {"0","1","2","3","4","5","6","7","8","9"};
	std::vector<std::string> vnumeric;
	m_numeric.assign(numeric, end(numeric));
	
	std::string special[] = {",",".",";",":","?","!","/","@","#","$","%","^","&","(",")","=","+","*","-","_","{","}","[","]","<",">","|","~"};
	std::vector<std::string> vspecial;
	m_special.assign(special,end(special));
    
	m_general.insert(m_general.end(),m_upper.begin(), m_upper.end());
	m_general.insert(m_general.end(),m_lower.begin(), m_lower.end());
	m_general.insert(m_general.end(),m_numeric.begin(), m_numeric.end());
	m_general.insert(m_general.end(),m_special.begin(), m_special.end());
}

std::string RandomStringGenerator::Generate(std::string pattern)
{
	m_pattern = pattern;
	std::string res = GenerateString((int)m_pattern.size());
	m_pattern = "";
	
	return res;
}

std::string RandomStringGenerator::Generate(int min, int max)
{
	if(max<min)
		return "";
	else
		return GenerateString(min + (rand() % (max - min)));
}

std::string RandomStringGenerator::Generate(int fixed)
{
	return GenerateString(fixed);
}

std::string RandomStringGenerator::GenerateString(int length)
{
	std::string result = "";

    if (m_pattern.size() > 0)
        result = PatternDrivenAlgo(m_pattern);
    else if (MaxUpper == 0 && MaxLower == 0 && MaxNumeric == 0 && MaxSpecial == 0)
        result = SimpleGenerateAlgo(length);
    else
        result = GenerateAlgoWithLimits(length);
    
	for(unsigned int i=0; i<ExistingStrings.size(); i++)
	{
		if(ExistingStrings.at(i).find(result) != std::string::npos)
			return GenerateString(length);
	}
		
	AddExistingString(result);
    return result;
}

std::string RandomStringGenerator::PatternDrivenAlgo(std::string Pattern)
{
	std::string result = "";

	std::vector<std::string> Characters;
	for(unsigned int i=0; i<Pattern.size(); i++)
	{
		std::string newChar = " ";
		switch(Pattern[i])
		{
		case 'L':
			newChar = GetRandomCharFromArray(m_upper, Characters);
			break;
		case 'l':
			newChar = GetRandomCharFromArray(m_lower, Characters);
			break;
		case 'n':
			newChar = GetRandomCharFromArray(m_numeric, Characters);
			break;
		case 's':
			newChar = GetRandomCharFromArray(m_special, Characters);
			break;
		case '*':
			newChar = GetRandomCharFromArray(m_general, Characters);
			break;
		default:
			break;
		}

		Characters.resize(Characters.size()+1);
		Characters[Characters.size()-1] = newChar;
		result += newChar;
	}

	return result;
}

std::string RandomStringGenerator::SimpleGenerateAlgo(int length)
{
	std::string result = "";

	for (int i = 0; i < length; i++)
    {
		std::string newChar = m_general[rand() % m_general.size()];
        
		if (find(newChar, result))
        {
            do
            {
                newChar = m_general[rand() % m_general.size()];
            } while (find(newChar, result));
        }
        result += newChar;
    }

	return result;
}

std::string RandomStringGenerator::GenerateAlgoWithLimits(int length)
{
	std::string result = "";
	std::vector<std::string> c;

	c.resize(MaxUpper);
	int iCount = (int)c.size()-1;
	for (int i = iCount; i > (iCount-MaxUpper); i--)
		c[i] = GetRandomCharFromArray(m_upper,c);
	
	c.resize(MaxLower+c.size());
	iCount = (int)c.size()-1;
	for (int i = iCount; i > (iCount-MaxLower); i--)
        c[i] = GetRandomCharFromArray(m_lower, c);
    
	c.resize(MaxNumeric+c.size());
	iCount = (int)c.size()-1;
	for (int i = iCount; i > (iCount-MaxNumeric); i--)
        c[i] = GetRandomCharFromArray(m_numeric, c);
    
	c.resize(MaxSpecial+c.size());
	iCount = (int)c.size()-1;
	for (int i = iCount; i > (iCount-MaxSpecial); i--)
        c[i] = GetRandomCharFromArray(m_special, c);
    
	for (int i = 0; i < length; i++)
    {
        int position = rand() % c.size();
		std::string CurrentChar = c[position];
        result += CurrentChar;
	}

	return result;
}

void RandomStringGenerator::AddExistingString(std::string s)
{
	//resize array and add append to end
	ExistingStrings.resize(ExistingStrings.size()+1);
	ExistingStrings[ExistingStrings.size()-1] = s;
}

std::string RandomStringGenerator::GetRandomCharFromArray(std::vector<std::string> arr, std::vector<std::string> items)
{
	std::string val;
	bool bFlag = false;
	do
	{
		val = arr.at(rand() % arr.capacity());
		
		for(unsigned int i=0; i<items.size(); i++)
		{
			if(items.at(i).find(val) != std::string::npos)
				bFlag = true;
			else
				bFlag = false;
		}

	} while(bFlag);
	
	return val;
}

bool RandomStringGenerator::find(std::string val, std::string list)
{
	bool bFlag = false;
	
	for(unsigned int i=0; i<list.size(); i++)
	{
		if(list.find(val) != std::string::npos)
			bFlag = true;
	}

	return bFlag;
}