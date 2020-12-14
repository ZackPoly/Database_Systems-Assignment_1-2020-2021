#include "site_hash.h"

Hashed_Site search_site_in_hash(Hash_For_Site SiteTable,char* site,int bucketsNum){
  int hashIndex=hash_func(site,bucketsNum);

  Hashed_Site currSite=SiteTable[hashIndex].root ;
  while(currSite!=NULL){
    if(!strcmp(currSite->site,site)) return currSite ;

    currSite=currSite->next ;
  }
  return NULL ;
}

Hashed_Site insert_site_in_hash(Hash_For_Site SiteTable,char* site,int siteBucketsNum,int idBucketsNum){

  /*                     DUPLICATE ID REJECTED                     */
  if(search_site_in_hash(SiteTable,site,siteBucketsNum)!=NULL) return NULL ;

  int hashIndex=hash_func(site,siteBucketsNum);
  int i ;

  /*                  EMPTY BUCKET LIST                     */
  if(SiteTable[hashIndex].root==NULL){
    SiteTable[hashIndex].root=malloc(sizeof(struct Hash_For_Site_Entry)) ;
    SiteTable[hashIndex].root->next=NULL ;
    SiteTable[hashIndex].root->site=malloc(strlen(site)+1) ;
    strcpy(SiteTable[hashIndex].root->site,site) ;
    SiteTable[hashIndex].root->Id_Hash_Array=malloc(idBucketsNum*sizeof(struct Ids_Bucket)) ;
    for(i=0;i<idBucketsNum;i++){
      SiteTable[hashIndex].root->Id_Hash_Array[i].root=NULL ;
    }

    return SiteTable[hashIndex].root ;
  }

  /*                  APPEND BUCKET LIST                     */
  Hashed_Site currSite=SiteTable[hashIndex].root ;

  while(currSite->next!=NULL){ // collision
    currSite=currSite->next ;
  }
  currSite->next=malloc(sizeof(struct Hash_For_Site_Entry)) ;
  currSite->next->next=NULL ;
  currSite->next->site=malloc(strlen(site)+1) ;
  strcpy(currSite->next->site,site) ;
  currSite->next->Id_Hash_Array=malloc(idBucketsNum*sizeof(struct Ids_Bucket)) ;
  for(i=0;i<idBucketsNum;i++){
    currSite->next->Id_Hash_Array[i].root=NULL ;
  }

  return currSite->next ;
}

Hashed_Id search_complex(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum,char* full_id){
  char* str=malloc(strlen(full_id)+1) ;
  strcpy(str,full_id) ;
  const char s[3]="//";
  char* site=NULL;
  site=strtok(str,s);

  /* get site and site bucket node*/
  Hashed_Site currSite=search_site_in_hash(SiteTable,site,siteBucketsNum) ;
  if(currSite==NULL) return NULL ;

  /* get bucket node of full_id */
  Hashed_Id currId=search_id_in_hash(currSite->Id_Hash_Array,full_id,idBucketsNum) ;
  if(currSite==NULL) return NULL ;      // that shouldnt happen

  free(str) ;
  // return &(currId->Complex) ;
  return currId ;
}

// search_neg_corr(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum,comp_head Complex)


void delete(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum){

  int i=0,j=0;
  Hashed_Site tmp_site=NULL;
  Hashed_Site tmp_site2=NULL;
  Hash_For_Id tmp_id_bucket=NULL;
  Hashed_Id tmp_id=NULL;
  Hashed_Id tmp_id2=NULL;
  specs tmp_spec=NULL;
  specs tmp_spec2=NULL;

  for(i=0;i<siteBucketsNum;i++){                         //delete each site_bucket

    tmp_site=SiteTable[i].root;

    while(tmp_site!=NULL){                              //delete each site from a site_bucket

      tmp_site2=tmp_site->next;

      free(tmp_site->site);
      tmp_site->site=NULL;

      tmp_id_bucket=tmp_site->Id_Hash_Array;

      for(j=0;j<idBucketsNum;j++){                    //delete each id_bucket from a site

        tmp_id=tmp_id_bucket[j].root;

        while(tmp_id!=NULL){                           //delete each id from an id_bucket

          tmp_id2=tmp_id->next;

          delete_specs_list(tmp_id) ;      //delete specs list

          delete_complex_node(tmp_id);        //delete complex node from the complex

          free(tmp_id->full_id);
          tmp_id->full_id=NULL;
          free(tmp_id);
          tmp_id=tmp_id2;

        }


      }

      free(tmp_id_bucket);
      tmp_id_bucket=NULL;

      free(tmp_site);
      tmp_site=tmp_site2;

    }

  }

  free(SiteTable);
  SiteTable=NULL;

}


