#include <stdlib.h>
#include "Utils.h"
#include "Logging.h"

extern char* readBinaryFile(const char* filename, unsigned int *filesize);

CharBuffer::CharBuffer()
{
    m_data = NULL;
    m_size = 0;
}

CharBuffer::~CharBuffer()
{
    LOG_INFO("Clearing data for CharBuffer: %s\n", m_file.c_str());
    free(m_data);
    m_data=NULL;
    m_size=0;
}

bool CharBuffer::createFromFile(const char *filename)
{
    m_file = filename;
    m_data = readBinaryFile(filename,&m_size);
    if(m_data==NULL)
    {
        LOG_ERROR("Fail to read from file:\n",filename);
        return false;
    }
    return true;
}

const char* CharBuffer::getData()
{
    return m_data;
}

unsigned int CharBuffer::getDataSize()
{
    return m_size;
}