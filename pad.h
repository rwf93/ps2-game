#ifndef PAD_H
#define PAD_H

#include <tamtypes.h>
#include <libpad.h>

typedef struct pad {
    u32 new;
    u32 old;

    struct padButtonStatus button_status;
} pad_t;

#define PAD_INIT_PARAMS game_globals_t *game
#define READ_PAD_PARAMS game_globals_t *game, int pad_id, int slot, pad_t *pad
#define PAD_END_PARAMS game_globals_t *game

int pad_init(PAD_INIT_PARAMS);
int read_pad(READ_PAD_PARAMS);
void pad_end(PAD_END_PARAMS);

#endif