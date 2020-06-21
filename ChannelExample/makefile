CC       = c++ -Wfatal-errors
FLAGS    = -no-pie
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I../ThreadMentor/include
TMLIB    = ../ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = ../ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = obj/sort-thread.o obj/sort-main.o 
EXE_FILE = bin/prog

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread

obj/sort-thread.o: sort-thread.cpp sort-thread.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c sort-thread.cpp -o $@

obj/sort-main.o: sort-main.cpp sort-thread.cpp sort-thread.h
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c sort-main.cpp -o $@

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
