#include "frequencies.h"

#define NOTE_COUNT 12

typedef struct {
    char* name;
    int frequency;
    int period_us;
} NoteData;

const NoteData NOTES[NOTE_COUNT] = {
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
};

static int mod(int a, int base) {
    return ((a % base) + base) % base;
}

NoteData note(int index, int key) {
    int i = index + key;

    NoteData note = NOTES[mod(i, NOTE_COUNT)];

    int octave_diff = i / NOTE_COUNT;
    if(i < 0) {
        octave_diff -= 1;
    }

    // Compute the frequency and period by multiplying/dividing by 2 for each octave
    for(int x = 0; x < octave_diff; x++) {
        note.frequency *= 2;
        note.period_us /= 2;
    }
    for(int x = octave_diff; x < 0; x++) {
        note.frequency /= 2;
        note.period_us *= 2;
    }

    return note;
}

int frequency(int index, int key) {
    return note(index, key).frequency;
}

int period(int index, int key) {
    return note(index, key).period_us;
}
