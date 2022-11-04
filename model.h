#ifndef MODEL_H
#define MODEL_H

#define MDL_LIGHTING (1 << 0)
#define MDL_WIREFRAME (1 << 1)

typedef struct model {
	int point_count;
	int vertex_count;

	int *points;
	VECTOR *vertices;
	VECTOR *colors;
	VECTOR *normals;

	prim_t prim_data;
	color_t color;
} model_t;

typedef struct model_list {
    char *name;
    model_t *model;
} model_list_t;

#define CREATE_MODEL_PARAMS game_globals_t *game, char* name, model_t *model
#define REMOVE_MODEL_PARAMS game_globals_t *game, char *name
#define GET_MODEL_PARAMS REMOVE_MODEL_PARAMS

void create_model(CREATE_MODEL_PARAMS);
void remove_model(REMOVE_MODEL_PARAMS);
model_t *get_model(GET_MODEL_PARAMS);

#endif

