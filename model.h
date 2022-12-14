#ifndef MODEL_H
#define MODEL_H

#define MDL_LIGHTING (1 << 0)
#define MDL_WIREFRAME (1 << 1)
#define MDL_TEXTURED (1 << 2)

typedef struct model {
	int point_count;
	int vertex_count;

	int *points;
	VECTOR *vertices;
	VECTOR *colors;
	VECTOR *normals;
	VECTOR *uv_coords;

	prim_t prim_data;
	color_t color;
} model_t;

typedef struct model_list {
    char *name;
    model_t *model;
} model_list_t;

#define CREATE_MODEL_PARAMS game_globals_t *game, char* name, model_t *model
#define GET_MODEL_PARAMS game_globals_t *game, char *name

void create_model(CREATE_MODEL_PARAMS);
model_t *get_model(GET_MODEL_PARAMS);

#endif

