#include "pch.h"

#include "config.h"
#include "game.h"
#include "render.h"
#include "model.h"

#include "assets/cube.h"
#include "assets/teapot.h"

#include "assets/texture.raw.h"
#include "assets/doctor.raw.h"
#include "assets/angry.raw.h"

#include <vec.h>

void init_gg(INIT_GG_PARAMS) {
	memset(game, 0, sizeof(game_globals_t)); // im extremely fucking lazy...

	VECTOR light_direction[4] = {
		{  0.00f,  0.00f,  0.00f, 1.00f },
		{  1.00f,  0.00f, -1.00f, 1.00f },
		{  0.00f,  1.00f, -1.00f, 1.00f },
		{ -1.00f, -1.00f, -1.00f, 1.00f }
	};

	VECTOR light_colour[4] = {
		{ 0.00f, 0.00f, 0.00f, 1.00f },
		{ 0.00f, 0.00f, 0.00f, 1.00f },
		{ 0.30f, 0.30f, 0.30f, 1.00f },
		{ 0.50f, 0.50f, 0.50f, 1.00f }
	};

	int light_type[4] = {
		LIGHT_AMBIENT,
		LIGHT_DIRECTIONAL,
		LIGHT_DIRECTIONAL,
		LIGHT_DIRECTIONAL
	};

	memcpy(game->lighting.light_position, light_direction, sizeof(VECTOR) * MAX_LIGHTS);
	memcpy(game->lighting.light_color, light_colour, sizeof(VECTOR) * MAX_LIGHTS);
	memcpy(game->lighting.light_type, light_type, sizeof(int) * MAX_LIGHTS);

	game->last_time = clock();

	// dma shit
	dma_channel_initialize(DMA_CHANNEL_GIF, NULL, 0);
	dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);
	dma_channel_fast_waits(DMA_CHANNEL_VIF1);

	init_render_context(&game->context);
	init_gs(game->frame_buffer, &game->z_buffer);
	init_drawing_environment(game->frame_buffer, &game->z_buffer);

	camera_t camera;
	
	camera.left = -3.0f;
	camera.right = 3.0f;
	camera.bottom = -3.0f;
	camera.top = 3.0f;

	camera.znear = 1.0f;
	camera.zfar = 2000.0f;

	VECTOR camera_position = { 0.00f, 0.00f, 100.00f, 1.00f };
	VECTOR camera_rotation = { 0.00f, 0.00f,   0.00f, 1.00f };
	
	memcpy(camera.camera_position, camera_position, sizeof(VECTOR));
	memcpy(camera.camera_rotation, camera_rotation, sizeof(VECTOR));

	create_view_screen(camera.view_screen, graph_aspect_ratio(), camera.left, camera.right, camera.bottom, camera.top, camera.znear, camera.zfar);

	game->camera = camera;
	
}

