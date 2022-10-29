#include "pch.h"

#include "game.h"
#include "model.h"
#include "render.h"

void create_model(CREATE_MODEL_PARAMS) {
    model_list_t m_list; 
    m_list.name = name;
    m_list.model = model;

    game->models[game->model_list_index] = m_list;
    game->model_list_index++;
}

void remove_model(REMOVE_MODEL_PARAMS) {
    
}  

model_t *get_model(GET_MODEL_PARAMS) { 
    for(int i = 0; i < MAX_MODELS; i++) {
        if(strcmp(game->models[i].name, name) == 0) {
            return game->models[i].model;
        }
    }

    return NULL;
}