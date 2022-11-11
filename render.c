#include "pch.h"

#include "config.h"
#include "game.h"
#include "render.h"

/*
	model_t cube_model;

	cube_model.point_count = points_count_cube;
	cube_model.vertex_count = vertex_count_cube;

	cube_model.points = points_cube;
	cube_model.vertices = vertices_cube;
	cube_model.colors = colours_cube;

	cube_model.prim_data.type = PRIM_TRIANGLE;
	cube_model.prim_data.shading = PRIM_SHADE_GOURAUD;
	cube_model.prim_data.mapping = DRAW_DISABLE;
	cube_model.prim_data.fogging = DRAW_DISABLE;
	cube_model.prim_data.blending = DRAW_DISABLE;
	cube_model.prim_data.antialiasing = DRAW_ENABLE;
	cube_model.prim_data.mapping_type = PRIM_MAP_ST;
	cube_model.prim_data.colorfix = PRIM_UNFIXED;

	cube_model.color.r = 0x80;
	cube_model.color.g = 0x80;
	cube_model.color.b = 0x80;
	cube_model.color.a = 0x80;
	cube_model.color.q = 1.0f;

	model_t teapot_model;

	teapot_model.point_count = points_count_teapot;
	teapot_model.vertex_count = vertex_count_teapot;

	teapot_model.points = points_teapot;
	teapot_model.vertices = vertices_teapot;
	teapot_model.colors = colours_teapot;

	teapot_model.prim_data.type = PRIM_LINE_STRIP;
	teapot_model.prim_data.shading = PRIM_SHADE_GOURAUD;
	teapot_model.prim_data.mapping = DRAW_DISABLE;
	teapot_model.prim_data.fogging = DRAW_DISABLE;
	teapot_model.prim_data.blending = DRAW_DISABLE;
	teapot_model.prim_data.antialiasing = DRAW_ENABLE;
	teapot_model.prim_data.mapping_type = PRIM_MAP_ST;
	teapot_model.prim_data.colorfix = PRIM_UNFIXED;

	teapot_model.color.r = 0x0;
	teapot_model.color.g = 0x80;
	teapot_model.color.b = 0x80;
	teapot_model.color.a = 0x80;
	teapot_model.color.q = 1.0f;
*/

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

