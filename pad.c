#include "pch.h"

#include "game.h"
#include "pad.h"

void wait_pad_ready(int port, int slot) {    
    char state_string[16];

    int state = padGetState(port, slot);
    int last_state = -1;

    while((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
        if(state != last_state) {
            padStateInt2String(state, state_string);
            printf("Wait pad(%i,%i) is in state %s\n", port, slot, state_string);
        }
        last_state = state;
        state = padGetState(port, slot);
    }

    if(last_state != -1) {
        printf("Pad OK\n");
    }
}

int init_pad(game_globals_t *game, int port, int slot) {
    wait_pad_ready(port, slot);

    int modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
    printf("Device has %i modes\n", modes);

    if(modes > 0) {
        printf("( ");
        for(int i = 0; i < modes; i++) {
            printf("%i ", padInfoMode(port, slot, PAD_MODETABLE, i));
        }
        printf(")\n");
    }

    printf("Pad is currently using mode %i\n", padInfoMode(port, slot, PAD_MODECURID, 0));

    if(modes == 0) {
        printf("Invalid mode\n");
        return 1;
    }

    //for(int i = 0; (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK); i++) {
    //    if(i >= modes) {
    //        printf("X1 - Not a dualshock controller\n");
    //        return 1;
    //    }
    //}
    //

    int i = 0;
    do {
        if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
            break;
        i++;
    } while (i < modes);
    if (i >= modes) {
        printf("X1 - Not a dualshock controller\n");
        return 1;
    }

    if(padInfoMode(port, slot, PAD_MODECUREXID, 0)) {
        printf("X2 - Not a dualshock controller\n");
        return 1;
    }

    padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

    wait_pad_ready(port, slot);
    printf("press mode %i\n", padInfoPressMode(port, slot));

    wait_pad_ready(port, slot);
    printf("enter mode %i\n", padEnterPressMode(port, slot));

    wait_pad_ready(port, slot);

    game->pad.actuators = padInfoAct(port, slot, -1, 0);
    printf("number of actuators: %i\n", game->pad.actuators);

    if(game->pad.actuators != 0) {
        game->pad.act_align[0] = 0;
        game->pad.act_align[1] = 1;
        game->pad.act_align[2] = 0xff;
        game->pad.act_align[3] = 0xff;
        game->pad.act_align[4] = 0xff;
        game->pad.act_align[5] = 0xff;

        wait_pad_ready(port, slot);

        printf("set act align %i\n", padSetActAlign(port, slot, game->pad.act_align)); 
    } 
    
    wait_pad_ready(port, slot);

    return 1;
}

int pad_init(PAD_INIT_PARAMS) {
    int ret = padPortOpen(port, slot, game->pad.pad_buf);

    if(ret == 0) {
        printf("failed to open pad(%i, %i) %i", port, slot, ret);
        return 1;
    }

    if(!init_pad(game, port, slot)) {
        printf("failed to init pad(%i, %i) %i", port, slot, ret);
        return 1;
    }

    return 0;
}
 
int read_pad(READ_PAD_PARAMS) {
    struct padButtonStatus status;
    u32 new_pad_data = 0;
    u32 new_pad = 0;
    static u32 old_pad = 0;

    wait_pad_ready(port, slot);

    if(padRead(port, slot, &status) != 0) {
        new_pad_data = 0xffff ^ status.btns;

        new_pad = new_pad_data & ~old_pad;
        old_pad = new_pad_data;

        pad_data->new_pad = new_pad;
        pad_data->old_pad = old_pad;

        printf("Left: (%i, %i) Right: (%i, %i)\n", status.ljoy_v, status.ljoy_v, status.rjoy_v, status.rjoy_v);
    }

    // holy shit hes so based <33333 
    #define EFUCKYOURSELF 69420
    return -EFUCKYOURSELF;
}
