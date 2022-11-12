EE_CFLAGS = -g -I./thirdparty

EE_OBJS = game.o render.o pad.o model.o texture.o
# VU pipelines
EE_OBJS += \
		pipelines/render_pipeline_normal.vo \
		pipelines/render_pipeline_test.vo
		

EE_LIBS = -ldraw -lgraph -lmath3d -lpacket -ldma -lpad -ldebug -lc -lfreetype -lpng -lz -lpatches -lpacket2

THIRDPARTY = thirdparty/fast_obj.o thirdparty/vec.o
PRECOMPILED = pch.h.gch $(subst .ttf,.ttf.h,$(shell ls assets/*.ttf)) $(subst .raw,.raw.h,$(shell ls assets/*.raw))

EE_BIN = game.elf
EE_ISO = game.iso

all: BUILD_THIRDPARTY $(PRECOMPILED) $(EE_ISO)
#	 $(EE_STRIP) --strip-all $(EE_BIN) 

$(EE_ISO): $(EE_BIN)
	mkisofs --quiet -l -o $(EE_ISO) $(EE_BIN) packaged/

clean: CLEAN_THIRDPARTY
	rm -f $(EE_BIN) $(EE_OBJS) $(PRECOMPILED) $(EE_ISO)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

BUILD_THIRDPARTY: $(THIRDPARTY)
	make --quiet -C thirdparty/vclpp
	make --quiet -C thirdparty/openvcl
	
CLEAN_THIRDPARTY:
	rm -f $(THIRDPARTY)
	# make --quiet -C thirdparty/vclpp clean
	# make --quiet -C thirdparty/openvcl clean

include Makefile.pref
include Makefile.eeglobal
include Makefile.filetypes