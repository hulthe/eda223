#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "player.h"
#include "cli.h"
#include "util.h"
#include "candler.h"
#include "lfo.h"
#include <stdlib.h>
#include <stdio.h>

CLI cliHandler = {initObject(), NULL, 0, {0}};

const char* PROMPT = "> ";

typedef struct {
    int name;
    int arg;
} CMD;

void read(CLI*, int);
void printInt(Serial* sci, int i);

void initCLI(CLI* self, int sci_ptr) {
    self->sci = (Serial*)sci_ptr;
    SCI_WRITE(self->sci, "list of songs:\n");
    for(int i = 0; i < SONG_LIST_LEN; i++) {
        SCI_WRITE(self->sci, "  ");
        printInt(self->sci, i);
        SCI_WRITE(self->sci, ": ");
        SCI_WRITE(self->sci, SONG_LIST[i]->name);
        SCI_WRITE(self->sci, "\n");
    }
    SCI_WRITE(self->sci, "\n");
    SCI_WRITE(self->sci, "type \"h<enter>\" for a list of commands\n");
    SCI_WRITE(self->sci, PROMPT);
}

int isWhitespace(int c) {
    switch (c) {
    case ' ':
    case '\t':
    case '\n':
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

void consumeWhitespace(CLI* self, int* i) {
    while(*i < self->buf_len) {
        if(isWhitespace(self->buf[*i])) {
            *i += 1;
        } else {
            return;
        }
    }
}

int consumeName(CLI* self, int* i) {
    if(*i >= self->buf_len) {
        return 0;
    }

    int c = self->buf[*i];

    while(*i < self->buf_len && !isWhitespace(self->buf[*i])) {
        *i += 1;
    }

    return c;
}

int consumeNum(CLI* self, int* i) {
    if(*i >= self->buf_len) {
        return 0;
    }

    int j = 0;
    char s[CLI_BUF_CAP] = {0};

    while(*i < self->buf_len && !isWhitespace(self->buf[*i])) {
        s[j] = self->buf[*i];
        *i += 1;
        j += 1;
    }

    return atoi(s);
}

CMD parseCMD(CLI* self) {
    int i = 0;
    consumeWhitespace(self, &i);
    int name = consumeName(self, &i);
    consumeWhitespace(self, &i);
    int arg = consumeNum(self, &i);

    CMD cmd = {name, arg};
    return cmd;
}

void handleCmd(CLI* self, CMD cmd) {
    switch(cmd.name) {
    case 'h':;
        SCI_WRITE(self->sci, "list of commands:\n");
        SCI_WRITE(self->sci, "  h:       show this text\n");
        SCI_WRITE(self->sci, "  p <num>: play song <num>!\n");
        SCI_WRITE(self->sci, "  s:       stop playing\n");
        SCI_WRITE(self->sci, "  m:       mute/unmute\n");
        SCI_WRITE(self->sci, "  q:       increase volume by 1\n");
        SCI_WRITE(self->sci, "  a:       decrease the volume by 1\n");
        SCI_WRITE(self->sci, "  v <num>: set the volume\n");
        SCI_WRITE(self->sci, "  k <num>: set the key\n");
        SCI_WRITE(self->sci, "  t <num>: set the tempo\n");
        SCI_WRITE(self->sci, "  l:       toggle CAN leader/slave-mode\n");
        SCI_WRITE(self->sci, "  c:       toggle printing info about CAN messages\n");
        SCI_WRITE(self->sci, "  i <num>: set the LFO intensity to <num> percent\n");
        SCI_WRITE(self->sci, "  w <num>: set the LFO waveform. 1 for square wave, 2 for sawtooth wave and 3 for sinus wave\n");
        SCI_WRITE(self->sci, "  f <num>: Set the LFO frequency to <num> dHz\n");
        SCI_WRITE(self->sci, "  o <num>: Set the parameter to modulate with LFO. 1 for volume, 2 for tempo and 3 for period\n");
        break;

    case 'm':;
        {
            int muted = SYNC(&player, playerIsMuted, 0);
            if (muted) {
                Command command = {CMD_UNMUTE};
                SYNC(&candler, sendCommand, (int)&command);
                SCI_WRITE(self->sci, "Unmuted\n");
                muted = 0;
            }
            else {
                Command command = {CMD_MUTE};
                SYNC(&candler, sendCommand, (int)&command);
                SCI_WRITE(self->sci, "Muted\n");
                muted = 1;
            }
        }
        break;

    case 'q':; // Increase the volume by 1
        {
            int volume = SYNC(&player, getPlayerVolume, NULL);
            volume = min(max(volume + 1, 1), 10);

            Command command = {CMD_SET_VOLUME, volume};
            SYNC(&candler, sendCommand, (int)&command);
        }
        break;

    case 'a':; // Decrease the volume by 1
        {
            int volume = SYNC(&player, getPlayerVolume, NULL);
            volume = min(max(volume - 1, 1), 10);

            Command command = {CMD_SET_VOLUME, volume};
            SYNC(&candler, sendCommand, (int)&command);
        }
        break;

    case 'v':; // Set the volume
        {
            int volume = cmd.arg;
            if ((volume > 0) && (volume <= 10)) {
                Command command = {CMD_SET_VOLUME, volume};
                SYNC(&candler, sendCommand, (int)&command);

                SCI_WRITE(self->sci, "New volume set\n");
            } else {
                SCI_WRITE(self->sci, "Error! Volume out of range!\nVolumerange: 1 - 10\n");
            }
        }
        break;

    case 'p':; // play a song!
        {
            if (cmd.arg >= SONG_LIST_LEN || cmd.arg < 0) {
                SCI_WRITE(self->sci, "Song index out of range [0..");
                printInt(self->sci, SONG_LIST_LEN - 1);
                SCI_WRITE(self->sci, "].\n");
            } else {
                Command command = {CMD_PLAY_START, cmd.arg};
                SYNC(&candler, sendCommand, (int)&command);
            }
        }
        break;

    case 's':; // stop playing
        {
            //ASYNC(&player, playerStop, NULL);
            Command command = {CMD_PLAY_STOP};
            SYNC(&candler, sendCommand, (int)&command);
        }
        break;

    case 't':; // Set music player tempo
        {
            int tempo = cmd.arg;
            if(tempo >= 60 && tempo <= 240) {
                Command command = {CMD_SET_TEMPO, tempo};
                SYNC(&candler, sendCommand, (int)&command);

                SCI_WRITE(self->sci, "Tempo set to ");
                printInt(self->sci, tempo);
                SCI_WRITE(self->sci, " bpm\n");
            } else {
                SCI_WRITE(self->sci, "Tempo out of range [60..240]\n");
            }
        }
        break;

    case 'k':; // Set music player key
        {
            int key = cmd.arg;
            if ((key <= 5) && (key >= -5)) {
                Command command = {CMD_SET_KEY, key};
                SYNC(&candler, sendCommand, (int)&command);

                SCI_WRITE(self->sci, "Now playing in key ");
                printInt(self->sci, key);
                SCI_WRITE(self->sci, "\n");
            }
            else {
                SCI_WRITE(self->sci, "Key out of range\n");
            }
        }
        break;

    case 'l':; // Toggle leader mode
    {
        if(SYNC(&candler, toggleLeaderMode, 0)) {
            SCI_WRITE(self->sci, "Now running in leader-mode\n");
        } else {
            SCI_WRITE(self->sci, "Now running in slave-mode\n");
        }
        break;
    }

    case 'c':;
    {
        ASYNC(&candler, toggleCanId, 0);
        break;
    }

    case 'f':;
    {
        int frequency = cmd.arg;
        if (frequency >= 2 && frequency <= 50) {
            Time period = SEC(1) * 10 / frequency;
            ASYNC(&lfOscillator, setLFOPeriod, period);
            SCI_WRITE(self->sci, "Frequency set\n");
        }
        else {
            SCI_WRITE(self->sci, "Frequency out of range [2, 50]dHz\n");
        }

        break;
    }


    case 'i':;
    {
        int inten = cmd.arg;
        if (inten >= 0 && inten <= 100) {
            ASYNC(&lfOscillator, setLFOIntensity, inten);
            SCI_WRITE(self->sci, "Intensity set\n");
        }
        else {
            SCI_WRITE(self->sci, "Intensity out of range\n");
        }

        break;
    }

    case 'w':;
    {
        int wave = cmd.arg;
        if (wave >= 1 && wave <= 3) {
            ASYNC(&lfOscillator, setLFOWaveform, wave);
            SCI_WRITE(self->sci, "Waveform set\n");
        }
        else {
            SCI_WRITE(self->sci, "Not a waveform\n");
        }

        break;
    }

    case 'o':;
    {
        int param = cmd.arg;
        if (param >= 1 && param <= 3) {
            ASYNC(&lfOscillator, setLFOParameter, param);
            SCI_WRITE(self->sci, "Target parameter set\n");
        }
        else {
            SCI_WRITE(self->sci, "Not a parameter\n");
        }

        break;
    }

    case '\0':;
        break;

    default:;
        SCI_WRITE(self->sci, "unknown command\n");
        break;
    }
}

void printInt(Serial* sci, int i) {
    // Buffer for generating strings
    char print_buf[CLI_BUF_CAP];

    // Convert the sum to a string
    snprintf(print_buf, CLI_BUF_CAP, "%d", i);

    SCI_WRITE(sci, print_buf);
}

void clearPrompt(CLI* self) {
    for (int i = 0; i < self->buf_len + 2; i++) {
        SCI_WRITECHAR(self->sci, 0x08 /* backspace */);
    }
    for (int i = 0; i < self->buf_len + 2; i++) {
        SCI_WRITECHAR(self->sci, ' ');
    }
    for (int i = 0; i < self->buf_len + 2; i++) {
        SCI_WRITECHAR(self->sci, 0x08 /* backspace */);
    }
}

void showPrompt(CLI* self) {
    SCI_WRITE(self->sci, PROMPT);
    for (int i = 0; i < self->buf_len; i++) {
        SCI_WRITECHAR(self->sci, self->buf[i]);
    }
}

void read(CLI* self, int c) {
    switch (c) {
    case '\n':;
    case '\r':;
        self->buf[self->buf_len] = '\0';
        CMD cmd = parseCMD(self);
        self->buf_len = 0;

        SCI_WRITE(self->sci, "\n");
        handleCmd(self, cmd);
        SCI_WRITE(self->sci, PROMPT);
        break;
    case 0x08:;
    case 0x7F:; // handle backspace
        if(self->buf_len > 0) {
            clearPrompt(self);
            self->buf_len -= 1;
            showPrompt(self);
        }
        break;
    default:;
        if(self->buf_len >= CLI_BUF_CAP) {
            return;
        }
        clearPrompt(self);
        self->buf[self->buf_len] = c;
        self->buf_len = self->buf_len + 1;
        showPrompt(self);
        break;
    }
}

void printLine(CLI* self, int ptr) {
    char* s = (char*)ptr;
    clearPrompt(self);
    SCI_WRITE(self->sci, s);
    SCI_WRITE(self->sci, "\n");
    showPrompt(self);
}
