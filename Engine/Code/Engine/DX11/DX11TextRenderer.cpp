#include "Engine/DX11/DX11TextRenderer.hpp"


DX11TextRenderer::DX11TextRenderer()
{
	ParseFontFile();
	//GetTextWidth("bat");
}


void DX11TextRenderer::ParseFontFile()
{
	std::string c= ""; // read chars
	std::stringstream ssdata;

	size_t sourceSize = 0U;
	void *sourceCode = FileReadToBuffer("data/arial32.fnt", &sourceSize);
	ssdata.write((const char*)sourceCode, sourceSize);

	std::getline(ssdata, c);
	ParseFirstRowOfFontFileInfo(c);	//Need face, size, bold & italic

	std::getline(ssdata, c);
	ParseSecondRowOfFontFileInfo(c);//Need Line height, base, scaleW, scaleH, pages
	
	std::getline(ssdata, c); //Lines Not used
	
	LoopThorughLettersInFontFile(c, ssdata);

	LoopThroughKearningsInFontFile(c, ssdata);

}

void DX11TextRenderer::ParseFirstRowOfFontFileInfo(std::string c)
{
	std::smatch m;
	std::string  temp = "";

	std::regex eSize("size=-?\\d+");
	std::regex_search(c, m, eSize);
	temp = m[0];
	temp = temp.substr(strlen("size="));
	size = stoi(temp);

	std::regex eBold("bold=-?\\d+");
	std::regex_search(c, m, eBold);
	temp = m[0];
	temp = temp.substr(strlen("bold="));
	if (temp == "0")
		bold = false;
	else
		bold = true;

	std::regex eItalic("italic=-?\\d+");
	std::regex_search(c, m, eItalic);
	temp = m[0];
	temp = temp.substr(strlen("italic="));
	if (temp == "0")
		italic = false;
	else
		italic = true;
}

void DX11TextRenderer::ParseSecondRowOfFontFileInfo(std::string c)
{
	std::smatch m;
	std::regex eLineHeight("lineHeight=-?\\d+");
	std::regex_search(c, m, eLineHeight);
	std::string  temp = m[0];
	temp = temp.substr(strlen("lineHeight="));
	lineHeight = stoi(temp);

	std::regex eBase("base=-?\\d+");
	std::regex_search(c, m, eBase);
	temp = m[0];
	temp = temp.substr(strlen("base="));
	base = stoi(temp);

	std::regex eScaleWidth("scaleW=-?\\d+");
	std::regex_search(c, m, eScaleWidth);
	temp = m[0];
	temp = temp.substr(strlen("scaleW="));
	scaleWidth = stoi(temp);

	std::regex eScaleHeight("scaleH=-?\\d+");
	std::regex_search(c, m, eScaleHeight);
	temp = m[0];
	temp = temp.substr(strlen("scaleH="));
	scaleHeight = stoi(temp);

	std::regex ePages("pages=-?\\d+");
	std::regex_search(c, m, ePages);
	temp = m[0];
	temp = temp.substr(strlen("pages="));
	pages = stoi(temp);
}

