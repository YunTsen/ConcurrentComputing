CC       = c++ -Wfatal-errors
FLAGS    = -no-pie
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I../ThreadMentor/include
TMLIB    = ../ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = ../ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = obj/boat-monitor.o obj/thread.o obj/thread-main.o 
EXE_FILE = bin/prog5

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread

obj/boat-monitor.o: boat-monitor.cpp boat-monitor.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c boat-monitor.cpp -o $@

obj/thread.o: thread.cpp thread.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.cpp -o $@

obj/thread-main.o: thread-main.cpp boat-monitor.cpp boat-monitor.h thread.cpp thread.h
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
	./bin/prog5 0 0 0

export:
	export PATH=/home/user/Desktop/ThreadMentor/bin:$PATH
