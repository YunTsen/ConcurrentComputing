CC       = g++
FLAGS    = -no-pie
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I/home/user/Desktop/ThreadMentor/include
TMLIB    = /home/user/Desktop/ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = /home/user/Desktop/ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = quicksort.o quicksort-main.o
EXE_FILE = quicksort

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread

quicksort.o: quicksort.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c quicksort.cpp

quicksort-main.o: quicksort-main.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c quicksort-main.cpp

noVisual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}

export PATH=/home/user/Desktop/ThreadMentor/bin:$PATH