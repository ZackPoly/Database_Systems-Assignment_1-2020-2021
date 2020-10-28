#include "id_hash.h"

typedef struct Hash_For_Site_Entry* Hashed_Site ;
struct Hash_For_Site_Entry{
  char* site ;

  Hash_For_Id Id_Hash_Array ;

  Hashed_Site next ;
};

typedef struct Sites_Bucket* Hash_For_Site;
struct Sites_Bucket{
  Hashed_Site root ;
};


Hashed_Site search_site_in_hash(Hash_For_Site,char*,int) ;

                      // hashTable     site  bucketSize
Hashed_Site insert_site_in_hash(Hash_For_Site,char*,int,int) ;

Hashed_Id search_complex(Hash_For_Site,int,int,char*) ;

void get_site_from_id(char*,const char*);
