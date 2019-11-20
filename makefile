run : dnf.c.o
	gcc -o run dnf.c.o
dnf.c.o : dnf.c.c
	gcc -c -o dnf.c.o dnf.c.c
