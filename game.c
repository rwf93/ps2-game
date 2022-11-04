#include "pch.h"

#include "config.h"
#include "game.h"
#include "render.h"
#include "model.h"

#include "assets/cube.h"
#include "assets/teapot.h"

VECTOR light_direction[4] = {
	{  0.00f,  0.00f,  0.00f, 1.00f },
	{  1.00f,  0.00f, -1.00f, 1.00f },
	{  0.00f,  1.00f, -1.00f, 1.00f },
	{ -1.00f, -1.00f, -1.00f, 1.00f }
};

VECTOR light_colour[4] = {
	{ 0.00f, 0.00f, 0.00f, 1.00f },
	{ 1.00f, 0.00f, 0.00f, 1.00f },
	{ 0.30f, 0.30f, 0.30f, 1.00f },
	{ 0.50f, 0.50f, 0.50f, 1.00f }
};

int light_type[4] = {
	LIGHT_AMBIENT,
	LIGHT_DIRECTIONAL,
	LIGHT_DIRECTIONAL,
	LIGHT_DIRECTIONAL
};

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
	
	// dma shit
	dma_channel_initialize(DMA_CHANNEL_GIF,NULL,0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);

	init_gs(game->frame_buffer, &game->z_buffer);
	init_drawing_environment(game->frame_buffer, &game->z_buffer);
	
	// truly despicable
	for(int i = 0; i < MAX_LIGHTS; i++) {
		for(int j = 0; j < 4; j++) {
			game->lighting.light_position[i][j] = light_direction[i][j];
			game->lighting.light_color[i][j] = light_colour[i][j];
		}
		
		game->lighting.light_type[i] = light_type[i];
	}

	create_view_screen(game->view_screen, graph_aspect_ratio(), -3.00f, 3.00f, -3.00f, 3.00f, 1.00f, 2000.00f);
}

qword_t *render(qword_t *q, game_globals_t *game) {
	static VECTOR pos = {0,0,0,0};
	static VECTOR rot = {0,0,0,0};	
	
	rot[0] += 1.0f * game->delta_time;
	rot[1] += 1.0f * game->delta_time;

	q = draw_model(q, game, get_model(game, "cube"), pos, rot, 0);
	q = draw_model(q, game, get_model(game, "teapot"), pos, rot, 1);

	return q;
}

void load_modules() {
	if(SifLoadModule("rom0:LOADFILE", 0, NULL) < 0) {
		printf("loadfile failed\n");
		SleepThread();
	}
	
	if(SifLoadModule("rom0:SIO2MAN", 0, NULL) < 0) {
		printf("sio failed\n");
		SleepThread();
	}

	if(SifLoadModule("rom0:PADMAN", 0, NULL) < 0) {
		printf("padman failed\n");
		SleepThread();
	}
}

int main(int argc, char *argv[]) {
	//// initalize RPC 
	SifInitRpc(0);
	load_modules();

	padInit(0);

	game_globals_t game;		
	init_gg(&game); // lol

	init_render_context(&game.context);

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
	teapot_model.normals = normals_teapot;

	teapot_model.prim_data.type = PRIM_TRIANGLE;
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

	create_model(&game, "cube", &cube_model);
	create_model(&game, "teapot", &teapot_model);

	for(;;) {
		game.current_time = clock();
		game.delta_time = (game.current_time - game.last_time) / 1000.0f;
		
		// qword and dmatag for rendering
		qword_t *q = 0;
		qword_t *dmatag = 0;

		q = begin_render(q, dmatag, &game, game.frame_buffer, &game.z_buffer);
		
		q = render(q, &game);

		q = end_render(q, dmatag, &game, game.frame_buffer, &game.z_buffer);

		game.last_time = game.current_time;
	}

	end_render_context(&game.context);

	SleepThread();
	
	return 0;
}
