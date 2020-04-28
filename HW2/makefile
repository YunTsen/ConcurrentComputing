.PHONY:run run2
all:main merge run
test:ut_main run2

main: main.c
	gcc  -Wfatal-errors -Werror   $< -o $@

merge: merge.c
	gcc  -Wfatal-errors -Werror   $< -o $@

run:
	./main < file.txt

ut_main: ut_main.c
	gcc -Wfatal-errors -Werror   $< -o $@
run2:
	./ut_main