void print_complexes(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum,FILE* Output){
  Hashed_Site tempSite ;
  Hashed_Id tempId ;
  complex tempComp ;
  int i,j,k=0 ;

  for(i=0;i<siteBucketsNum;i++){
    tempSite=SiteTable[i].root ;

    while(tempSite!=NULL){
      for(j=0;j<idBucketsNum;j++){
        tempId=tempSite->Id_Hash_Array[j].root ;

        while(tempId!=NULL){
          tempComp=tempId->Complex->head ;
          while(tempComp!=NULL){
            if(k==1) fprintf(Output, "%s,%s\n",tempId->full_id,tempComp->id );
            if(!strcmp(tempId->full_id,tempComp->id)) k=1 ;

            tempComp=tempComp->next ;
          }
          k=0 ;

          tempId=tempId->next ;
        }
      }

      tempSite=tempSite->next ;
    }
  }
}

void train_test_val_split(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum,FILE* Train,FILE* Test,FILE* Validation){
  Hashed_Site tempSite ;
  Hashed_Id tempId ;
  Hashed_Id compId ;
  complex tempComp ;
  neg_corr tempNeg ;
  int i,j,k=0 ;

  srand(time(NULL)) ;
  int fileSelect ;

  for(i=0;i<siteBucketsNum;i++){
    tempSite=SiteTable[i].root ;

    while(tempSite!=NULL){
      for(j=0;j<idBucketsNum;j++){
        tempId=tempSite->Id_Hash_Array[j].root ;

        while(tempId!=NULL){

          tempComp=tempId->Complex->head ;

          while(tempComp!=NULL){
            if(k==1){
              fileSelect=rand()%101 ;
              if(fileSelect<60){
                fprintf(Train, "%s,%s,%s\n",tempId->full_id,tempComp->id,"1" );
              }
              else if(fileSelect>=60 && fileSelect<80){
                fprintf(Test, "%s,%s,%s\n",tempId->full_id,tempComp->id,"1" );
              }
              else{
                fprintf(Validation, "%s,%s,%s\n",tempId->full_id,tempComp->id,"1" );
              }

            }
            if(!strcmp(tempId->full_id,tempComp->id)) k=1 ;

            tempComp=tempComp->next ;
          }
          k=0 ;

          // tempNeg=tempId->Complex->head_neg ;
          // while (tempNeg!=NULL) {
          //
          //   tempComp=tempNeg->corr->head ;
          //
          //   while(tempComp!=NULL){
          //
          //     compId=search_id_in_hash(tempSite->Id_Hash_Array,tempComp->id,idBucketsNum) ;
          //
          //     if(compId->tf_idf_index>tempId->tf_idf_index){
          //
          //       fileSelect=rand()%101 ;
          //       if(fileSelect<60){
          //         fprintf(Train, "%s,%s,%s\n",tempId->full_id,tempComp->id,"0" );
          //       }
          //       else if(fileSelect>=60 && fileSelect<80){
          //         fprintf(Test, "%s,%s,%s\n",tempId->full_id,tempComp->id,"0" );
          //       }
          //       else{
          //         fprintf(Validation, "%s,%s,%s\n",tempId->full_id,tempComp->id,"0" );
          //       }
          //     }
          //
          //     tempComp=tempComp->next ;
          //   }
          //
          //   tempNeg=tempNeg->next ;
          // }

          tempId=tempId->next ;
        }
      }

      tempSite=tempSite->next ;
    }
  }
}
