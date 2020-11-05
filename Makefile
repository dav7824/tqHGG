CC := g++
ROOTFLAG := $(shell root-config --cflags)
ROOTLIB := $(shell root-config --libs)
BIN := bin
SRC := src
INC := include
BUI := build

all: 

${BIN}/Presel_%: ${BUI}/Presel_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/CalcYield: ${BUI}/CalcYield.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/AddYields: ${BUI}/AddYields.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/PrintYieldSummary: ${BUI}/PrintYieldSummary.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/PrintYieldSummary_sig: ${BUI}/PrintYieldSummary_sig.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/HistNorm: ${BUI}/HistNorm.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/HistAdd: ${BUI}/HistAdd.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/HistPlotter: ${BUI}/HistPlotter.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/AddTreeVar: ${BUI}/AddTreeVar.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/FillHist%: ${BUI}/FillHist%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/FindGenPart: ${BUI}/FindGenPart.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/MVAreco_GenPerm_%: ${BUI}/MVAreco_GenPerm_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/MVAreco_train_%: ${BUI}/MVAreco_train_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB} -lTMVA

${BIN}/MVAreco_application: ${BUI}/MVAreco_application.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB} -lTMVA

${BIN}/MVAreco_AdjustTree: ${BUI}/MVAreco_AdjustTree.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/NuPz: ${BUI}/NuPz.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/DivideTree%: ${BUI}/DivideTree%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/DivideSamples%: ${BUI}/DivideSamples%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/SFcalc_%: ${BUI}/SFcalc_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/SFbtag_extrapolation: ${BUI}/SFbtag_extrapolation.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/SFbtag_extrapolation_add: ${BUI}/SFbtag_extrapolation_add.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/CountEvents: ${BUI}/CountEvents.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/MakeBDTinput_%: ${BUI}/MakeBDTinput_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/BDTtrain_%: ${BUI}/BDTtrain_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB} -lTMVA

${BIN}/MakeDataDummySFtree: ${BUI}/MakeDataDummySFtree.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/BDTapplication: ${BUI}/BDTapplication.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB} -lTMVA

${BIN}/Opt_test: ${BUI}/Opt_test.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BUI}/MVAreco_GenPerm_%.o: ${SRC}/MVAreco_GenPerm_%.cc ${INC}/MVAreco_GenPerm.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/FillHist%.o: ${SRC}/FillHist%.cc ${INC}/SFhelper.h ${INC}/utility.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/CalcYield.o: ${SRC}/CalcYield.cc ${INC}/SFhelper.h ${INC}/utility.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/SFcalc_btag.o: ${SRC}/SFcalc_btag.cc ${INC}/BTagCalibrationStandalone.h ${INC}/BTagCalibrationStandalone.cpp
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/SFbtag_extrapolation.o: ${SRC}/SFbtag_extrapolation.cc ${INC}/BTagCalibrationStandalone.h ${INC}/BTagCalibrationStandalone.cpp
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/%.o: ${SRC}/%.cc ${INC}/utility.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/%.o: tmp/%.cc ${INC}/utility.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings
