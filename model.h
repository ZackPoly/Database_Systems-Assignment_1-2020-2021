#include "site_hash.h"
#include "bag_of_words.h"

typedef struct model* Model;
struct model{
   float* weigths;
   float b;
   float h;
};


void initialize_model(Model,int,float,float);

void fit(Model,BoW bow,char*);
