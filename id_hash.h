#include "specs_list.h"
#include "complex.h"
#include "specs_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

// struct for list of ids of same bucket
typedef struct Hash_For_Id_Entry* Hashed_Id ;
struct Hash_For_Id_Entry{
  char* full_id ;

  specs Specs ;
  complex Complex ;

  hashed_id next ;
};

// struct for bucket of hash table for full ids.
typedef struct Ids_Bucket* Hash_For_Id;
struct Ids_Bucket{
  hashed_Id root ;
};

int hash_func(char*,int) ;

Hashed_Id search_id_in_hash(Hash_For_Id,char*,int) ;

                                    //  id   file-data
void insert_id_in_hash(Hash_For_Id,int,char*,char*) ;
