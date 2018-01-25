#pragma once
#include <string>
#include <vector>

bool LoadBinaryFileToBuffer(const std::string& filePath, std::vector< unsigned char >& out_buffer);
bool SaveBufferToBinaryFile(const std::string& filePath, const std::vector< unsigned char >& buffer);
bool SaveBufferToBinaryFile(const std::string& filePath, const std::vector< std::string >& buffer);
bool DoesFileExsist(const std::string& filePath);
void* FileReadToBuffer(char const *filename, size_t *out_size);
