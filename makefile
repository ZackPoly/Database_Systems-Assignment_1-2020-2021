
main: site_hash.o id_hash.o bag_of_words.o model.o queue.o job_scheduler.o ./JSON_hashing/site_hash.h ./logistic_regression/bag_of_words.h ./logistic_regression/model.h ./multithreading/job_scheduler.h
	gcc site_hash.o id_hash.o bag_of_words.o model.o queue.o job_scheduler.o -o main main.c -ggdb3 -pthread -lm

site_hash.o: ./JSON_hashing/site_hash.c ./JSON_hashing/site_hash.h
	gcc -c ./JSON_hashing/site_hash.c -ggdb3

id_hash.o: ./JSON_hashing/id_hash.c ./JSON_hashing/id_hash.h
	gcc -c ./JSON_hashing/id_hash.c -ggdb3

bag_of_words.o: ./logistic_regression/bag_of_words.c ./logistic_regression/bag_of_words.h
	gcc -c ./logistic_regression/bag_of_words.c -ggdb3 -lm

model.o: ./logistic_regression/model.c ./logistic_regression/model.h
	gcc -c ./logistic_regression/model.c -ggdb3 -lm

queue.o: ./multithreading/queue.c ./multithreading/job_scheduler.h
	gcc -c ./multithreading/queue.c -ggdb3

job_scheduler.o: ./multithreading/job_scheduler.c ./multithreading/job_scheduler.h
	gcc -c ./multithreading/job_scheduler.c -ggdb3 -pthread -lm
