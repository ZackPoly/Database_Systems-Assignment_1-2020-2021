
main: site_hash.o id_hash.o site_hash.h
	gcc site_hash.o id_hash.o -o main main.c

site_hash.o: site_hash.c site_hash.h
	gcc -c site_hash.c

id_hash.o: id_hash.c id_hash.h
	gcc -c id_hash.c
