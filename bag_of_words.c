#include "bag_of_words.h"
#include "hash.h"
#include <ctype.h>

void preprocess(char* value){
  int i ;
  for(i=0 ; i<strlen(value) ; i++){
    if(isupper(value[i]))
      value[i]=tolower(value[i]) ;
    else if(ispunct(value[i]))
      value[i]=' ' ;
    else if(isdigit(value[i]))
      value[i]=' ' ;
    else if(isspace(value[i]))
      value[i]=' ' ;
  }
}

Hashed_Word search_word_in_hash(Hash_For_Word WordTable,char* word,int* index){
  int hashIndex=hash_func(word,BOW_B_NUM);

  Hashed_Word currWord=WordTable[hashIndex].root ;
  while(currWord!=NULL){
    if(!strcmp(currWord->word,word)){
      *index=currWord->index ;
      return currWord ;
    }

    currWord=currWord->next ;
  }
  return NULL ;
}


int insert_word_in_hash(BoW bow,char* word){
  int index ;

  if(search_word_in_hash(bow->dict,word,&index)!=NULL) return index ;

  /*                  EMPTY BUCKET LIST                     */
  int hashIndex=hash_func(word,BOW_B_NUM);

  if(bow->dict[hashIndex].root==NULL){
    bow->dict[hashIndex].root=malloc(sizeof(struct Dictionary_Word)) ;
    bow->dict[hashIndex].root->next=NULL ;
    bow->dict[hashIndex].root->word=malloc(strlen(word)+1) ;
    strcpy(bow->dict[hashIndex].root->word,word) ;
    bow->dict[hashIndex].root->index=bow->dict_len ;

  }
  else{
    /*                  APPEND BUCKET LIST                     */
    Hashed_Word currWord=bow->dict[hashIndex].root ;

    while(currWord->next!=NULL){ // collision
      currWord=currWord->next ;
    }
    currWord->next=malloc(sizeof(struct Dictionary_Word)) ;
    currWord->next->next=NULL ;
    currWord->next->word=malloc(strlen(word)+1) ;
    strcpy(currWord->next->word,word) ;
    currWord->next->index=bow->dict_len ;

  }

  index=bow->dict_len ;
  bow->dict_len++ ;

  if(index>=bow->bow_len){    // expand bow vectors
    int i,j ;
    for(i=0 ; i<bow->filesNum ; i++){
      bow->values[i]=realloc(bow->values[i],sizeof(float)*(bow->bow_len+BOW_STEP));

      for(j=bow->bow_len ; j<(bow->bow_len+BOW_STEP) ; j++){
        bow->values[i][j]=0.0 ;
      }
    }

    bow->bow_len+=BOW_STEP ;
  }
  printf("%s %d\n",word,index );

  return index ;
}

void initialize_bow(Hash_For_Site SiteTable,BoW* bow,int filesNum,int siteBucketsNum,int idBucketsNum){
  int i,j ;
  char* value=NULL ;  // sentence / attr value
  char* token ;

  *bow=malloc(sizeof(struct Bag_Of_Words)) ;          // init Bag_Of_Words struct

  (*bow)->dict=malloc(BOW_B_NUM*sizeof(struct Words_Bucket)) ;
  (*bow)->dict_len=0 ;
  (*bow)->bow_len=BOW_STEP ;
  (*bow)->filesNum=filesNum ;

  (*bow)->full_id_array=malloc(filesNum*sizeof(char*)) ;

  for(i=0;i<BOW_B_NUM;i++){
    (*bow)->dict[i].root=NULL;
  }

  (*bow)->values=malloc(filesNum*sizeof(float*)) ;
  for(i=0 ; i<(*bow)->filesNum ; i++){
    (*bow)->values[i]=malloc((*bow)->bow_len*sizeof(float)) ;

    for(j=0 ; j<(*bow)->bow_len ; j++){
      (*bow)->values[i][j]=0.0 ;
    }
  }

  Hashed_Site tempSite ;    // temp vars for traversing all files
  Hashed_Id tempId ;
  specs tempSpecs ;
  str_list tempValue ;

  int vec_index=0 ;     // index of bow vector
  int word_index ;    // index of word

  for(i=0;i<siteBucketsNum;i++){
    tempSite=SiteTable[i].root ;

    while(tempSite!=NULL){
      for(j=0;j<idBucketsNum;j++){
        tempId=tempSite->Id_Hash_Array[j].root ;

        while(tempId!=NULL){

          (*bow)->full_id_array[vec_index]=malloc(strlen(tempId->full_id)+1) ;
          strcpy((*bow)->full_id_array[vec_index],tempId->full_id) ;

          tempSpecs=tempId->Specs ;

          while(tempSpecs!=NULL){
            // if not given attribute  continue ..

            tempValue=tempSpecs->values ;
            while(tempValue!=NULL){
              value=malloc(strlen(tempValue->str)+1) ;
              strcpy(value,tempValue->str) ;
              preprocess(value) ;
              // printf("%s\n",value );
              token=strtok(value," ") ;
              while(token){
                trim(token) ;

                if(strlen(token)>1){
                  // IF NOT STOPWORD //

                  word_index=insert_word_in_hash(*bow,token) ;
                  (*bow)->values[vec_index][word_index]++ ;
                }

                token=strtok(NULL," ") ;
              }

              free(value) ;
              tempValue=tempValue->next ;
            }

            tempSpecs=tempSpecs->next ;
          }

          vec_index++ ;
          tempId=tempId->next ;
        }
      }

      tempSite=tempSite->next ;
    }
  }
}
