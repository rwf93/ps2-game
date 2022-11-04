EE_CFLAGS = -g
EE_BIN = game.elf
EE_OBJS = game.o render.o pad.o model.o
EE_LIBS = -ldraw -lgraph -lmath3d -lpacket -ldma -lpad -ldebug -lc -lfreetype -lpng -lz
EE_PCH = pch.h.gch$(subst .ttf,.ttf.h, $(shell ls assets/*.ttf))

all: $(EE_PCH) $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN) 

clean:
	rm -f $(EE_BIN) $(EE_OBJS) $(EE_PCH)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

include Makefile.pref
include Makefile.eeglobal