#ifndef PAD_H
#define PAD_H

typedef struct pad_man {
    char pad_buf[256] __attribute__((aligned(64)));
    
    char act_align[6];
    int actuators;
} pad_man_t;

#define PAD_INIT_PARAMS game_globals_t *game, int port, int slot
#define READ_PAD_PARAMS game_globals_t *game, int port, int slot

int pad_init(PAD_INIT_PARAMS);
int read_pad(READ_PAD_PARAMS);

#endif