#include "bag_of_words.h"
#include "hash.h"

const char * stopwords[] = {
    "a","able","about","across","after","all","almost","also","am","among","an",
    "and","any","are","as","at","be","because","been","but","by","can","cannot",
    "could","dear","did","do","does","either","else","ever","every","for","from",
    "get","got","had","has","have","he","her","hers","him","his","how","however",
    "i","if","in","into","is","it","its","just","least","let","like","likely",
    "may","me","might","most","must","my","neither","no","nor","not","of","off",
    "often","on","only","or","other","our","own","rather","said","say","says",
    "she","should","since","so","some","than","that","the","their","them","then",
    "there","these","they","this","tis","to","too","twas","us","wants","was",
    "we","were","what","when","where","which","while","who","whom","why","will",
    "with","would","yet","you","your",
};

#define n_stop_words (sizeof (stopwords) / sizeof (const char *))

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

int is_stopword(char* word){
  int a = 0 ;
  int b = n_stop_words ;
  int c ;
  int comp ;

  while(a < b){
    c = (b + a) / 2 ;

    comp = strcmp(word, stopwords[c]) ;
    if(comp == 0)
      return 1 ;
    else if(comp < 0)
      b = c - 1 ;
    else if(comp > 0)
      a = c + 1 ;

  }

  return 0 ;
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
      bow->values[i]=realloc(bow->values[i],sizeof(double)*(bow->bow_len+BOW_STEP));

      for(j=bow->bow_len ; j<(bow->bow_len+BOW_STEP) ; j++){
        bow->values[i][j]=0.0 ;
      }
    }

    bow->bow_len+=BOW_STEP ;
  }
  // printf("%s %d\n",word,index );

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


  (*bow)->filesLen=malloc(filesNum*sizeof(int)) ;
  for(i=0 ; i<filesNum ; i++){
    (*bow)->filesLen[i]=0 ;
  }

  for(i=0;i<BOW_B_NUM;i++){
    (*bow)->dict[i].root=NULL;
  }

  (*bow)->values=malloc(filesNum*sizeof(double*)) ;
  for(i=0 ; i<(*bow)->filesNum ; i++){
    (*bow)->values[i]=malloc((*bow)->bow_len*sizeof(double)) ;

    for(j=0 ; j<(*bow)->bow_len ; j++){
      (*bow)->values[i][j]=0.0 ;
    }
  }

  Hashed_Site tempSite ;    // temp vars for traversing all files
  Hashed_Id tempId ;
  specs tempSpecs ;
  str_list tempValue ;

  int vec_index=0 ;         // index of bow vector
  int word_index ;          // index of word

  int word_count = 0 ;      // counts words in one file

  for(i=0;i<siteBucketsNum;i++){
    tempSite=SiteTable[i].root ;

    while(tempSite!=NULL){
      for(j=0;j<idBucketsNum;j++){
        tempId=tempSite->Id_Hash_Array[j].root ;

        while(tempId!=NULL){

          // word_count = 0 ;

          // (*bow)->full_id_array[vec_index]=malloc(strlen(tempId->full_id)+1) ;
          // strcpy((*bow)->full_id_array[vec_index],tempId->full_id) ;

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

                if(!is_stopword(token) && strlen(token)<9){
                  (*bow)->filesLen[vec_index]++ ;

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

  (*bow)->wordsFileCount=malloc((*bow)->dict_len*sizeof(int)) ;
  for(i=0 ; i<(*bow)->dict_len ; i++){
    (*bow)->wordsFileCount[i]=0 ;
  }
}

void delete_unimportant_words(BoW bow){                                         //delete words that appear only once

  // Hash_For_Word  tmp_dict_bucket=NULL;
  Hashed_Word    tmp_word1=NULL;
  Hashed_Word    tmp_word2=NULL;

  int i=0;

  for(i=0;i<BOW_B_NUM;i++){

    // *bow->dict[i]=bow->dict[i];

    tmp_word1=bow->dict[i].root;
    tmp_word2=tmp_word1;

    while(tmp_word1!=NULL){

      if(bow->wordsFileCount[tmp_word1->index]==1){
        printf("%s\n",tmp_word1->word );

        if(tmp_word1==bow->dict[i].root){
          bow->dict[i].root=bow->dict[i].root->next;

          free(tmp_word1->word);
          free(tmp_word1);

          tmp_word1=bow->dict[i].root;
          tmp_word2=tmp_word1;

          bow->dict_len--;

          continue;
        }

        tmp_word2->next=tmp_word1->next;

        free(tmp_word1->word);
        free(tmp_word1);

        tmp_word1=tmp_word2->next;

        bow->dict_len--;

        continue;

      }

      if(tmp_word1!=tmp_word2){
        tmp_word2=tmp_word2->next;
      }
      tmp_word1=tmp_word1->next;
    }

  }

  return;

}

void bow_to_tf_idf(BoW bow){
  int i,j ;

  for(i=0 ; i<bow->filesNum ; i++){

    for(j=0 ; j<bow->dict_len ; j++){
      if(bow->values[i][j]){
        bow->wordsFileCount[j]++ ;

        bow->values[i][j]=bow->values[i][j] / bow->filesLen[i] ;
      }

    }
  }

  // drop words with wordsFileCount=1
  delete_unimportant_words(bow) ;

  printf("%s\n","done with tf" );

  double n=bow->filesNum*1.0 ;
  Hashed_Word tempWord ;

  for(j=0 ; j<BOW_B_NUM ; j++){
    tempWord=(bow->dict[j]).root ;

    while(tempWord!=NULL){
      for(i=0 ; i<bow->filesNum ; i++){
        if(bow->values[i][tempWord->index])
          bow->values[i][tempWord->index]=bow->values[i][tempWord->index] * log(n / bow->wordsFileCount[tempWord->index]) ;
      }

      tempWord=tempWord->next ;
    }
  }
}
