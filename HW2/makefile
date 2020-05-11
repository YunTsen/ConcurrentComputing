CC       = gcc -Wfatal-errors -Werror -std=c89

.PHONY:run run2 clean
all:main merge run
test:ut_main run2

main: main.c
	${CC}   $< -o $@

merge: merge.c
	${CC}  -Wfatal-errors -Werror   $< -o $@

run:
	./main < file.txt

ut_main: ut_main.c
	${CC} -Wfatal-errors -Werror   $< -o $@
run2:
	./ut_main
clean:
	rm -f merge main main ut_main