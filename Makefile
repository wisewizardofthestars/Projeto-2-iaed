CFLAGS= -Wall -Wextra -Werror -ansi -pedantic
FILE = proj2

run : build
	./$(FILE).out

build:
	gcc $(CFLAGS) $(FILE).c -o $(FILE).out

test : build
	./test.sh ./$(FILE).out ./tests/community_tests

test2: build
	./test.sh ./$(FILE).out ./tests/public_tests

test3: build
	./test.sh ./$(FILE).out ./tests/testes-turno2



zip : build
	zip proj2.zip *.c *.h

clean :
	# rm *.zip
	rm vgcore*