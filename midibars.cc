#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <unistd.h>

#include "MidiFile.h"
#include <RtMidi.h>

using namespace std;
using namespace smf;

const char *usage = "usage: midiplay [-b bar] [-d dur] [-h] file\n\
\n\
Play selected bars from a midi file\n\
\n\
positional arguments:\n\
  file                     midi file\n\
\n\
optional arguments:\n\
  -b                       bar      (default: 1)\n\
  -d                       duration (default: 1)\n\
  -h                       display help and exit\n";


int main(int argc, char *argv[]) {
	int bar = 1, duration = 1;
	MidiFile midifile;
	RtMidiOut *midiout = new RtMidiOut();
	int opt;

	while ((opt = getopt(argc, argv, "b:d:h")) != -1) {
		switch (opt) {
		case 'b': bar = atoi(optarg); break;
		case 'd': duration = atoi(optarg); break;
		case 'h': cout << usage; return EXIT_SUCCESS;
		}
	}

	if (optind >= argc) {
		cout << usage; return EXIT_FAILURE;
	} else
		midifile.read(argv[optind]);

	midifile.doTimeAnalysis();
	midifile.joinTracks();
//	midifile.linkNotePairs();

	// Check available ports.
	unsigned int nPorts = midiout->getPortCount();
	if (nPorts == 0) {
		cout << "No ports available!\n";
		return EXIT_FAILURE;
	}
	// Open first available port.
	midiout->openPort(0);

	int track=0;
	int a = midifile.getTicksPerQuarterNote()*4*(bar-1);
	int b = midifile.getTicksPerQuarterNote()*4*(bar-1+duration);
	// TODO: clean this mess, split this into 0->a and a->b
	for (int event=0; event<midifile[track].size(); event++) {
		if (midifile[track][event].tick < a && !midifile[track][event].isMeta() && !midifile[track][event].isNote()) midiout->sendMessage(&midifile[track][event]);
		if (midifile[track][event].tick < a) continue;
		if (midifile[track][event].tick > b) break;
		if (midifile[track][event].tick == b && midifile[track][event].isNoteOff())
			midiout->sendMessage(&midifile[track][event]);
		else if (midifile[track][event].tick == b) {
		} else {
		if (!midifile[track][event].isMeta())
			midiout->sendMessage(&midifile[track][event]);
		if (event+1 < midifile[track].size() && midifile[track][event+1].tick-midifile[track][event].tick !=0)
//		this_thread::sleep_for(chrono::milliseconds(int((midifile[track][event+1].seconds-midifile[track][event].seconds)*1000)));
//TODO: use ticks here
		this_thread::sleep_for(chrono::duration<double>(midifile[track][event+1].seconds-midifile[track][event].seconds));
		}
	}

	return EXIT_SUCCESS;
}
