/* this is a small wrapper around the basic file io functions,
   ie open, close, read, gets */

#include "filesystem/nebu_file_io.h"

#ifdef IO_ZLIB

file_handle file_open(const char *path, const char* mode) {
  return gzopen(path, mode);
}

int file_close(file_handle file) {
  return gzclose(file);
}

int file_read(file_handle file, void* data, unsigned int size) {
  return gzread(file, data, size);
}

char* file_gets(file_handle file, char* data, unsigned int size) {
  return gzgets(file, data, size);
}

#else

file_handle file_open(const char *path, const char* mode) {
  return fopen(path, mode);
}

int file_close(file_handle file) {
  return fclose(file);
}

int file_read(file_handle file, void* data, unsigned int size) {
  return fread(data, 1, size, file);
}

char* file_gets(file_handle file, char* data, unsigned int size) {
  return fgets(data, size, file);
}

#endif
