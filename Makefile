############################
#
# Makefile assignment 2
#
# Eduardo Valdovinos cssc3166
# Chris Palomares    cssc3146  --> Please use for grading

###########################


EXEC = xsh
CC = g++
FLAGS = -Wall -Wextra -std=c++17

FILES = main.cpp parser.cpp pipeline.cpp

$(EXEC):
	g++ $(FLAGS) -o $(EXEC) $(FILES)

clean:
	rm -f *.o core a.out $(EXEC)

########## EOF #############

