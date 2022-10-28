#ifndef RENDER_H
#define RENDER_H

#include <time.h>
#include <malloc.h>
#include <draw.h>
#include <packet.h>

#define MAX_VERTICES 65535
#define ALIGH_VERT(alignment, type) memalign(alignment, sizeof(type) * MAX_VERTICES);
#define ALIGN_VERT_128(type) ALIGH_VERT(128, type);

typedef struct model {
	int point_count;
	int vertex_count;

	int *points;
	VECTOR *vertices;
	VECTOR *colors;

	prim_t prim_data;
	color_t color;
} model_t;

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


#define INIT_DRAWING_ENVIRONNMENT_PARAMS framebuffer_t *frame, zbuffer_t *z
#define INIT_GS_PARAMS framebuffer_t *frame, zbuffer_t *z
#define DRAW_MODEL_PARAMS game_globals_t *game, model_t *model, qword_t *q, VECTOR position, VECTOR rotation
//#define RENDER_PARAMS game_globals_t *game, framebuffer_t *frame, zbuffer_t *z

#define INIT_RENDER_CONTEXT render_context_t *context
#define END_RENDER_CONTEXT INIT_RENDER_CONTEXT // basic for now...

#define BEGIN_RENDER_PARAMS game_globals_t *game, framebuffer_t *frame, zbuffer_t *z
#define END_RENDER_PARAMS qword_t *q, BEGIN_RENDER_PARAMS

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