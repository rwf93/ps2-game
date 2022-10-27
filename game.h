#ifndef GAME_H
#define GAME_H

#include <tamtypes.h>
#include <draw.h>
#include <math3d.h>
#include <packet.h>

#define ON_RENDER_PARAMS struct game_globals *game, qword_t *q, qword_t *dmatag, packet_t **packets, packet_t *current_packet 

typedef struct game_globals { 
	unsigned long last_time;
	unsigned long current_time;
	float delta_time;

	VECTOR camera_position;
	VECTOR camera_rotation;

	MATRIX view_screen;
	
	framebuffer_t frame_buffer;
	zbuffer_t z_buffer;

	// shared rendering shit
	VECTOR *shared_verticies;
	VECTOR *shared_normals;
	VECTOR *shared_lights;
	VECTOR *shared_colors;

	xyz_t *xyz;
	color_t *rgbaq;

    void (*on_render)(ON_RENDER_PARAMS);

    // pads
    char pad_buffer[256] __attribute__((aligned(64)));
} game_globals_t;

#define INIT_GG_PARAMS game_globals_t *game
#define FREE_GG_PARAMS game_globals_t *game

void init_gg(INIT_GG_PARAMS);
void free_gg(FREE_GG_PARAMS);

#endif