#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "../nxjson/nxjson.h"

#include <sys/stat.h>

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02
#define ERROR(msg, p) fprintf(stderr, "ERROR: " msg " %s\n", (p));

typedef struct _config
{
    int sizeX;
    int sizeY;
    int infectedPercent;
    int incubatedPercent;
    int lapsNumber;
} config;

typedef struct _outFileLine
{
    int posX;
    int posY;
    int time;
    char* state;
} outFileLine;

/**
 * Read file loaded and returns config struct
 */
config readConfigFile(char* filepath);

/**
 * Load file passed in parameter returns text
 * Copyright (c) 2013 Yaroslav Stavnichiy <yarosla@gmail.com> 
 */
char* loadFile(char* filepath);

/**
 * Display help message
 */
void displayHelp();


/**
 * Return 1 if ok
 * Return -1 if config file is incorrect
 */ 
int checkArgvParameter(char* param);

/**
 * Return 1 if the srings are equal -1 if not
 */ 
int compareStrings(char* str1, char* str2, int size);

/**
 * Return 0 if config file is ok -1 if not
 * 
 */ 
int checkConfFile(config conf);
