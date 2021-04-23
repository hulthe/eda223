#ifndef CLI_H
#define CLI_H

#include "TinyTimber.h"
#include "sciTinyTimber.h"

#define CLI_BUF_CAP 32

typedef struct {
    Object super;
    
    Serial* sci;
    
    int buf_len;

    char buf[CLI_BUF_CAP];
} CLI;

extern CLI cliHandler;

// "string" is a char*
void printLine(CLI* self, int string);

void read(CLI* self, int);
void initCLI(CLI* self, int);

#endif