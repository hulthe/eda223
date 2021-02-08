#include "frequencies.h"

#define NOTE_COUNT 25

const int BASE_NOTE_INDEX = 10;

typedef struct {
    char* name;
    int frequency;
    int period_us;    
} Note;

const Note NOTES[NOTE_COUNT] = {
    { "B",  246, 2024, },
    { "C",  261, 1911, },
    { "C#", 277, 1803, },
    { "D",  293, 1702, },
    { "D#", 311, 1607, },
    { "E",  329, 1516, },
    { "F",  349, 1431, },
    { "F#", 369, 1351, },
    { "G",  391, 1275, },
    { "G#", 415, 1203, },
    { "A",  440, 1136, }, // Base note A (440Hz)
    { "A#", 466, 1072, },
    { "B",  493, 1012, },
    { "C",  523,  955, },
    { "C#", 554,  901, },
    { "D",  587,  851, },
    { "D#", 622,  803, },
    { "E",  659,  758, },
    { "F",  698,  715, },
    { "F#", 739,  675, },
    { "G",  783,  637, },
    { "G#", 830,  601, },
    { "A",  880,  568, },
    { "A#", 932,  536, },
    { "B",  987,  506, },
};

Note note(int index, int key) {
    int actual_i = index + key + BASE_NOTE_INDEX;

    if(actual_i < 0 || actual_i >= NOTE_COUNT) {
        Note empty = {0, 0};
        return empty;
    }

    return NOTES[actual_i];
}

int frequency(int index, int key) {
    return note(index, key).frequency;
}

int period(int index, int key) {
    return note(index, key).period_us;
}