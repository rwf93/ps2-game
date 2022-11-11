#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct texture_list {
    char *name;
    texbuffer_t *texture;
} texture_list_t;

#define CREATE_TEXTURE_PARAMS game_globals_t *game, char *name, void *data, texbuffer_t *buf
#define GET_TEXTURE_PARAMS game_globals_t *game, char *name
#define SET_TEXTURE_PARAMS qword_t *q, game_globals_t *game, texbuffer_t *buf, clutbuffer_t *clut, lod_t *lod

void create_texture(CREATE_TEXTURE_PARAMS);
texbuffer_t *get_texture(GET_TEXTURE_PARAMS);
qword_t *set_texture(SET_TEXTURE_PARAMS);

#endif