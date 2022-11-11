EE_CFLAGS = -g -I./thirdparty

EE_OBJS = $(THIRDPARTY) game.o render.o pad.o model.o texture.o
EE_LIBS = -ldraw -lgraph -lmath3d -lpacket -ldma -lpad -ldebug -lc -lfreetype -lpng -lz -lpatches

THIRDPARTY = thirdparty/fast_obj.o thirdparty/vec.o
PRECOMPILED = pch.h.gch $(subst .ttf,.ttf.h,$(shell ls assets/*.ttf)) $(subst .raw,.raw.h,$(shell ls assets/*.raw))

EE_BIN = game.elf
EE_ISO = game.iso

all: BUILD_THIRDPARTY $(PRECOMPILED) $(EE_ISO)
#	 $(EE_STRIP) --strip-all $(EE_BIN) 

$(EE_ISO): $(EE_BIN)
	mkisofs -l -o $(EE_ISO) $(EE_BIN) packaged/

clean: CLEAN_THIRDPARTY
	rm -f $(EE_BIN) $(EE_OBJS) $(PRECOMPILED) $(EE_ISO)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

BUILD_THIRDPARTY:	
	make -C thirdparty/openvcl
	
CLEAN_THIRDPARTY:
	make -C thirdparty/openvcl clean

EE_VCL = thirdparty/openvcl/openvcl
EE_DVP = dvp-as

include Makefile.pref
include Makefile.eeglobal
include Makefile.filetypes