EE_CFLAGS = -g -I./thirdparty
EE_BIN = game.elf

ISO = game.iso
THIRDPARTY = thirdparty/fast_obj.o thirdparty/hashmap.o thirdparty/vec.o
PRECOMPILED = pch.h.gch $(subst .ttf,.ttf.h,$(shell ls assets/*.ttf)) $(subst .raw,.raw.h,$(shell ls assets/*.raw))

EE_OBJS = game.o render.o pad.o model.o texture.o $(THIRDPARTY)
EE_LIBS = -ldraw -lgraph -lmath3d -lpacket -ldma -lpad -ldebug -lc -lfreetype -lpng -lz -lpatches

BUILD_THIRDPARTY:
	make -C thirdparty/openvcl --silent

CLEAN_THIRDPARTY:
	make -C thirdparty/openvcl clean --silent

all: BUILD_THIRDPARTY $(PRECOMPILED) $(EE_ISO)

$(ISO): $(EE_BIN)
	mkisofs -l -o $(ISO) $(EE_BIN) packaged/

clean: CLEAN_THIRDPARTY
	rm -f $(EE_BIN) $(EE_OBJS) $(PRECOMPILED) $(ISO)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

include Makefile.pref
include Makefile.eeglobal
include Makefile.filetypes