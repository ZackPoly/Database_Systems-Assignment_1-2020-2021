#include <ctype.h>
#include <math.h>

#define BOW_B_NUM 1001
#define BOW_STEP 17000


typedef struct Sites_Bucket* Hash_For_Site;
struct Sites_Bucket ;

typedef struct Dictionary_Word* Hashed_Word ;
struct Dictionary_Word{
  char* word ;
  int index ;

  Hashed_Word next ;
};

// struct for bucket of hash table for dictioary words.
typedef struct Words_Bucket* Hash_For_Word;
struct Words_Bucket{
  Hashed_Word root ;
};

typedef struct Bag_Of_Words* BoW ;
struct Bag_Of_Words{
  Hash_For_Word dict ;
  int dict_len ;

  // char** full_id_array ;

  int* filesLen ;
  int* wordsFileCount ;                 // in how many files each word appears

  double** values ;                     // the vectors

  int bow_len ;
  int filesNum ;
};

Hashed_Word search_word_in_hash(Hash_For_Word,char*,int*) ;


int insert_word_in_hash(BoW,char*) ;

void initialize_bow(Hash_For_Site,BoW*,int,int,int) ;


void delete_dict_word_by_index(BoW,int) ;

void bow_to_tf_idf(BoW bow) ;
