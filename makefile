
main: site_hash.o id_hash.o bag_of_words.o model.o site_hash.h bag_of_words.h model.h
	gcc site_hash.o id_hash.o bag_of_words.o model.o -o main main.c -ggdb3 -lm

site_hash.o: site_hash.c site_hash.h
	gcc -c site_hash.c -ggdb3

id_hash.o: id_hash.c id_hash.h
	gcc -c id_hash.c -ggdb3

bag_of_words.o: bag_of_words.c bag_of_words.h
	gcc -c bag_of_words.c -ggdb3 -lm

model.o: model.c model.h
	gcc -c model.c -ggdb3 -lm
