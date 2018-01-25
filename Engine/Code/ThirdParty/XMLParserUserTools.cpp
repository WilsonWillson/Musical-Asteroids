#include "ThirdParty/XMLParserUserTools.hpp"

int ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, int defaultValue)
{
	std::string value = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (value == "")
		return defaultValue;
	return (int)atoi( value.c_str());
}

std::string ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const char* defaultValue)
{
	std::string charValue = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (charValue == "")
		return defaultValue;
	return charValue;
}

bool ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition)
{
	std::string check = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);

	if (check == "yes" || check == "Yes" || check == "Y" || check == "y")
		return true;
	else
		return false;
}

float ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, float defaultValue)
{
	std::string value = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (value == "")
		return defaultValue;
	float returnValue = std::stof(value);
	return returnValue;
}

Vector4 ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, Vector4 defaultValue)
{
	std::string rgbaValue = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (rgbaValue == "")
		return defaultValue;
	std::vector<std::string> glyphIndividualColors = ParseStringIntoPiecesByDelimiter(rgbaValue, ',');

	Vector4 glyphColor;
	glyphColor.x = std::stof(glyphIndividualColors[0]);
	glyphColor.y = std::stof(glyphIndividualColors[1]);
	glyphColor.z = std::stof(glyphIndividualColors[2]);
	glyphColor.w = std::stof(glyphIndividualColors[3]);
	return glyphColor;
}

Rgba ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, Rgba defaultValue)
{
	std::string rgbaValue = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (rgbaValue == "")
		return defaultValue;
	std::vector<std::string> glyphIndividualColors = ParseStringIntoPiecesByDelimiter(rgbaValue, ',');

	Rgba glyphColor;
	glyphColor.r = (char) atoi(glyphIndividualColors[0].c_str());
	glyphColor.g = (char)atoi(glyphIndividualColors[1].c_str());
	glyphColor.b = (char)atoi(glyphIndividualColors[2].c_str());
	glyphColor.a = (char)atoi(glyphIndividualColors[3].c_str());
	return glyphColor;
}

Vector2 ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const Vector2& defaultValue)
{
	std::string stringVec2 = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (stringVec2 == "")
		return defaultValue;
	std::vector<std::string> glyphIndividualPositions = ParseStringIntoPiecesByDelimiter(stringVec2, ',');

	Vector2 vec2;
	vec2.x = (float)atoi(glyphIndividualPositions[0].c_str());
	vec2.y = (float) atoi(glyphIndividualPositions[1].c_str());
	return vec2;
}


IntVector2 ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const IntVector2& defaultValue)
{
	std::string stringVec2 = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (stringVec2 == "")
		return defaultValue;
	std::vector<std::string> glyphIndividualPositions = ParseStringIntoPiecesByDelimiter(stringVec2, ',');

	IntVector2 vec2;
	vec2.x = atoi(glyphIndividualPositions[0].c_str());
	vec2.y = atoi(glyphIndividualPositions[1].c_str());
	return vec2;
}


std::string ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const std::string& defaultValue)
{
	std::string name = element.getChildNode(attributeName, childStep).getAttributeValue(attributePosition);
	if (name == "")
		return defaultValue;
	return name;
}

/*
void ValidateXmlElement(const XMLNode& element, const char* commaSeparatedListOfValidChildElementNames,  const char* commaSeparatedListOfValidAttributeNames)
{

}
*/