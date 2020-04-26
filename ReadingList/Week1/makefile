.PHONY:dir
all: dir bin/pA bin/pB bin/pC bin/procA bin/procB

/bin/pA: pA.c
	#錯誤指令，得到.o文件，而非可執行檔，執行會回報cannot execute binary file: Exec format error
	#g++ -c pA.c -o pA
	g++ $< -o $@

bin/pB: pB.c
	g++ -o $@ $<

bin/pC: pC.c
	g++ -o $@ $<

bin/procA: procA.c
	g++ -o $@ $<

bin/procB: procB.c
	g++ -o $@ $<

dir:
	mkdir -p bin
	