#ifndef INIT_H
#define INIT_H

extern void initSubsystems(int argc, const char *argv[]);
extern void exitSubsystems(void);
extern void initScripting(void);
extern void initConfiguration(int argc, const char *argv[]);
extern void initVideo(void);
extern void initAudio(void);
extern void initInput(void);

/* platform stuff */
#endif
