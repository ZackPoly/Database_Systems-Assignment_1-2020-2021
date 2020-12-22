
main: site_hash.o id_hash.o bag_of_words.o model.o ./JSON_hashing/site_hash.h ./logistic_regression/bag_of_words.h ./logistic_regression/model.h
	gcc site_hash.o id_hash.o bag_of_words.o model.o -o main main.c -ggdb3 -lm

site_hash.o: ./JSON_hashing/site_hash.c ./JSON_hashing/site_hash.h
	gcc -c ./JSON_hashing/site_hash.c -ggdb3

id_hash.o: ./JSON_hashing/id_hash.c ./JSON_hashing/id_hash.h
	gcc -c ./JSON_hashing/id_hash.c -ggdb3

bag_of_words.o: ./logistic_regression/bag_of_words.c ./logistic_regression/bag_of_words.h
	gcc -c ./logistic_regression/bag_of_words.c -ggdb3 -lm

model.o: ./logistic_regression/model.c ./logistic_regression/model.h
	gcc -c ./logistic_regression/model.c -ggdb3 -lm
