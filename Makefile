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

${BIN}/AddVar_%: ${BUI}/AddVar_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/FillHist_%: ${BUI}/FillHist_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/FindGenPart: ${BUI}/FindGenPart.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/MVAreco_GenPerm_%: ${BUI}/MVAreco_GenPerm_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/NuPz: ${BUI}/NuPz.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BUI}/MVAreco_GenPerm_%.o: ${SRC}/MVAreco_GenPerm_%.cc ${INC}/MVAreco_GenPerm.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/%.o: ${SRC}/%.cc ${INC}/utility.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings

${BUI}/%.o: tmp/%.cc ${INC}/utility.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings
