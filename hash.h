#include "specs_list.h"
#include "complex.h"

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
  comp_head Complex ;

  Hashed_Id next ;
};

// struct for bucket of hash table for full ids.
typedef struct Ids_Bucket* Hash_For_Id;
struct Ids_Bucket{
  Hashed_Id root ;
};

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

int hash_func(char*,int) ;

void trim(char*) ;
