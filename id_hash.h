#include "specs_list.h"
#include "complex.h"

// struct for list of ids of same bucket
typedef struct Hash_For_Id_Entry* hashed_Id ;
struct Hash_For_Id_Entry{
  char* id ;

  specs Specs ;
  complex Complex ;

  hashed_id next ;
};

// struct for bucket of hash table for full ids.
typedef struct Ids_Bucket* Hash_For_Id;
struct Ids_Bucket{
  hashed_Id root ;
};
