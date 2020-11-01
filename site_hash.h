#include "id_hash.h"

Hashed_Site search_site_in_hash(Hash_For_Site,char*,int) ;

                      // hashTable     site  bucketSize
Hashed_Site insert_site_in_hash(Hash_For_Site,char*,int,int) ;

Hashed_Id search_complex(Hash_For_Site,int,int,char*) ;

void get_site_from_id(char*,const char*);

void delete(Hash_For_Site,int,int);
