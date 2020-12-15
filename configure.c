#include "model.h"

void test_print(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum){
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
            if(k==1) printf("%s: %s,%s\n",tempSite->site,tempId->full_id,tempComp->id );
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

//./CItest .json
int main(int argc, char** argv){
  int siteBucketsNum = 5 ;
  int idBucketsNum = 1 ;
  int i=0;

  Hash_For_Site site_hash_table=NULL;
  site_hash_table=malloc(siteBucketsNum*sizeof(struct Sites_Bucket));
  for(i=0;i<siteBucketsNum;i++){
    site_hash_table[i].root=NULL;
  }

  Hashed_Site currSite1=NULL;
  Hashed_Site currSite2=NULL;
  printf("%s\n","hmm1" );
  currSite1=insert_site_in_hash(site_hash_table,"www0.TestSite.com",siteBucketsNum,idBucketsNum);
  printf("%s\n","hmm2" );
  FILE* JSON_file=fopen("4233.json","r");
  printf("%s\n","hmm3" );
  insert_id_in_hash(currSite1->Id_Hash_Array,idBucketsNum,"www0.TestSite.com//4233",JSON_file,1);
  insert_id_in_hash(currSite1->Id_Hash_Array,idBucketsNum,"www0.TestSite.com//4233",JSON_file,2); // duplicate

  currSite2=insert_site_in_hash(site_hash_table,"www1.TestSiteG4.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite2->Id_Hash_Array,idBucketsNum,"www1.TestSiteG4.com//4233",JSON_file,3);

  test_print(site_hash_table,siteBucketsNum,idBucketsNum) ;

  /////////////////////////// ALLAGH KLIKAS ///////////////////////////
  Hashed_Id complex1=NULL;
  Hashed_Id complex2=NULL;
  complex tmp1=NULL;
  Hashed_Id tmp_complex=NULL;
  complex1=find_ID(site_hash_table,siteBucketsNum,idBucketsNum,"www0.TestSite.com//4233") ;
  complex2=find_ID(site_hash_table,siteBucketsNum,idBucketsNum,"www1.TestSiteG4.com//4233") ;
  complex1->Complex->tail->next=complex2->Complex->head ;
  complex1->Complex->tail=complex2->Complex->tail ;
  tmp1=complex2->Complex->head;
  while(tmp1!=NULL){
    if(strcmp("www1.TestSiteG4.com//4233",tmp1->id)!=0){
      tmp_complex=find_ID(site_hash_table,siteBucketsNum,idBucketsNum,tmp1->id);
      tmp_complex->Complex=complex1->Complex;
    }
    tmp1=tmp1->next;
  }
  free(complex2->Complex) ;
  complex2->Complex=complex1->Complex;
  /////////////////////////// ALLAGH KLIKAS ///////////////////////////

  test_print(site_hash_table,siteBucketsNum,idBucketsNum) ;
  return 0 ;
}
