CC       = c++ -Wfatal-errors
FLAGS    = -no-pie
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I../ThreadMentor/include
TMLIB    = ../ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = ../ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = obj/IncDec-mon.o obj/IncDec-Thrd.o obj/IncDec-main.o 
EXE_FILE = bin/prog

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread

obj/IncDec-mon.o: IncDec-mon.cpp IncDec-mon.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c IncDec-mon.cpp -o $@

obj/IncDec-Thrd.o: IncDec-Thrd.cpp IncDec-Thrd.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c IncDec-Thrd.cpp -o $@

obj/IncDec-main.o: IncDec-main.cpp IncDec-mon.cpp IncDec-mon.h IncDec-Thrd.cpp IncDec-Thrd.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c IncDec-main.cpp -o $@

noVisual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

clean:
	rm -f bin/*
	rm -f obj/*

dirs:
	mkdir -p bin
	mkdir -p obj

export:
	export PATH=/home/user/Desktop/ThreadMentor/bin:$PATH