qword_t *draw_model(DRAW_MODEL_PARAMS) {
	packet2_add_float(game->context.shared_packet, 2048.0f);
	packet2_add_float(game->context.shared_packet, 2048.0f);
	packet2_add_float(game->context.shared_packet, ((float)0xFFFFFF) / 32.0F);

	packet2_add_s32(game->context.shared_packet, model->point_count);

	//packet2_utils_gif_add_set(game->context.shared_packet, 1);
	//packet2_utils_gif_add_set(game->context.shared_packet, 1);
	//packet2_utils_gif_add_set(game->context.shared_packet, 1);
	//packet2_utils_gif_add_set(game->context.shared_packet, 1);
	
	
	
	/*
	MATRIX local_light;
	MATRIX local_world;
	MATRIX world_view;

	MATRIX local_screen;

	qword_t *dmatag = q;
	q++;

	create_local_world(local_world, position, rotation);
	create_local_light(local_light, rotation);	
	create_world_view(world_view, game->camera.camera_position, game->camera.camera_rotation);
	create_local_screen(local_screen, local_world, world_view, game->camera.view_screen);

	prim_t temp_prim = model->prim_data;
	VECTOR *temp_colors = model->colors;
	
	if(flags & MDL_LIGHTING) {
		calculate_normals(game->context.shared_normals, model->vertex_count, model->normals, local_light);
		calculate_lights(game->context.shared_lights, model->vertex_count, game->context.shared_normals, game->lighting.light_position, game->lighting.light_color, game->lighting.light_type, MAX_LIGHTS);
		calculate_colours(game->context.shared_colors, model->vertex_count, model->colors, game->context.shared_lights);
		temp_colors = game->context.shared_colors; // really sus
	}

	if(flags & MDL_WIREFRAME) {
		temp_prim.type = PRIM_LINE_STRIP;
	}

	calculate_vertices(game->context.shared_verticies, model->vertex_count, model->vertices, local_screen);

	draw_convert_xyz(game->context.xyz, 2048, 2048, 32, model->vertex_count, (vertex_f_t*)game->context.shared_verticies);
	draw_convert_rgbq(game->context.rgbaq, model->vertex_count, (vertex_f_t*)game->context.shared_verticies, (color_f_t*)temp_colors, model->color.a);

	if(flags & MDL_TEXTURED) {
		draw_convert_st(game->context.st, model->vertex_count, (vertex_f_t*)game->context.shared_verticies, (texel_f_t*)model->uv_coords);
	}
	
	q = draw_prim_start(q, 0, &temp_prim, &model->color);

	if(flags & MDL_TEXTURED) {
		u64 *dw = (u64*)q;
		for(int i = 0; i < model->point_count; i++) {
			*dw++ = game->context.rgbaq[model->points[i]].rgbaq;
			*dw++ = game->context.st[model->points[i]].uv;
			*dw++ = game->context.xyz[model->points[i]].xyz;
		}

		if ((u32)dw % 16) {
			*dw++ = 0;
		}

		q = draw_prim_end((qword_t*)dw,3,DRAW_STQ_REGLIST);

		DMATAG_CNT(dmatag,q-dmatag-1,0,0,0);

		return q;
	}
	
	for(int i = 0; i < model->point_count; i++) {
		q->dw[0] = game->context.rgbaq[model->points[i]].rgbaq;
		q->dw[1] = game->context.xyz[model->points[i]].xyz;
		q++;
	}

	q = draw_prim_end(q,2,DRAW_RGBAQ_REGLIST);

	DMATAG_CNT(dmatag,q-dmatag-1,0,0,0);

	return q;
	*/
}

extern u32 render_pipeline_normal_CodeStart __attribute__((section(".vudata")));
extern u32 render_pipeline_normal_CodeEnd __attribute__((section(".vudata")));

