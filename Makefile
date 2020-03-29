CC := g++
ROOTFLAG := $(shell root-config --cflags)
ROOTLIB := $(shell root-config --libs)
BIN := bin
SRC := src
INC := include
BUI := build

all: ${BIN}/Presel_a

${BIN}/Presel_%: ${BUI}/Presel_%.o
	@echo "[MAKE] Making $@"
	${CC} -o $@ $^ ${ROOTLIB}

${BUI}/%.o: ${SRC}/%.cc
	@echo "[MAKE] Making $@"
	${CC} -c -o $@ $< ${ROOTFLAG}
