#include "id_hash.h"

typedef struct Hash_For_Site_Entry* hashed_Site ;
struct Hash_For_Site_Entry{
  char* site ;

  Hash_For_Id Id_Hash_Array ;

  hashed_Site next ;
};

typedef struct Sites_Bucket* Hash_For_Site;
struct Sites_Bucket{
  hashed_Site root ;
};