qword_t *render(qword_t *q, game_globals_t *game) {
	pad_data_t pad;
	read_pad(game, 0, 0, &pad);

	if(pad.button_status.rjoy_h > 240) {
		game->camera.camera_rotation[1] -= 1 * game->delta_time;
	}

	if(pad.button_status.rjoy_v > 240) {
		game->camera.camera_rotation[0] -= 1 * game->delta_time;
	}

	if(pad.button_status.rjoy_h < 90) {
		game->camera.camera_rotation[1] += 1 * game->delta_time;
	}

	if(pad.button_status.rjoy_v < 90) {
		game->camera.camera_rotation[0] += 1 * game->delta_time;
	}

	//static VECTOR pos = {0,0,0,0};
	//static VECTOR rot = {0,0,0,0};	
	//
	//rot[0] += 1.0f * game->delta_time;
	//rot[1] += 1.0f * game->delta_time;
	//
	////q = set_texture(q, game, get_texture(game, "doctor"), &clut, &lod);
	////q = draw_model(q, game, get_model(game, "cube"), pos, rot, MDL_TEXTURED);
	////
	////pos[0] = 40;
	////pos[1] = 40;
	////pos[2] = 40;
	////pos[3] = 40;
	////
	////q = set_texture(q, game, get_texture(game, "flower"), &clut, &lod);
	////q = draw_model(q, game, get_model(game, "cube"), pos, rot, MDL_TEXTURED);
//
	//pos[0] = 0;
	//pos[1] = 0;
	//pos[2] = 0;
	//pos[3] = 0;

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

void reset_iop() {
	SifExitIopHeap();
    SifLoadFileExit();
    SifExitRpc();

    SifInitRpc(0);

    while (!SifIopReset("", 0)) {};
    while (!SifIopSync()) {};

    SifInitRpc(0);
    SifLoadFileInit();
    SifInitIopHeap();
    sbv_patch_enable_lmb();
    sbv_patch_disable_prefix_check();
}

int main(int argc, char *argv[]) {	
	SifInitRpc(0);
	load_modules();
	//reset_iop();

	game_globals_t game;		
	init_gg(&game); // lol
	
	//create_texture(&game, "flower", assets_texture_raw, &buf);
	//create_texture(&game, "doctor", assets_doctor_raw, &buf2); 

	padInit(0);
	pad_init(&game, 0, 0);


	model_t cube_model = {0};
	
	cube_model.point_count = points_count_cube;
	cube_model.vertex_count = vertex_count_cube;

	cube_model.points = points_cube;
	cube_model.vertices = vertices_cube;
	cube_model.colors = colours_cube;
	cube_model.uv_coords = coordinates_cube;

	cube_model.prim_data.type = PRIM_TRIANGLE;
	cube_model.prim_data.shading = PRIM_SHADE_GOURAUD;
	cube_model.prim_data.mapping = DRAW_ENABLE;
	cube_model.prim_data.fogging = DRAW_DISABLE;
	cube_model.prim_data.blending = DRAW_ENABLE;
	cube_model.prim_data.antialiasing = DRAW_DISABLE;
	cube_model.prim_data.mapping_type = PRIM_MAP_ST;
	cube_model.prim_data.colorfix = PRIM_UNFIXED;
	
	cube_model.color.r = 0x80;
	cube_model.color.g = 0x80;
	cube_model.color.b = 0x80;
	cube_model.color.a = 0x80;
	cube_model.color.q = 1.0f;

	model_t teapot_model = {0};

	teapot_model.point_count = points_count_teapot;
	teapot_model.vertex_count = vertex_count_cube;

	VECTOR useless_coords[9408] = {0};

	teapot_model.points = points_teapot;
	teapot_model.vertices = vertices_teapot;
	teapot_model.colors = colours_teapot;
	teapot_model.uv_coords = useless_coords;

	teapot_model.prim_data.type = PRIM_TRIANGLE;
	teapot_model.prim_data.shading = PRIM_SHADE_GOURAUD;
	teapot_model.prim_data.mapping = DRAW_ENABLE;
	teapot_model.prim_data.fogging = DRAW_DISABLE;
	teapot_model.prim_data.blending = DRAW_ENABLE;
	teapot_model.prim_data.antialiasing = DRAW_DISABLE;
	teapot_model.prim_data.mapping_type = PRIM_MAP_ST;
	teapot_model.prim_data.colorfix = PRIM_UNFIXED;

	teapot_model.color.r = 0x80;
	teapot_model.color.g = 0x80;
	teapot_model.color.b = 0x80;
	teapot_model.color.a = 0x80;
	teapot_model.color.q = 1.0f;

	clutbuffer_t clut = {0};
	lod_t lod = {0};

	lod.calculation = LOD_USE_K;
	lod.max_level = 0;
	lod.mag_filter = LOD_MAG_NEAREST;
	lod.min_filter = LOD_MIN_NEAREST;
	lod.l = 0;
	lod.k = 0;

	clut.storage_mode = CLUT_STORAGE_MODE1;
	clut.start = 0;
	clut.psm = 0;
	clut.load_method = CLUT_NO_LOAD;
	clut.address = 0;

	texbuffer_t t = {0};
	t.width = 128;
	t.psm = GS_PSM_24;
	t.address = graph_vram_allocate(128, 128, GS_PSM_24, GRAPH_ALIGN_BLOCK);

	t.info.width = draw_log2(128);
	t.info.height = draw_log2(128);
	t.info.components = TEXTURE_COMPONENTS_RGB;
	t.info.function = TEXTURE_FUNCTION_DECAL;

	packet2_t *packet2 = packet2_create(50, P2_TYPE_NORMAL, P2_MODE_CHAIN, 0);
	packet2_update(packet2, draw_texture_transfer(packet2->next, assets_angry_raw, 128, 128, GS_PSM_24, t.address, t.width));
	packet2_update(packet2, draw_texture_flush(packet2->next));
	dma_channel_send_packet2(packet2, DMA_CHANNEL_GIF, 1);
	dma_wait_fast();
	packet2_free(packet2);

	for(;;) {
		game.current_time = clock();
		game.delta_time = (game.current_time - game.last_time) / 1000.0f;

		begin_render(&game, game.frame_buffer, &game.z_buffer);

		static VECTOR pos = {0};
		static VECTOR rot = {0};
		
		rot[0] += 1.0f * game.delta_time;
		rot[1] += 1.0f * game.delta_time;

		draw_model(&game, &cube_model, &t, &clut, &lod, pos, rot, 0);

		end_render(&game, game.frame_buffer, &game.z_buffer);

		game.last_time = game.current_time;
	}

	SleepThread();
	
	return 0;
}
