/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2005 Naomi Peori <naomi@peori.ca>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/

#include <stdio.h>

#include <kernel.h>
#include <stdlib.h>
#include <tamtypes.h>
#include <math3d.h>
#include <memory.h>

#include <dma.h>

#include <sifrpc.h>
#include <loadfile.h>

#include <libpad.h>

#include "game.h"
#include "render.h"

void load_modules() {
    int ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
    if (ret < 0) {
        printf("SifLoadModule sio failed: %d\n", ret);
        SleepThread();
    }

    ret = SifLoadModule("rom0:PADMAN", 0, NULL);
    if (ret < 0) {
        printf("SifLoadModule pad failed: %d\n", ret);
        SleepThread();
    }	
}

void init_gg(INIT_GG_PARAMS) {
	
	game->camera_position[0] = 0.00f;
	game->camera_position[1] = 0.00f;
	game->camera_position[2] = 100.00f;
	game->camera_position[3] = 1.00f;
	
	game->camera_rotation[0] = 0.00f;
	game->camera_rotation[1] = 0.00f;
	game->camera_rotation[2] = 0.00f;
	game->camera_rotation[3] = 1.00f;

	game->last_time = clock();

	// TROLL

	memset(game->view_screen, 0, sizeof(game->view_screen));

	game->shared_verticies = ALIGN_VERT_128(VECTOR);
	game->shared_normals = ALIGN_VERT_128(VECTOR);
	game->shared_lights = ALIGN_VERT_128(VECTOR);
	game->shared_colors = ALIGN_VERT_128(VECTOR);

	game->xyz = ALIGN_VERT_128(xyz_t);
	game->rgbaq = ALIGN_VERT_128(color_t);

	memset(game->pad_buffer, 0, sizeof(char) * 256);
	
	// gs shit
	dma_channel_initialize(DMA_CHANNEL_GIF,NULL,0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);

	init_gs(&game->frame_buffer, &game->z_buffer);
	init_drawing_environment(game, &game->frame_buffer, &game->z_buffer);
}

void free_gg(FREE_GG_PARAMS) {
	free(game->shared_colors);
	free(game->shared_lights);
	free(game->shared_normals);
	free(game->shared_verticies);
}

void on_render(ON_RENDER_PARAMS) {
	static struct padButtonStatus status;
	padRead(0, 0, &status);

	printf("%i\n", status.btns);
}

int main(int argc, char *argv[]) {
	SifInitRpc(0);

	game_globals_t game;
	init_gg(&game); // lol
	
	game.on_render = &on_render; 

	render(&game, &game.frame_buffer,&game.z_buffer);

	free_gg(&game);

	padEnd();

	SleepThread();
	
	return 0;
}
