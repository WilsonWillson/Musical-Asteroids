#pragma once
#include "Engine/File/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <regex>
#include<map>

struct FontData 
{
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int xadvance;
	int yoffset;
	int page;

};

struct KearningCount
{
	int first;
	int second;
	int amount;
};

class DX11TextRenderer
{
public:
	DX11TextRenderer();
	~DX11TextRenderer();

private:
	void ParseFontFile();
	void ParseFirstRowOfFontFileInfo(std::string c);
	void ParseSecondRowOfFontFileInfo(std::string c);
	void LoopThorughLettersInFontFile(std::string c, std::stringstream& ssdata);
	void LoopThroughKearningsInFontFile(std::string c, std::stringstream& ssdata);

	int GetTextWidth(char* const text);

public:
	std::string face;
	int size;

	bool bold;
	bool italic;
	int lineHeight;
	int base;
	int scaleWidth;
	int scaleHeight;
	int pages;

	std::map <int, FontData*> letter_list;
	std::map <int, KearningCount*> kearning_list;
};