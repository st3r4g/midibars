LDLIBS = `pkg-config --libs rtmidi`
CXXFLAGS = -I midifile/include `pkg-config --cflags rtmidi`

midibars: midifile/lib/libmidifile.a

midifile/lib/libmidifile.a:
	$(MAKE) -C midifile library
