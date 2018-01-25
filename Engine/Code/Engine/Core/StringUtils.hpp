#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>

//-----------------------------------------------------------------------------------------------
const std::string Stringf( const char* format, ... );
const std::string Stringf( const int maxLength, const char* format, ... );

std::vector <std::string> ParseStringIntoPiecesByDelimiter(std::string word, char delimiter);

const bool StringIsNullOrEmpty(std::string node_name);


