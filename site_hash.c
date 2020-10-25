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

  /*                  EMPTY BUCKET LIST                     */
  if(SiteTable[hashIndex].root==NULL){
    SiteTable[hashIndex].root=malloc(sizeof(struct Hash_For_Site_Entry)) ;
    SiteTable[hashIndex].root->next=NULL ;
    SiteTable[hashIndex].root->site=malloc(strlen(site)+1) ;
    strcpy(SiteTable[hashIndex].root->site,site) ;
    SiteTable[hashIndex].root->Id_Hash_Array=malloc(idBucketsNum*sizeof(struct Ids_Bucket)) ;

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

  return currSite->next ;
}

Hashed_Id search_complex(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum,char* full_id){

  char str[80] ;
  strcpy(str,full_id) ;
  const char s[2]="//";
  char* site;
  site=strtok(str,s);

  /* get site and site bucket node*/
  Hashed_Site currSite=search_site_in_hash(SiteTable,site,siteBucketsNum) ;
  if(currSite==NULL) return NULL ;

  /* get bucket node of full_id */
  Hashed_Id currId=search_id_in_hash(currSite->Id_Hash_Array,full_id,idBucketsNum) ;
  if(currSite==NULL) return NULL ;      // that shouldnt happen

  // return &(currId->Complex) ;
  return currId ;
}

// TODO
// Test with NULL and not NULL site
void get_site_from_id(char* site,const char* full_id){
  char str[80] ;
  strcpy(str,full_id) ;
  const char s[2]="//";

  site=strtok(str,s);
}
