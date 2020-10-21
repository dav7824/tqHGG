ROOT_FLAGS := $(shell root-config --cflags)
ROOT_LIB := $(shell root-config --libs)

all: SFcalc_btag SFcalc_print

SFcalc_btag: SFcalc_btag.o
	g++ -o $@ $^ $(ROOT_LIB)

SFcalc_btag.o: SFcalc_btag.cc BTagCalibrationStandalone.h BTagCalibrationStandalone.cpp
	g++ -c -o $@ $< $(ROOT_FLAGS) -I.

SFcalc_print: SFcalc_print.o
	g++ -o $@ $^ $(ROOT_LIB)

SFcalc_print.o: SFcalc_print.cc BTagCalibrationStandalone.h BTagCalibrationStandalone.cpp
	g++ -c -o $@ $< $(ROOT_FLAGS) -I.
