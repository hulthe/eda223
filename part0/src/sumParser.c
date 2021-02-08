#include "sumParser.h"
#include "sciTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

void sumParserRead(SumParser* self, Serial* sci, int c) {
    if(self->buf_len >= SUM_PARSER_BUF_CAP) {
        return;
    }

    switch(c) {
    case 'e':
        // Parse number
        self->buf[self->buf_len] = '\0';
        int num = atoi(self->buf);
        self->sum += num;

        // Clear buffer
        self->buf_len = 0;

        // Convert the sum to a string
        char print_buf[SUM_PARSER_BUF_CAP];
        snprintf(print_buf, SUM_PARSER_BUF_CAP, "%d", self->sum);

        // Print the number
        SCI_WRITE(sci, "sum: ");
        SCI_WRITE(sci, print_buf);
        SCI_WRITE(sci, "\n");

        break;
    case 'F':
        self->sum = 0;
        self->buf_len = 0;
        break;
    default:
        self->buf[self->buf_len] = c;
        self->buf_len = self->buf_len + 1;
        break;
    }
}