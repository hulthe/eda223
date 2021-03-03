#include "TinyTimber.h"
#include "player.h"
#include "toneGenerator.h"
#include "frequencies.h"
#include "cli.h"
#include "song.h"
#include <stdio.h>

Player player = { initObject(), (Song*)NULL, 0, MSEC(500), 0, 3, 0, 0};

const Time NOTE_SILENCE = MSEC(50);
const Time PLAYER_DEADLINE = USEC(100);

void nextNote(Player* self, int _);

// Time pauseLength
void notePause(Player* self, int pauseLength);

int getPlayerVolume(Player* self, int _) {
    return self->volume;
}

int setPlayerVolume(Player* self, int volume) {
    if ((volume > 0) && (volume <= 10)) {
        self->volume = volume;
        return 0;
    } else {
        return -1;
    }
}

int muteUnmutePlayer(Player* self, int _) {
    self->muted = !self->muted;
    return self->muted;
}

void setPlayerKey(Player* self, int receivedKey) {
    self->key = receivedKey;
}

void setPlayerTempo(Player* self, int receivedTempo) {
    self->baseTempo = MSEC((60 * 1000) / receivedTempo);
}

Time noteLength(const Player* self, const Note* note) {
    Time noteLength = self->baseTempo * note->tempo / BASE_BEAT;
    return noteLength;
}

void nextNote(Player* self, int _) {
    if (self->song == (Song*)NULL) {
        self->nextNoteTaskActive = 0;
        return;
    }

    // if all notes have been played, repeat
    if (self->currentNote >= self->song->length) {
        self->currentNote = 0;
    }

    // start the tone generator with the appropriate settings
    Note note = self->song->notes[self->currentNote++];
    int tonePeriod = period(note.note, self->key);
    BEFORE(PLAYER_DEADLINE, &toneGenerator, setGeneratorTonePeriod, tonePeriod);

    if (self->muted) {
        BEFORE(PLAYER_DEADLINE, &toneGenerator, muteGenerator, NULL);
    } else {
        BEFORE(PLAYER_DEADLINE, &toneGenerator, setGeneratorVolume, self->volume);
        BEFORE(PLAYER_DEADLINE, &toneGenerator, unmuteGenerator, NULL);
    }

    Time totalLength = noteLength(self, &note);
    Time pauseLength = totalLength / 10;
    Time toneLength = totalLength - pauseLength;

    // call notePause after the note has been playing for the appropriate time
    SEND(toneLength, PLAYER_DEADLINE, self, notePause, pauseLength);
}

// Time pauseLength
void notePause(Player* self, int pauseLength) {
    // mute the tone generator
    BEFORE(PLAYER_DEADLINE, &toneGenerator, muteGenerator, NULL);

    // play the next note after some amount of silence
    SEND((Time)pauseLength, PLAYER_DEADLINE, self, nextNote, NULL);
}

void playerPlay(Player* self, int song_ptr) {
    if (!self->nextNoteTaskActive) {
        self->nextNoteTaskActive = 1;
        ASYNC(self, nextNote, 0);
    }

    self->song = (Song*)song_ptr;
    self->currentNote = 0;

    char s[100];
    snprintf(s, 100, "Playing song: %s", self->song->name);
    SYNC(&cliHandler, printLine, (int)s);
}

void playerStop(Player* self, int _) {
    if(self->song != (Song*)NULL) {
        self->song = (Song*)NULL;
        SYNC(&cliHandler, printLine, (int)"Stopped playing.");
    }
}
