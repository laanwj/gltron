#ifndef NEBU_FILE_IO_H
#define NEBU_FILE_IO_H
/* this is a small wrapper around the basic file io functions,
   ie open, close, read, gets */

#ifdef FILE_IO_ZLIB
#include <zlib.h>
typedef gzFile file_handle;
#else
#include <stdio.h>
typedef FILE* file_handle;
#endif

file_handle file_open(const char *path, const char *mode);
int file_close(file_handle file);

int file_read(file_handle file, void* data, unsigned int size);
char* file_gets(file_handle file, char* data, unsigned int size);

#endif
