#ifndef PLAYER_H
#define PLAYER_H

#include "TinyTimber.h"
#include "sioTinyTimber.h"
#include "song.h"
#include "lfo.h"
#include <stdint.h>

typedef struct {
    Object super;

    Song* song;
    uint32_t currentNote;
    Time baseTempo;
    int key;
    LFOSample lfo;
    uint8_t volume;
    uint8_t muted;
    uint8_t nextNoteTaskActive;
} Player;

extern Player player;

extern SysIO sio0;

int getPlayerVolume(Player* self, int _);
int setPlayerVolume(Player* self, int volume);

int playerIsMuted(Player* self, int _);
void mutePlayer(Player* self, int _);
void unmutePlayer(Player* self, int _);

void setPlayerKey(Player* self, int receivedKey);
void setPlayerTempo(Player* self, int tempo);

// LFOSample* lfoSample
void setPlayerLFO(Player* self, int lfoSample);

// Song* song
void playerPlay(Player* self, int song);
void playerStop(Player* self, int _);

#endif