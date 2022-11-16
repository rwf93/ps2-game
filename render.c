#include "pch.h"

#include "config.h"
#include "game.h"
#include "render.h"

void init_gs(INIT_GS_PARAMS)
{
	// defines our framebuffers
	frame->width = FB_WIDTH;
	frame->height = FB_HEIGHT;
	frame->mask = 0;
	frame->psm = GS_PSM_32;
	frame->address = graph_vram_allocate(frame->width, frame->height, frame->psm, GRAPH_ALIGN_PAGE);
	
	frame++;

	frame->width = FB_WIDTH;
	frame->height = FB_HEIGHT;
	frame->mask = 0;
	frame->psm = GS_PSM_32;
	frame->address = graph_vram_allocate(frame->width, frame->height, frame->psm, GRAPH_ALIGN_PAGE);
	
	// Enable the zbuffer.
	z->enable = DRAW_ENABLE;
	z->mask = 0;
	z->method = ZTEST_METHOD_GREATER_EQUAL;
	z->zsm = GS_ZBUF_32;
	z->address = graph_vram_allocate(frame->width, frame->height,z->zsm, GRAPH_ALIGN_PAGE);

	// Initialize the screen and tie the first framebuffer to the read circuits.
	graph_initialize(frame->address, frame->width, frame->height, frame->psm,0,0);
}

void init_drawing_environment(INIT_DRAWING_ENVIRONNMENT_PARAMS)
{
	packet2_t *packet = packet2_create(20, P2_TYPE_NORMAL, P2_MODE_NORMAL, 0);

	packet2_update(packet, draw_setup_environment(packet->next, 0, frame, z));
	packet2_update(packet, draw_primitive_xyoffset(packet->next, 0, 2048 - (FB_WIDTH / 2),2048 - (FB_HEIGHT / 2)));
	packet2_update(packet, draw_finish(packet->next));

	dma_channel_send_packet2(packet, DMA_CHANNEL_GIF, 1);
	dma_wait_fast();

	packet2_free(packet);
	/*
	packet_t *packet = packet_init(20,PACKET_NORMAL);
	qword_t *q = packet->data;

	q = draw_setup_environment(q,0,frame,z);
	q = draw_primitive_xyoffset(q,0,(2048-(FB_WIDTH/2)),(2048-(FB_HEIGHT/2)));
	q = draw_finish(q);

	dma_channel_send_normal(DMA_CHANNEL_GIF,packet->data,q - packet->data, 0, 0);
	dma_wait_fast();

	packet_free(packet);
	*/
}

void draw_model(DRAW_MODEL_PARAMS) {
	/*
	packet2_add_float(game->context.shared_packet, 2048.0f);
	packet2_add_float(game->context.shared_packet, 2048.0f);
	packet2_add_float(game->context.shared_packet, ((float)0xFFFFFF) / 32.0F);
	*/

	for(int i = 0; i < model->point_count; i++) {
		for(int j = 0; j < 4; j++) {
			game->context.shared_verticies[i][j] = model->vertices[model->points[i]][j];
			game->context.shared_coordinates[i][j] = model->uv_coords[model->points[i]][j];
		}
	}

	MATRIX local_world = {0};
	MATRIX local_screen = {0};
	MATRIX world_view = {0};
		
	create_local_world(local_world, position, rotation);
	create_world_view(world_view, game->camera.camera_position, game->camera.camera_rotation);
	create_local_screen(local_screen, local_world, world_view, game->camera.view_screen);

	game->context.current = game->context.packets[game->context.context];
	packet2_reset(game->context.current, 0);

	packet2_utils_vu_add_unpack_data(game->context.current, 0, &local_screen, 8, 0);

	u32 vif_added_bytes = 0;
	packet2_utils_vu_add_unpack_data(game->context.current, vif_added_bytes, game->context.shared_packet->base, packet2_get_qw_count(game->context.shared_packet), 1);
	vif_added_bytes += packet2_get_qw_count(game->context.shared_packet);

	packet2_utils_vu_add_unpack_data(game->context.current, vif_added_bytes, game->context.shared_verticies, model->point_count, 1);
	vif_added_bytes += model->point_count;

	packet2_utils_vu_add_unpack_data(game->context.current, vif_added_bytes, game->context.shared_coordinates, model->point_count, 1);
	vif_added_bytes += model->point_count;

	packet2_utils_vu_add_start_program(game->context.current, game->context.vu_programs[0]);
	packet2_utils_vu_add_end_tag(game->context.current);
	dma_channel_wait(DMA_CHANNEL_VIF1, 0);
	dma_channel_send_packet2(game->context.current, DMA_CHANNEL_VIF1, 1);
}

