#ifndef SONG_H
#define SONG_H

typedef struct {
    int note;
    int tempo;
} Note;

typedef struct {
    const char* name;
    int length;
    Note* notes;
} Song;

extern const int BASE_BEAT;

extern const int SONG_LIST_LEN;
extern Song* SONG_LIST[];

#endif
