PWD=$(shell pwd)

FAUSTPATH_PREFIX?=/usr

CXXFLAGS="-I$(FAUSTPATH_PREFIX)/include/"
FAUST_DSPDIR=$(FAUSTPATH_PREFIX)/share/faust/

main: clean faust
	c++ -I$(FAUSTPATH_PREFIX)/include/ \
	-I . -std=c++11 -Ofast -march=native jack-sfreload.cpp -lOSCFaust \
	-D SOUNDFILE -D SOUNDFILE_RELOAD -D MIDICTRL \
	-L$(FAUSTPATH_PREFIX)/lib -I/usr/include/opus -I/usr/include/x86_64-linux-gnu -ljack -lsndfile -lmp3lame -lFLAC -lm -logg -lvorbis -lm -logg -lvorbisenc -lvorbis -lm -logg -lopus -lm -lmpg123 -lm   -lz -lncurses  -lzstd -ltinfo -lpthread  -o jack-sfreload

faust:
	$(FAUSTPATH_PREFIX)/bin/faust \
	-A $(PWD) -a jack-console.cpp \
	test.dsp -o jack-sfreload.cpp
	
test: main
	./jack-sfreload

clean:
	rm -f jack-sfreload jack-sfreload.cpp