extern u32 render_pipeline_normal_CodeStart __attribute__((section(".vudata")));
extern u32 render_pipeline_normal_CodeEnd __attribute__((section(".vudata")));

extern u32 render_pipeline_test_CodeStart __attribute__((section(".vudata")));
extern u32 render_pipeline_test_CodeEnd __attribute__((section(".vudata")));

u32 add_program(u32 *begin, u32 *end, u32 addr) {
	u32 count = (end - begin) / 2;
	if(count & 1) count++;
	
	u32 packet_size = packet2_utils_get_packet_size_for_program(begin, end)+1;
	packet2_t *packet = packet2_create(packet_size, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	packet2_vif_add_micro_program(packet, addr, begin, end);
	packet2_utils_vu_add_end_tag(packet);
	dma_channel_send_packet2(packet, DMA_CHANNEL_VIF1, 1);
	dma_channel_wait(DMA_CHANNEL_VIF1, 0);
	packet2_free(packet);

	return count;
}

void upload_microcode(render_context_t *context) {
	memset(context->vu_programs, 0, sizeof(int) * 4);
	
	int program = 0;
	int program_size = 0;
	// start
	program_size = add_program(&render_pipeline_normal_CodeStart, &render_pipeline_normal_CodeEnd, program);
	context->vu_programs[0] = program;

	// this one is intentionally fucked for obvious testing purposes
	program += program_size;	
	program_size = add_program(&render_pipeline_test_CodeStart, &render_pipeline_test_CodeEnd, program);
	context->vu_programs[1] = program;
}

void enable_doublebuffer() {
	packet2_t *packet = packet2_create(1, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	packet2_utils_vu_add_double_buffer(packet, 8, 496);
	packet2_utils_vu_add_end_tag(packet);
	dma_channel_send_packet2(packet, DMA_CHANNEL_VIF1, 1);
	dma_channel_wait(DMA_CHANNEL_VIF1, 0);
	packet2_free(packet);
}

void init_render_context(INIT_RENDER_CONTEXT) {
	context->packets[0] = packet2_create(11, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	context->packets[1] = packet2_create(11, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	context->shared_packet = packet2_create(10, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	context->flip = packet2_create(4, P2_TYPE_UNCACHED_ACCL, P2_MODE_NORMAL, 0);

	upload_microcode(context);
	enable_doublebuffer();

	context->context = 0;

	context->shared_verticies = ALIGN_VERT_128(VECTOR);
	context->shared_coordinates = ALIGN_VERT_128(VECTOR);
	//context->shared_normals = ALIGN_VERT_128(VECTOR);
	//context->shared_lights = ALIGN_VERT_128(VECTOR);
	//context->shared_colors = ALIGN_VERT_128(VECTOR);
}

void begin_render(BEGIN_RENDER_PARAMS) {
	packet2_t *packet = packet2_create(36, P2_TYPE_NORMAL, P2_MODE_NORMAL, 0);

	packet2_update(packet, draw_disable_tests(packet->next, 0, z));
	packet2_update(packet, draw_clear(packet->next, 0, 2048.0f - (FB_WIDTH/2), 2048.0f - (FB_HEIGHT / 2), frame->width, frame->height, 0x40, 0x40, 0x40));
	packet2_update(packet, draw_enable_tests(packet->next, 0, z));
	packet2_update(packet, draw_finish(packet->next));

	dma_wait_fast();
	dma_channel_send_packet2(packet, DMA_CHANNEL_GIF, 1);

	packet2_free(packet);

	draw_wait_finish();
}

void flip_buffers(game_globals_t *game, framebuffer_t *frame) {	
	graph_set_framebuffer_filtered(frame[game->context.context].address,frame[game->context.context].width,frame[game->context.context].psm,0,0);
	
	game->context.context ^= game->context.context;

	packet2_update(game->context.flip, draw_framebuffer(game->context.flip->base, 0, &frame[game->context.context]));

	packet2_update(game->context.flip, draw_finish(game->context.flip->next));
	dma_channel_wait(DMA_CHANNEL_GIF, 0);
	dma_channel_send_packet2(game->context.flip, DMA_CHANNEL_GIF, 1);
	draw_wait_finish();
}

void end_render(END_RENDER_PARAMS) {
	graph_wait_vsync(); // le veesync
	flip_buffers(game, frame);
}
