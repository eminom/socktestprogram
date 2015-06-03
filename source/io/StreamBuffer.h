

#ifndef _STREAM_BUFFER_DEF__
#define _STREAM_BUFFER_DEF__

#include <string>

class StreamBuffer
{
public:
	StreamBuffer(const char *buffer, int length);
	StreamBuffer();
	~StreamBuffer();
	void append(char *start, int length);
	bool readInt32(int &value);
	bool readInt16(int &value);
	bool readString(std::string &str, int length);
	
	int payLength()const;
	void commit(int offset);

private:
	char *read_ptr_;
	char *write_ptr_;
	char *begin_ptr_;
	char *end_ptr_;
	bool owned_;

private:
	StreamBuffer(const StreamBuffer&);
	void operator=(const StreamBuffer&);
};



#endif