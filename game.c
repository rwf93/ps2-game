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

#include <packet.h>

#include "game.h"
#include "render.h"

#include "cube.h"
#include "teapot.h"

#include <graph.h>

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
	
	// gs shit
	dma_channel_initialize(DMA_CHANNEL_GIF,NULL,0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);

	init_gs(&game->frame_buffer, &game->z_buffer);
	init_drawing_environment(&game->frame_buffer, &game->z_buffer);
	
	create_view_screen(game->view_screen, graph_aspect_ratio(), -3.00f, 3.00f, -3.00f, 3.00f, 1.00f, 2000.00f);
}

void end_gg(FREE_GG_PARAMS) {
	packet_free(game->context->flip);
	packet_free(game->context->packets[0]);
	packet_free(game->context->packets[1]);

	free(game->context->shared_colors);
	free(game->context->shared_lights);
	free(game->context->shared_normals);
	free(game->context->shared_verticies);
}

int main(int argc, char *argv[]) {
	game_globals_t game;		
	init_gg(&game); // lol

	render_context_t context;
	init_render_context(&context);
	game.context = &context;

	model_t cube_model;

	cube_model.point_count = points_count_cube;
	cube_model.vertex_count = vertex_count_cube;

	cube_model.points = points_cube;
	cube_model.vertices = vertices_cube;
	cube_model.colors = colours_cube;

	cube_model.prim_data.type = PRIM_TRIANGLE;
	cube_model.prim_data.shading = PRIM_SHADE_GOURAUD;
	cube_model.prim_data.mapping = DRAW_DISABLE;
	cube_model.prim_data.fogging = DRAW_DISABLE;
	cube_model.prim_data.blending = DRAW_DISABLE;
	cube_model.prim_data.antialiasing = DRAW_ENABLE;
	cube_model.prim_data.mapping_type = PRIM_MAP_ST;
	cube_model.prim_data.colorfix = PRIM_UNFIXED;

	cube_model.color.r = 0x80;
	cube_model.color.g = 0x80;
	cube_model.color.b = 0x80;
	cube_model.color.a = 0x80;
	cube_model.color.q = 1.0f;

	model_t teapot_model;

	teapot_model.point_count = points_count_teapot;
	teapot_model.vertex_count = vertex_count_teapot;

	teapot_model.points = points_teapot;
	teapot_model.vertices = vertices_teapot;
	teapot_model.colors = colours_teapot;

	teapot_model.prim_data.type = PRIM_LINE_STRIP;
	teapot_model.prim_data.shading = PRIM_SHADE_GOURAUD;
	teapot_model.prim_data.mapping = DRAW_DISABLE;
	teapot_model.prim_data.fogging = DRAW_DISABLE;
	teapot_model.prim_data.blending = DRAW_DISABLE;
	teapot_model.prim_data.antialiasing = DRAW_ENABLE;
	teapot_model.prim_data.mapping_type = PRIM_MAP_ST;
	teapot_model.prim_data.colorfix = PRIM_UNFIXED;

	teapot_model.color.r = 0x0;
	teapot_model.color.g = 0x80;
	teapot_model.color.b = 0x80;
	teapot_model.color.a = 0x80;
	teapot_model.color.q = 1.0f;

	VECTOR pos = {0,0,0,0};
	VECTOR rot = {0,0,0,0};

	VECTOR pos2 = {15,15,15,0};

	
	VECTOR postest = {0,0,0,0};
	VECTOR rottest = {0,0,0,0};

	VECTOR pos2test = {15,15,15,0};

	for(;;) {
		qword_t *q;

		q = begin_render(&game, &game.frame_buffer, &game.z_buffer);
		q = draw_model(&game, &cube_model, q, pos, rot);
		q = end_render(q, &game, &game.frame_buffer, &game.z_buffer);

		//game->current_time = clock();
		//game->delta_time = (game->current_time - game->last_time) / 1000.0f;
//
		//qword_t *q;
		//// really scuffed render code
//
		//q = begin_render(&game, &game.frame_buffer, &game.z_buffer);
//
		//q = draw_model(&game, &cube_model, q, pos, rot);
//
		//q = draw_model(&game, &teapot_model, q, pos2, rot);
//
		//q = end_render(q, &game, &game.frame_buffer, &game.z_buffer);
	}

	end_render_context(&context);
	end_gg(&game);

	SleepThread();
	
	return 0;
}
