#include "parser.h"
#include "sciTinyTimber.h"
#include "frequencies.h"
#include <stdlib.h>
#include <stdio.h>

int parseBuf(Parser* self);
void parseKey(Parser* self, Serial* sci);
void parseSum(Parser* self, Serial* sci);

void parserInput(Parser* self, Serial* sci, int c) {
    if(self->buf_len >= SUM_PARSER_BUF_CAP) {
        return;
    }

    switch(c) {
    case 'k': // Print the periods for brother jacob in a given key
        parseKey(self, sci);
        break;

    case 'e': // Print the sum of this and all pervious numbers
        parseSum(self, sci);
        break;

    case 'F': // Reset the sum
        self->sum = 0;
        self->buf_len = 0;
        break;
        
    default: // Add input to parse buffer
        self->buf[self->buf_len] = c;
        self->buf_len = self->buf_len + 1;
        break;
    }
}

int parseBuf(Parser* self) {
    // Add terminating null char
    self->buf[self->buf_len] = '\0';

    // Parse number
    int num = atoi(self->buf);

    // Clear buffer
    self->buf_len = 0;

    return num;
}

void parseKey(Parser* self, Serial* sci) {
    // Buffer for generating strings
    char print_buf[SUM_PARSER_BUF_CAP];

    int key = parseBuf(self);

    if ((key >= -5) & (key <= 5)) {
        const int BROTHER_JACOB_NOTES[32] = {
            0, 2, 4, 0, 0, 2, 4, 0, 4, 5, 7, 4, 5, 7, 7, 9,
            7, 5, 4, 0, 7, 9, 7, 5, 4, 0, 0, -5, 0, 0, -5, 0
        };

        // Print the key
        SCI_WRITE(sci, "key: ");
        snprintf(print_buf, SUM_PARSER_BUF_CAP, "%d", key);
        SCI_WRITE(sci, print_buf);
        SCI_WRITE(sci, "\n");

        // Print all the periods for the notes in brother jacob
        for (int i = 0; i < 32; i++) {
            int note = BROTHER_JACOB_NOTES[i];
            int p = period(note, key);

            if (i > 0) {
                SCI_WRITE(sci, " ");
            }

            // Print the period
            snprintf(print_buf, SUM_PARSER_BUF_CAP, "%d", p);
            SCI_WRITE(sci, print_buf);
        }
        SCI_WRITE(sci, "\n");
    } else {
        SCI_WRITE(sci, "key out of range (-5 <= key <= 5)\n");
    }
}

void parseSum(Parser* self, Serial* sci) {
    int num = parseBuf(self);

    self->sum += num;

    // Buffer for generating strings
    char print_buf[SUM_PARSER_BUF_CAP];

    // Convert the sum to a string
    snprintf(print_buf, SUM_PARSER_BUF_CAP, "%d", self->sum);

    // Print the number
    SCI_WRITE(sci, "sum: ");
    SCI_WRITE(sci, print_buf);
    SCI_WRITE(sci, "\n");
}