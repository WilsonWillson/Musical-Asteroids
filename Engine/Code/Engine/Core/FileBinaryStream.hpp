#pragma once
#include <stdio.h>
#include "Engine/Core/BinaryStream.hpp"
// So we're going to be dealing directly with files in this class, so we'll
// also implement a file version. 
// I am most familiar with C style file commands, so that is what
// I'll use in my demo, but any implementation is fine
// as long as the reuslt is binary! 

typedef unsigned char byte_t;

class FileBinaryStream : public BinaryStream
{
public:
	//----------------------------------------------------------------------------
	FileBinaryStream()
		: file_pointer(nullptr)
	{}

	//----------------------------------------------------------------------------
	~FileBinaryStream()
	{
		close();
	}


	bool open_for_read(char const *filename);
	bool open_for_write(char const *filename);
	void close();
	virtual bool read_byte(byte_t *out) override;
	virtual bool write_byte(byte_t const &value) override;


	virtual unsigned int read_bytes(void *out_buffer, unsigned int const count);
	virtual unsigned int write_bytes(void const* buffer, unsigned int const size);
	//----------------------------------------------------------------------------
	inline bool is_open() const { return nullptr != file_pointer; }

public:
	FILE *file_pointer;   // file pointer;

};