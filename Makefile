# Sample Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra 
KITTEN_IMG=/home/ips24042/myrepo220/CS220/projects/midterm/data/kitten.ppm
TREES_IMG=/home/ips24042/myrepo220/CS220/projects/midterm/data/trees.ppm

# Links together files needed to create executable. The "-lm" should be where it is in this file. If it's after image_manip.o,
# it won't work. 
#main: main_file.o functions.o
project: project.o image_manip.o ppm_io.o
	$(CC) -o project project.o image_manip.o ppm_io.o -lm

# Compiles mainFile.c to create mainFile.o
# Note that we list functions.h here as a file that
# main_file.c depends on, since main_file.c #includes it
#main_file.o: main_file.c functions.h
project.o: project.c image_manip.h ppm_io.h
	$(CC) $(CFLAGS) -c project.c 

# Compiles functions.c to create functions.o
# Note that we list functions.h here as a file that
# functions.c depends on, since functions.c #includes it
#functions.o: functions.c functions.h
image_manip.o: image_manip.c image_manip.h ppm_io.h
	$(CC) $(CFLAGS) -c image_manip.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c 

# Removes all object files and the executable named main,
# so we can start fresh
clean:
	rm -f *.o project