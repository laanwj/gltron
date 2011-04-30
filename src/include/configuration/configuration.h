#ifndef CONFIGURATION_H
#define CONFIGURATION_H

void parse_args(int argc, const char *argv[]);
void updateSettingsCache(void);
void initColors(void);
void checkSettings(void);
void saveSettings(void);

#endif
