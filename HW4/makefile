CC       = c++ -Wfatal-errors
FLAGS    = -no-pie
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I../ThreadMentor/include
TMLIB    = ../ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = ../ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = obj/thread.o obj/thread-support.o obj/thread-main.o 
EXE_FILE = bin/prog4

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread

obj/thread.o: thread.cpp thread.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.cpp -o $@

obj/thread-support.o: thread-support.cpp thread.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-support.cpp -o $@

obj/thread-main.o: thread-main.cpp thread.h thread.cpp thread-support.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-main.cpp -o $@

noVisual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

clean:
	rm -f bin/*
	rm -f obj/*

dirs:
	mkdir -p bin
	mkdir -p obj

exe:
	./bin/prog4 5 8 12

export:
	export PATH=/home/user/Desktop/ThreadMentor/bin:$PATH
