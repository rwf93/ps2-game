#ifndef RENDER_H
#define RENDER_H

#include <time.h>
#include <malloc.h>
#include <draw.h>
#include <packet.h>

#include "model.h"
#include "config.h"

typedef struct render_context {
	int context;

	packet_t *packets[2];
	packet_t *current;
	packet_t *flip;

	// shared rendering shit
	VECTOR *shared_verticies;
	VECTOR *shared_normals;
	VECTOR *shared_lights;
	VECTOR *shared_colors;

	xyz_t *xyz;
	color_t *rgbaq;
} render_context_t;

typedef struct world_lighting { 
	VECTOR light_position[MAX_LIGHTS];
	VECTOR light_color[MAX_LIGHTS];
	int light_type[MAX_LIGHTS];
} world_lighting_t;

typedef struct camera {
	VECTOR camera_position;
	VECTOR camera_rotation;

	MATRIX view_screen;

	float left;
	float right;
	float top;
	float bottom;

	float zfar;
	float znear;
} camera_t;

#define INIT_DRAWING_ENVIRONNMENT_PARAMS framebuffer_t *frame, zbuffer_t *z
#define INIT_GS_PARAMS framebuffer_t *frame, zbuffer_t *z
#define DRAW_MODEL_PARAMS qword_t *q, game_globals_t *game, model_t *model, VECTOR position, VECTOR rotation, int flags
//#define RENDER_PARAMS game_globals_t *game, framebuffer_t *frame, zbuffer_t *z

#define INIT_RENDER_CONTEXT render_context_t *context
#define END_RENDER_CONTEXT INIT_RENDER_CONTEXT // basic for now...

#define BEGIN_RENDER_PARAMS qword_t *q, qword_t *dmatag, game_globals_t *game, framebuffer_t *frame, zbuffer_t *z
#define END_RENDER_PARAMS BEGIN_RENDER_PARAMS

void init_gs(INIT_GS_PARAMS);
void init_drawing_environment(INIT_DRAWING_ENVIRONNMENT_PARAMS);
// draw a model to the current context
qword_t *draw_model(DRAW_MODEL_PARAMS); 

void init_render_context(INIT_RENDER_CONTEXT);
void end_render_context(END_RENDER_CONTEXT);

qword_t *begin_render(BEGIN_RENDER_PARAMS);
qword_t *end_render(END_RENDER_PARAMS);

//int render(RENDER_PARAMS);

#endif