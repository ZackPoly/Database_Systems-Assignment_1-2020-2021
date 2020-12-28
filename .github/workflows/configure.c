#include "../../logistic_regression/model.h"

void test_print(Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum){
  Hashed_Site tempSite ;
  Hashed_Id tempId ;
  Hashed_Id compId ;
  complex tempComp ;
  neg_corr tempNeg ;
  int i,j,k=0 ;

  for(i=0;i<siteBucketsNum;i++){
    tempSite=SiteTable[i].root ;

    while(tempSite!=NULL){
      for(j=0;j<idBucketsNum;j++){
        tempId=tempSite->Id_Hash_Array[j].root ;

        while(tempId!=NULL){

          tempComp=tempId->Complex->head ;

          while(tempComp!=NULL){
            if(k==1){
              printf("%s,%s,%s\n",tempId->full_id,tempComp->id,"1" );
            }
            if(!strcmp(tempId->full_id,tempComp->id)) k=1 ;

            tempComp=tempComp->next ;
          }
          k=0 ;

          tempNeg=tempId->Complex->head_neg ;
          while (tempNeg!=NULL) {

            tempComp=tempNeg->corr->head ;

            while(tempComp!=NULL){

              compId=find_ID(SiteTable,siteBucketsNum,idBucketsNum,tempComp->id) ;

              if(compId->tf_idf_index>tempId->tf_idf_index){
                printf("%s,%s,%s\n",tempId->full_id,tempComp->id,"0" );
              }

              tempComp=tempComp->next ;
            }

            tempNeg=tempNeg->next ;
          }

          tempId=tempId->next ;
        }
      }

      tempSite=tempSite->next ;
    }
  }
}

// ./configure
int main(int argc, char** argv){
  int siteBucketsNum = 5 ;
  int idBucketsNum = 1 ;
  int i=0;

  Hash_For_Site site_hash_table=NULL;
  site_hash_table=malloc(siteBucketsNum*sizeof(struct Sites_Bucket));
  for(i=0;i<siteBucketsNum;i++){
    site_hash_table[i].root=NULL;
  }

  /////////////////////////// EISAGWGH JSON FILES ///////////////////////////
  Hashed_Site currSite1=NULL;
  Hashed_Site currSite2=NULL;

  currSite1=insert_site_in_hash(site_hash_table,"www0.TestSite.com",siteBucketsNum,idBucketsNum);

  FILE* JSON_file=fopen("4233.json","r");

  insert_id_in_hash(currSite1->Id_Hash_Array,idBucketsNum,"www0.TestSite.com//4233",JSON_file,1);
  insert_id_in_hash(currSite1->Id_Hash_Array,idBucketsNum,"www0.TestSite.com//4233",JSON_file,2); // duplicate

  fclose(JSON_file);
  JSON_file=fopen("4233.json","r");

  currSite2=insert_site_in_hash(site_hash_table,"www1.TestSiteG4.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite2->Id_Hash_Array,idBucketsNum,"www1.TestSiteG4.com//4233",JSON_file,3);

  fclose(JSON_file);
  JSON_file=fopen("2.json","r");

  currSite2=insert_site_in_hash(site_hash_table,"www1.TestSiteG4.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite2->Id_Hash_Array,idBucketsNum,"www1.TestSiteG4.com//2",JSON_file,4);

  fclose(JSON_file);
  JSON_file=fopen("34.json","r");

  currSite2=insert_site_in_hash(site_hash_table,"www1.TestSiteG4.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite2->Id_Hash_Array,idBucketsNum,"www1.TestSiteG4.com//34",JSON_file,5);
  /////////////////////////// EISAGWGH JSON FILES ///////////////////////////

  /////////////////////////// ALLAGH KLIKAS ///////////////////////////
  Hashed_Id id_entry1=NULL;
  Hashed_Id id_entry2=NULL;
  complex tmp1=NULL;
  Hashed_Id tmp_complex=NULL;
  id_entry1=find_ID(site_hash_table,siteBucketsNum,idBucketsNum,"www0.TestSite.com//4233") ;
  id_entry2=find_ID(site_hash_table,siteBucketsNum,idBucketsNum,"www1.TestSiteG4.com//4233") ;
  id_entry1->Complex->tail->next=id_entry2->Complex->head ;
  id_entry1->Complex->tail=id_entry2->Complex->tail ;
  tmp1=id_entry2->Complex->head;
  while(tmp1!=NULL){
    if(strcmp("www1.TestSiteG4.com//4233",tmp1->id)!=0){
      tmp_complex=find_ID(site_hash_table,siteBucketsNum,idBucketsNum,tmp1->id);
      tmp_complex->Complex=id_entry1->Complex;
    }
    tmp1=tmp1->next;
  }
  free(id_entry2->Complex) ;
  id_entry2->Complex=id_entry1->Complex;


  id_entry1=find_ID(SiteTable,siteBucketsNum,idBucketsNum,"www0.TestSite.com//4233");
  id_entry2=find_ID(SiteTable,siteBucketsNum,idBucketsNum,"www1.TestSiteG4.com//2");

  if(id_entry1->Complex!=id_entry2->Complex)
    append_negative(id_entry1->Complex,id_entry2->Complex) ;


  id_entry1=find_ID(SiteTable,siteBucketsNum,idBucketsNum,"www1.TestSiteG4.com//4233");
  id_entry2=find_ID(SiteTable,siteBucketsNum,idBucketsNum,"www1.TestSiteG4.com//34");

  if(id_entry1->Complex!=id_entry2->Complex)
    append_negative(id_entry1->Complex,id_entry2->Complex) ;
  /////////////////////////// ALLAGH KLIKAS ///////////////////////////

  test_print(site_hash_table,siteBucketsNum,idBucketsNum) ;
  return 0 ;
}
