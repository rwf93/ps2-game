#include "pch.h"

#include "game.h"
#include "render.h"
#include "model.h"
#include "texture.h"

void alloc_texbuf(texbuffer_t *texbuf) {
    texbuf->width = FB_HEIGHT / 2;
    texbuf->psm = GS_PSM_24;
    texbuf->address = graph_vram_allocate(FB_HEIGHT / 2,FB_HEIGHT / 2,GS_PSM_24,GRAPH_ALIGN_BLOCK);
}

void load_texbuf(texbuffer_t *texbuf, char *data) {
    packet_t *packet = packet_init(PACKET_SIZE,PACKET_NORMAL);

	qword_t *q;

	q = packet->data;

	q = draw_texture_transfer(q,data,FB_HEIGHT/2,FB_HEIGHT/2,GS_PSM_24,texbuf->address,texbuf->width);
	q = draw_texture_flush(q);

	dma_channel_send_chain(DMA_CHANNEL_GIF,packet->data, q - packet->data, 0,0);
	dma_wait_fast();

	packet_free(packet);
}

qword_t *set_texture(SET_TEXTURE_PARAMS) {
	buf->info.width = draw_log2(FB_HEIGHT/2);
	buf->info.height = draw_log2(FB_HEIGHT/2);
	buf->info.components = TEXTURE_COMPONENTS_RGB;
	buf->info.function = TEXTURE_FUNCTION_DECAL;

    qword_t *dmatag = q;
	q++;

	q = draw_texture_sampling(q,0,lod);
	q = draw_texturebuffer(q,0,buf,clut);

    DMATAG_CNT(dmatag,q-dmatag-1,0,0,0);

    return q;
}

void create_texture(CREATE_TEXTURE_PARAMS) {
    alloc_texbuf(buf);
    load_texbuf(buf, data);

    texture_list_t t_list; 
    t_list.name = name;
    t_list.texture = buf;

    game->textures[game->texture_list_index++] = t_list;
}

texbuffer_t *get_texture(GET_TEXTURE_PARAMS) {
     for(int i = 0; i < MAX_MODELS*2; i++) {
        if(strcmp(game->textures[i].name, name) == 0) {
            return game->textures[i].texture;
        }
    }

    return NULL;
}