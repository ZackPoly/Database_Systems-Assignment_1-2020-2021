#define BOW_B_NUM 1001
#define BOW_STEP 1000

// typedef struct Hash_For_Id_Entry* Hashed_Id ;
// struct Hash_For_Id_Entry ;
//
// typedef struct Ids_Bucket* Hash_For_Id;
// struct Ids_Bucket ;
//
// typedef struct Hash_For_Site_Entry* Hashed_Site ;
// struct Hash_For_Site_Entry ;
//
typedef struct Sites_Bucket* Hash_For_Site;
struct Sites_Bucket ;
//
// typedef struct str_node* str_list ;
// struct str_node ;
//
// typedef struct Spec_node* specs ;
// struct Spec_node ;


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

  char** full_id_array ;

  float** values ;

  int bow_len ;
  int filesNum ;
};

Hashed_Word search_word_in_hash(Hash_For_Word,char*,int*) ;


int insert_word_in_hash(BoW,char*) ;

void initialize_bow(Hash_For_Site,BoW*,int,int,int) ;
