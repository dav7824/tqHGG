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

${BIN}/SampleSummary: ${BUI}/SampleSummary.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/AddVar_%: ${BUI}/AddVar_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/FillHist_%: ${BUI}/FillHist_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BIN}/MVAreco_%: ${BUI}/MVAreco_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB} -lTMVA

${BIN}/NuPz: ${BUI}/NuPz.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BUI}/%.o: ${SRC}/%.cc ${INC}/utility.h
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG} -I. -Wno-write-strings
