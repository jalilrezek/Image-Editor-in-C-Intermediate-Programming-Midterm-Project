CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -lm
KITTEN_IMG=/home/ips24042/myrepo220/CS220/projects/midterm/data/kitten.ppm
TREES_IMG=/home/ips24042/myrepo220/CS220/projects/midterm/data/trees.ppm

project: project.c image_manip.c ppm_io.c ppm_io.h image_manip.h
	$(CC) $(CFLAGS) project.c image_manip.c ppm_io.c -o project

# Removes all object files and the executable named main,
# so we can start fresh
clean:
	rm -f *.o project
