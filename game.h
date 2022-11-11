#ifndef GAME_H
#define GAME_H

typedef struct game_globals game_globals_t;
typedef struct model model_t;
typedef struct model_list model_list_t;
typedef struct pad_man pad_man_t;
typedef struct world_lighting world_lighting_t;
typedef struct camera camera_t;
typedef struct texture_list texture_list_t;

#include "render.h"
#include "model.h"
#include "pad.h"
#include "texture.h"

#include "config.h"

typedef struct game_globals { 
	unsigned long last_time;
	unsigned long current_time;
	float delta_time;
	
	framebuffer_t frame_buffer[2];
	zbuffer_t z_buffer;
	
	camera_t camera;

	world_lighting_t lighting;
	render_context_t context;
	
	int model_list_index;
	model_list_t models[MAX_MODELS];

	int texture_list_index;
	texture_list_t textures[MAX_MODELS*2];

	pad_man_t pad[MAX_PADS];
} game_globals_t;

#define INIT_GG_PARAMS game_globals_t *game

void init_gg(INIT_GG_PARAMS);


#endif