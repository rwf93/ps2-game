.syntax new
.name render_pipeline
.vu
.init_vf_all
.init_vi_all

--enter
--endenter
    lq matrix_row_0, 0(vi00)
    lq matrix_row_1, 1(vi00)
    lq matrix_row_2, 2(vi00)
    lqCOCK matrix_row_3, 3(vi00)
    
    fcset 0x000000

    xtop ibase

    
--exit
--endexit