void upload_microcode() {
	u32 packet_size = packet2_utils_get_packet_size_for_program(&render_pipeline_normal_CodeStart, &render_pipeline_normal_CodeEnd)+1;
	packet2_t *packet = packet2_create(packet_size, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	packet2_vif_add_micro_program(packet, 0, &render_pipeline_normal_CodeStart, &render_pipeline_normal_CodeEnd);
	packet2_utils_vu_add_end_tag(packet);
	dma_channel_send_packet2(packet, DMA_CHANNEL_VIF1, 1);
	dma_channel_wait(DMA_CHANNEL_VIF1, 0);
	packet2_free(packet);
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
	context->shared_packet = packet2_create(10, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	context->packets[0] = packet2_create(11, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	context->packets[1] = packet2_create(11, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);

	upload_microcode();
	enable_doublebuffer();

	/*
	//dma_wait_fast();
	context->context = 0;
	
	context->packets[0] = packet_init(PACKET_SIZE, PACKET_NORMAL);
	context->packets[1] = packet_init(PACKET_SIZE, PACKET_NORMAL);
	context->flip = packet_init(3, PACKET_UCAB);
	
	context->shared_verticies = ALIGN_VERT_128(VECTOR);
	context->shared_normals = ALIGN_VERT_128(VECTOR);
	context->shared_lights = ALIGN_VERT_128(VECTOR);
	context->shared_colors = ALIGN_VERT_128(VECTOR);
	
	context->xyz = ALIGN_VERT_128(xyz_t);
	context->rgbaq = ALIGN_VERT_128(color_t);
	context->st = ALIGN_VERT_128(texel_t);
	
	//memset(context->view_screen, 0, sizeof(MATRIX));
	*/
}

qword_t *begin_render(BEGIN_RENDER_PARAMS) {
	/*
	game->context.current = game->context.packets[game->context.context];
	q = game->context.current->data;

	qword_t *dmatag = q;
	q++;

	q = draw_disable_tests(q,0,z);
	q = draw_clear(q,0,2048.0f-(FB_WIDTH/2),2048.0f-(FB_HEIGHT/2),frame->width,frame->height,0x40,0x40,0x40);
	q = draw_enable_tests(q,0, z);

	DMATAG_CNT(dmatag,q-dmatag - 1,0,0,0);

	return q;
	*/
}

void flip_buffers(packet_t *flip, framebuffer_t *frame) {
	/*
	qword_t *q = flip->data;

	q = draw_framebuffer(q,0,frame);
	q = draw_finish(q);

	dma_wait_fast();
	dma_channel_send_normal_ucab(DMA_CHANNEL_GIF,flip->data,q - flip->data, 0);

	draw_wait_finish();
	*/
}

qword_t *end_render(END_RENDER_PARAMS) {
	/*
	qword_t *dmatag = q;
	q++;

	q = draw_finish(q);
	DMATAG_END(dmatag,q-dmatag-1,0,0,0);

	dma_wait_fast();
	dma_channel_send_chain(DMA_CHANNEL_GIF, game->context.current->data, q - game->context.current->data, 0, 0);

	graph_wait_vsync();

	draw_wait_finish();
	graph_set_framebuffer_filtered(frame[game->context.context].address,frame[game->context.context].width,frame[game->context.context].psm,0,0);

	game->context.context ^= 1;

	flip_buffers(game->context.flip, &frame[game->context.context]);

	return q;
	*/
}

/*
int render(RENDER_PARAMS)
{
	int context = 0;

	//prim_t prim;
	//color_t color;

	// The data packets for double buffering dma sends.
	//packet_t *packets[2];
	//packet_t *current;
	//
	//packets[0] = packet_init(MAX_VERTICES,PACKET_NORMAL);
	//packets[1] = packet_init(MAX_VERTICES,PACKET_NORMAL);
	
	packet_t *flip = packet_init(3,PACKET_UCAB);

	create_view_screen(game->view_screen, graph_aspect_ratio(), -3.00f, 3.00f, -3.00f, 3.00f, 1.00f, 2000.00f);

	dma_wait_fast();
	
	for (;;)
	{	
		game->current_time = clock();
		game->delta_time = (game->current_time - game->last_time) / 1000.0f;

		qword_t *q;
		qword_t *dmatag;

		current = packets[context];
		dmatag = current->data;

		q = dmatag;
		q++;

		// Clear framebuffer but don't update zbuffer.
		q = draw_disable_tests(q,0,z);
		q = draw_clear(q,0,2048.0f-320.0f,2048.0f-256.0f,frame->width,frame->height,0x00,0x00,0x00);
		q = draw_enable_tests(q,0,z);

		DMATAG_CNT(dmatag,q-dmatag - 1,0,0,0);



		dmatag = q;
		q++;

		// Setup a finish event.
		q = draw_finish(q);

		DMATAG_END(dmatag,q-dmatag-1,0,0,0);
		
		dma_wait_fast();
		dma_channel_send_chain(DMA_CHANNEL_GIF, current->data, q - current->data, 0, 0);

		graph_wait_vsync();

		draw_wait_finish();
		graph_set_framebuffer_filtered(frame[context].address,frame[context].width,frame[context].psm,0,0);
		
		// Now switch our packets so we can process data while the DMAC is working.
		context ^= 1;

		// double buffer; flip packet
		q = flip->data;
		q = draw_framebuffer(q,0,frame);
		q = draw_finish(q);
	
		dma_wait_fast();
		dma_channel_send_normal_ucab(DMA_CHANNEL_GIF,flip->data,q - flip->data, 0);
	
		draw_wait_finish();
		
		game->last_time = game->current_time;
	}

	packet_free(packets[0]);
	packet_free(packets[1]);

	return 0;
}
*/ 