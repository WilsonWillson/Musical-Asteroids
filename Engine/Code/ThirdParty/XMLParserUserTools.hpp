#pragma once
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "ThirdParty/XMLParser/XMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <string>


int ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, int defaultValue);
std::string ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const char* defaultValue);
bool ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition);
float ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, float defaultValue);
Rgba ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, Rgba defaultValue);
Vector2 ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const Vector2& defaultValue);
IntVector2 ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const IntVector2& defaultValue);
std::string ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, const std::string& defaultValue);


Vector4 ParseXmlAttribute(const XMLNode& element, const char* attributeName, int childStep, int attributePosition, Vector4 defaultValue);
void ValidateXmlElement(const XMLNode& element,		// ASSERTS that “element” has no
		const char* commaSeparatedListOfValidChildElementNames,	// child elements or attributes with 
		const char* commaSeparatedListOfValidAttributeNames); 	// names other than those given.
