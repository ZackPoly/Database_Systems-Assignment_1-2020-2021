
main: site_hash.o id_hash.o bag_of_words.o site_hash.h bag_of_words.h
	gcc site_hash.o id_hash.o bag_of_words.o -o main main.c -ggdb3

site_hash.o: site_hash.c site_hash.h
	gcc -c site_hash.c -ggdb3

id_hash.o: id_hash.c id_hash.h
	gcc -c id_hash.c -ggdb3

bag_of_words.o: bag_of_words.c bag_of_words.h
	gcc -c bag_of_words.c -ggdb3
