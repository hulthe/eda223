#ifndef SUM_PARSER_H
#define SUM_PARSER_H

#include "sciTinyTimber.h"

#define SUM_PARSER_BUF_CAP 30

typedef struct {
    char buf[SUM_PARSER_BUF_CAP];
    int buf_len;
    int sum;
} Parser;

#define initParser() \
    { {0}, 0, 0 }

void parserInput(Parser* self, Serial* sci, int c);

#endif