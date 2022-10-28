#ifndef GAME_H
#define GAME_H

#include <tamtypes.h>
#include <draw.h>
#include <math3d.h>
#include <packet.h>

// stupid fucking hack, i hate this lang
typedef struct render_context render_context_t; 

typedef struct game_globals { 
	unsigned long last_time;
	unsigned long current_time;
	float delta_time;

	VECTOR camera_position;
	VECTOR camera_rotation;

	
	framebuffer_t frame_buffer;
	zbuffer_t z_buffer;

	MATRIX view_screen;

	render_context_t *context;
} game_globals_t;

#define INIT_GG_PARAMS game_globals_t *game
#define FREE_GG_PARAMS game_globals_t *game

void init_gg(INIT_GG_PARAMS);
void end_gg(FREE_GG_PARAMS);

#endif