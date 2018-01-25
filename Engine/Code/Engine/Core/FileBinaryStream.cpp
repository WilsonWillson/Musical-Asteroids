#define _CRT_SECURE_NO_WARNINGS
#include "Engine/Core/FileBinaryStream.hpp"
#include "Engine/Core/BinaryStream.hpp"
//----------------------------------------------------------------------------
bool FileBinaryStream::open_for_read(char const *filename)
{
//	ASSERT(!is_open());
	file_pointer = fopen(filename, "rb"); // open for read & binary
	return is_open();
}

//----------------------------------------------------------------------------
bool FileBinaryStream::open_for_write(char const *filename)
{
	//ASSERT(!is_open());
	file_pointer = fopen(filename, "wb"); // open for read & binary
	return is_open();
}

//----------------------------------------------------------------------------
void FileBinaryStream::close()
{
	if (is_open()) {
		fclose(file_pointer);
		file_pointer = nullptr;
	}
}


//----------------------------------------------------------------------------
// BinaryStream Impl
//----------------------------------------------------------------------------
bool FileBinaryStream::read_byte(byte_t *out)
{
	return (read_bytes(out, 1) == 1);
}

//----------------------------------------------------------------------------
bool FileBinaryStream::write_byte(byte_t const &value)
{
	return (write_bytes(&value, 1) == 1);
}


//----------------------------------------------------------------------------
// read/write 'count' bytes.  Returns number of bytes actually read. 
// will return 0 on failure. 
unsigned int FileBinaryStream::read_bytes(void *out_buffer, unsigned int const count)
{
	unsigned int bytes_read = 0;
	if (is_open()) {
		bytes_read = (unsigned int)fread(out_buffer, 1, count, file_pointer);
	}

	return bytes_read;
}

//----------------------------------------------------------------------------
unsigned int FileBinaryStream::write_bytes(void const* buffer, unsigned int const size)
{
	unsigned int bytes_read = 0;
	if (is_open()) {
		bytes_read = (unsigned int)fwrite(buffer, 1, size, file_pointer);
	}

	return bytes_read;
}
