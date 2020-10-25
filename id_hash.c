#include "id_hash.h"

int hash_func(char* key,int bucketsNum){
  int i,v=0 ;
  for(i=0;i<=strlen(key);i++){
    v=v+key[i] ;
  }
  return v%bucketsNum ;
}


Hashed_Id search_id_in_hash(Hash_For_Id IdTable,char* full_id,int bucketsNum){
  int hashIndex=hash_func(full_id,bucketsNum);

  Hashed_Id currId=IdTable[hashIndex].root ;
  while(currId!=NULL){
    if(!strcmp(currId->full_id,full_id)) return currId ;

    currId=currId->next ;
  }
  return NULL ;
}

void insert_id_in_hash(Hash_For_Id IdTable,int bucketsNum,char* full_id,char* specsData){

  /*                     DUPLICATE ID REJECTED                     */
  if(search_id_in_hash(IdTable,full_id,bucketsNum)!=NULL) return NULL ;

  /*                  EMPTY BUCKET LIST                     */
  if(IdTable[hashIndex].root==NULL){
    IdTable[hashIndex].root=malloc(sizeof(struct Hash_For_Id_Entry)) ;
    IdTable[hashIndex].root->next=NULL ;
    IdTable[hashIndex].root->full_id=malloc(strlen(full_id)+1) ;
    strcpy(IdTable[hashIndex].root->full_id,full_id) ;
    /////////////
    ///////////// initialize Specs
    /////////////

                      /* initialize Complex */
    IdTable[hashIndex].root->Complex=malloc(sizeof(struct Comp_node)) ;
    IdTable[hashIndex].root->Complex->id=malloc(strlen(full_id)+1) ;
    strcpy(IdTable[hashIndex].root->Complex->id,full_id) ;

    return IdTable[hashIndex].root ;
  }

  /*                  APPEND BUCKET LIST                     */
  Hashed_Id curId=IdTable[hashIndex].root ;

  while(curId->next!=NULL){ // collision
    curId=curId->next ;
  }
  curId->next=malloc(sizeof(struct Hash_For_Site_Entry)) ;
  curId->next->next=NULL ;
  curId->next->full_id=malloc(strlen(full_id)+1) ;
  strcpy(curId->next->full_id,full_id) ;
  // initialize Specs

  // initialize Complex
  curId->next->Complex=malloc(sizeof(struct Comp_node)) ;
  curId->next->Complex->id=malloc(strlen(full_id)+1) ;
  strcpy(curId->next->Complex->id,full_id) ;

  return curId->next ;
}
