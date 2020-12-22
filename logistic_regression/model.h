#include "../JSON_hashing/site_hash.h"
#include "bag_of_words.h"

typedef struct model* Model;
struct model{
   double* weights;
   double b;
   double h;
};


void initialize_model(Model*,int,double,double);

void fit(Model,BoW,char*,Hash_For_Site,int,int);

void predict(Model,BoW,char*,Hash_For_Site,int,int) ;

void delete_model(Model*);
