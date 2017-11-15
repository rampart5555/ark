#include <stdlib.h>
#include <stdio.h>

char* readBinaryFile(const char *filename, unsigned int *file_len)
{
    char *buffer = 0;
    long length;
    FILE *fp = fopen (filename, "rb");

    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = (char*)malloc (length + 1);
        fread (buffer, 1, length, fp);
        buffer[length]=0;
        fclose (fp);
    }
    *file_len = length;
    return buffer;
}    
