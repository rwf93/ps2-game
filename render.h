#ifndef RENDER_H
#define RENDER_H

#include <time.h>
#include <malloc.h>
#include <draw.h>

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

#define INIT_DRAWING_ENVIRONNMENT_PARAMS game_globals_t *game, framebuffer_t *frame, zbuffer_t *z
#define INIT_GS_PARAMS framebuffer_t *frame, zbuffer_t *z
#define DRAW_MODEL_PARAMS game_globals_t *game, model_t *model, qword_t *q, VECTOR position, VECTOR rotation
#define RENDER_PARAMS game_globals_t *game, framebuffer_t *frame, zbuffer_t *z

void init_gs(INIT_GS_PARAMS);
void init_drawing_environment(INIT_DRAWING_ENVIRONNMENT_PARAMS);
qword_t *draw_model(DRAW_MODEL_PARAMS);
int render(RENDER_PARAMS);

#endif