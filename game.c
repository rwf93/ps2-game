#include "pch.h"

#include "config.h"
#include "game.h"
#include "render.h"
#include "model.h"

#include "assets/cube.h"
#include "assets/teapot.h"

#include <sbv_patches.h>
#include <iopcontrol.h>
#include <iopheap.h>

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

void init_gg(INIT_GG_PARAMS) {
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
	dma_channel_initialize(DMA_CHANNEL_GIF,NULL,0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);

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

	static VECTOR pos = {0,0,0,0};
	static VECTOR rot = {0,0,0,0};	
	
	//rot[0] += 1.0f * game->delta_time;
	//rot[1] += 1.0f * game->delta_time;

	q = draw_model(q, game, get_model(game, "cube"), pos, rot, 0);
	
	pos[0] = 40;
	pos[1] = 40;
	pos[2] = 40;
	pos[3] = 40;

	q = draw_model(q, game, get_model(game, "teapot"), pos, rot, MDL_LIGHTING);

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	pos[3] = 0;

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

#define LOAD_ATTRIBUTE(accesor, numComp, dataType, dstPtr) \
    { \
        int n = 0; \
        dataType *buffer = (dataType *)accesor->buffer_view->buffer->data + accesor->buffer_view->offset/sizeof(dataType) + accesor->offset/sizeof(dataType); \
        for (int k = 0; k < accesor->count; k++) \
        {\
            for (int l = 0; l < numComp; l++) \
            {\
                dstPtr[numComp*k + l] = buffer[n + l];\
            }\
            n += (int)(accesor->stride/sizeof(dataType));\
        }\
	}

int main(int argc, char *argv[]) {
	//// initalize RPC 	
	SifInitRpc(0);
	load_modules();

	game_globals_t game;		
	init_gg(&game); // lol

	padInit(0);
	pad_init(&game, 0, 0);

	init_render_context(&game.context);

	cgltf_options options = {0};
	cgltf_data* data = NULL;
	cgltf_parse_file(&options, "CUBE.GLTF", &data);

	int current_model = 0;
	
	model_t cube_model;

	cube_model.point_count = points_count_cube;
	cube_model.vertex_count = vertex_count_cube;


	for(int i = 0; i < data->meshes[current_model].primitives_count; i++) {
		cgltf_primitive prim = data->meshes[current_model].primitives[i];
		cgltf_size num_vertex = prim.attributes[0].data->count;

		if(prim.indices != NULL) {
			cgltf_accessor *attribute = data->meshes[0].primitives[i].indices;

			unsigned int *points = malloc(attribute->count * sizeof(unsigned int));

			for(cgltf_size index = 0; index < attribute->count; index++) {
				unsigned int point = 0;
				cgltf_accessor_read_uint(attribute, index, &point, 4);
				printf("%u", point);
			}

			free(points);

		}
	}

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
	teapot_model.color.a = 0x50;
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
