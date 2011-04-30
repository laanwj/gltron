#include "SDL_rwops.h"

SDL_RWops* bs_RWop;

#define BUFSIZE 65536
#define LOW_WATER 16384

static int bs_read(SDL_RWops *context, void *ptr, int size, int maxnum)
{
}

static int bs_close()
{
}
										
SDL_RWops* bs_RWFromFile(const char *file) {
  bs_RWop = SDL_AllocRW();
  bs_RWop->read = bs_read;
  bs_RWop->seek = NULL;
  bs_RWop->write = NULL;
  bs_RWop->close = bs_close;
}
