#include "bag_of_words.h"
#include "hash.h"
#include <ctype.h>

Hashed_Word search_word_in_hash(Hash_For_Word WordTable,char* word,int bucketsNum,int* index){
  int hashIndex=hash_func(word,bucketsNum);

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


int insert_word_in_hash(BoW bow,char* word,int bucketsNum){
  int index ;
  if(search_id_in_hash(bow->dict,full_id,bucketsNum,&index)!=NULL) return index ;

  /*                  EMPTY BUCKET LIST                     */
  int hashIndex=hash_func(word,bucketsNum);

  if(bow->dict[hashIndex].root==NULL){
    bow->dict[hashIndex].root=malloc(sizeof(struct Dictionary_Word)) ;
    bow->dict[hashIndex].root->next=NULL ;
    bow->dict[hashIndex].root->word=malloc(strlen(word)+1) ;
    strcpy(bow->dict[hashIndex].root->word,word) ;

    bow->dict_len++ ;
    index=bow->dict_len-1 ;
    return index ;
  }

  /*                  APPEND BUCKET LIST                     */
  Hashed_Word currWord=bow->dict[hashIndex].root ;

  while(currWord->next!=NULL){ // collision
    currWord=curWord->next ;
  }
  currWord->next=malloc(sizeof(struct Dictionary_Word)) ;
  currWord->next->next=NULL ;
  currWord->next->word=malloc(strlen(word)+1) ;
  strcpy(currWord->next->word,word) ;

  bow->dict_len++ ;
  index=bow->dict_len-1 ;
  return index ;
}

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

BoW initialize_bow(Hash_For_Site SiteTable,BoW bow,int fileNum,int siteBucketsNum,int idBucketsNum){
  int i,j,k=0 ;
  char* value=NULL ;
  char* token ;

  bow=malloc(sizeof(struct Bag_Of_Words)) ;

  bow->dict=malloc(BOW_B_NUM*sizeof(struct Words_Bucket)) ;
  bow->bow_len=BOW_STEP ;

  for(i=0;i<BOW_B_NUM;i++){
    bow->dict[i].root=NULL;
  }

  bow->values=malloc(fileNum*sizeof(float*)) ;
  for(i=0 ; i<fileNum ; i++){
    values[i]=malloc(bow_len*sizeof(float)) ;

    for(j=0 ; j<bow_len ; j++){
      values[i][j]=0.0 ;
    }
  }

  Hashed_Site tempSite ;
  Hashed_Id tempId ;
  specs tempSpecs ;
  str_list tempValue ;


  for(i=0;i<siteBucketsNum;i++){
    tempSite=SiteTable[i].root ;

    while(tempSite!=NULL){
      for(j=0;j<idBucketsNum;j++){
        tempId=tempSite->Id_Hash_Array[j].root ;

        while(tempId!=NULL){
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
                  printf("%s\n",token );
                }

                token=strtok(NULL," ") ;
              }

              free(value) ;
              tempValue=tempValue->next ;
            }

            tempSpecs=tempSpecs->next ;
          }

          tempId=tempId->next ;
        }
      }

      tempSite=tempSite->next ;
    }
  }
}