void DX11TextRenderer::LoopThorughLettersInFontFile(std::string c, std::stringstream& ssdata)
{
	std::smatch m;
	int charCount;
	std::regex eCharsCount("chars count=-?\\d+");

	std::getline(ssdata, c);
	std::regex_search(c, m, eCharsCount);

	std::string temp = m[0];
	temp = temp.substr(strlen("chars count="));
	charCount = stoi(temp);

	//Now Loop to get all data
	for (int i = 0; i < charCount; i++)
	{
		int id = 0;
		std::getline(ssdata, c); // now we iterate over the stringstream, char by char
		FontData* newFont = new FontData();

		std::regex eId("id=-?\\d+");
		std::regex_search(c, m, eId);
		temp = m[0];
		temp = temp.substr(strlen("id="));
		id = stoi(temp);


		std::regex eX("x=-?\\d+");
		std::regex_search(c, m, eX);
		temp = m[0];
		temp = temp.substr(strlen("x="));
		newFont->x = stoi(temp);

		std::regex eY("y=-?\\d+");
		std::regex_search(c, m, eY);
		temp = m[0];
		temp = temp.substr(strlen("y="));
		newFont->y = stoi(temp);


		std::regex eWidth("width=-?\\d+");
		std::regex_search(c, m, eWidth);
		temp = m[0];
		temp = temp.substr(strlen("width="));
		newFont->width = stoi(temp);

		std::regex eHeight("height=-?\\d+");
		std::regex_search(c, m, eHeight);
		temp = m[0];
		temp = temp.substr(strlen("height="));
		newFont->height = stoi(temp);


		std::regex eXOffset("xoffset=-?\\d+");
		std::regex_search(c, m, eXOffset);
		temp = m[0];
		temp = temp.substr(strlen("xoffset="));
		newFont->xoffset = stoi(temp);

		std::regex eYOffset("yoffset=-?\\d+");
		std::regex_search(c, m, eYOffset);
		temp = m[0];
		temp = temp.substr(strlen("yoffset="));
		newFont->yoffset = stoi(temp);

		std::regex ePage("page=-?\\d+");
		std::regex_search(c, m, ePage);
		temp = m[0];
		temp = temp.substr(strlen("page="));
		newFont->page = stoi(temp);

		std::regex eXAdvance("xadvance=-?\\d+");
		std::regex_search(c, m, eXAdvance);
		temp = m[0];
		temp = temp.substr(strlen("xadvance="));
		newFont->xadvance = stoi(temp);

		//Insert into map and go to next key       
		letter_list.insert(std::pair<int, FontData*>(id, newFont));
	}
}

void DX11TextRenderer::LoopThroughKearningsInFontFile(std::string c, std::stringstream& ssdata)
{
	std::smatch m;
	int kearningCount;
	std::regex eCount("kernings count=-?\\d+");

	std::getline(ssdata, c);
	std::regex_search(c, m, eCount);


	std::string temp = m[0];
	temp = temp.substr(strlen("kernings count="));
	kearningCount = stoi(temp);

	int keyCounter = 0;
	for (int i = 0; i < kearningCount; i++)
	{
		std::getline(ssdata, c); // now we iterate over the stringstream, char by char
		KearningCount* newKearning = new KearningCount();

		std::regex eFirst("first=-?\\d+");
		std::regex_search(c, m, eFirst);
		temp = m[0];
		temp = temp.substr(strlen("first="));
		newKearning->first = stoi(temp);

		std::regex eSecond("second=-?\\d+");
		std::regex_search(c, m, eSecond);
		temp = m[0];
		temp = temp.substr(strlen("second="));
		newKearning->second = stoi(temp);

		std::regex eAmount("amount=-?\\d+");
		std::regex_search(c, m, eAmount);
		temp = m[0];
		temp = temp.substr(strlen("amount="));
		newKearning->first = stoi(temp);

		kearning_list.insert(std::pair<int, KearningCount*>(keyCounter, newKearning));
	}
}

DX11TextRenderer::~DX11TextRenderer()
{

}


//----------------------Getters---------------------------
int DX11TextRenderer::GetTextWidth(char* const text)
{
	unsigned int cursor = 0;
	std::map<int, FontData*>::iterator it;
	std::map<int, FontData*>::iterator nextIt;
	char const* c = text;
	int previousCharacterCode = -1;

	while (*c != NULL)
	{
		it = letter_list.find(*c);
		if (it == letter_list.end())
		{
			return 0;
			//Invalid Letter
		}

		cursor += it->second->xadvance;
		previousCharacterCode = *c;
		c++;

		int kerning = 0;
		nextIt = letter_list.find(*c);

		for (std::map<int, KearningCount*>::iterator step = kearning_list.begin(); step != kearning_list.end(); ++step)
		{
			if (step->second->first == it->first && step->second->second == nextIt->first)
				kerning = step->second->amount;
		}

		cursor += kerning;
	}

	return cursor;
}