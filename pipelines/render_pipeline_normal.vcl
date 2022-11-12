.name render_pipeline_normal
#vuprog
    lq matrix_row_0, 0(vi00)
    lq matrix_row_1, 1(vi00)
    lq matrix_row_2, 2(vi00)
    lq matrix_row_3, 3(vi00)
    
    fcset 0x000000

    xtop ibase

    lq.xyz scale, 0(ibase)

    lq gif_set_tag, 1(ibase)
    lq tex_gif_tag1, 2(ibase)
    lq tex_gif_tag2, 3(ibase)
    lq prim_tag, 4(ibase)
    lq rgba, 5(ibase) ; q is stored in ST when passed into VU1

    iaddiu vertex_data, ibase, 6
    ilw.w vert_count, 0(ibase)
    iadd stq_data, vertex_data, vert_count
    iadd kick_address, stq_data, vert_count
    iadd dest_address, stq_data, vert_count

    sqi gif_set_tag, (dest_address++)
    sqi tex_gif_tag1, (dest_address++)
    sqi gif_set_tag, (dest_address++)
    sqi tex_gif_tag2, (dest_address++)
    sqi prim_tag, (dest_address++)

    iadd vertex_counter, ibase, vert_count
    vertex_loop:
        lq vertex, 0(vertex_data)
        lq stq, 0(stq_data)

        mul acc, matrix_row_0, vertex[x]
        madd acc, matrix_row_1, vertex[y]
        madd acc, matrix_row_2, vertex[z]
        madd vertex, matrix_row_3, vertex[w]
        
        ; frustum culling
        clipw.xyz vertex, vertex
        fcand vi01, 0x3FFFF
        iaddiu iadc, vi01, 0x7FFF

        isw.w iadc, 2(dest_address)

        div q, vf00[w], vertex[w]
        mul.xyz vertex, vertex, q
        mula.xyz acc, scale, vf00[w]
        madd.xyz vertex, vertex, scale
        ftoi4.xyz vertex, vertex

        mulq modstq, stq, q

        sq modstq, 0(dest_address)
        sq rgba, 1(dest_address)
        sq.xyz vertex, 2(dest_address)

        iaddiu vertex_data, vertex_data, 1
        iaddiu stq_data, stq_data, 1
        iaddiu dest_address, dest_address, 3
        
        iaddi vertex_counter, vertex_counter, -1
        ibne vertex_counter, ibase, vertex_loop
    
    --barrier

    xgkick kick_address
#endvuprog