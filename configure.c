#include "./logistic_regression/model.h"

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
  printf("%s\n","TEST MAIN" );

  int siteBucketsNum = 5 ;
  int idBucketsNum = 1 ;
  int i=0;

  Hash_For_Site SiteTable=NULL;
  SiteTable=malloc(siteBucketsNum*sizeof(struct Sites_Bucket));
  for(i=0;i<siteBucketsNum;i++){
    SiteTable[i].root=NULL;
  }

  /////////////////////////// EISAGWGH JSON FILES ///////////////////////////
  Hashed_Site currSite1=NULL;
  Hashed_Site currSite2=NULL;
  Hashed_Site currSite3=NULL;
  Hashed_Site currSite4=NULL;

  FILE* JSON_file1=fopen(".github/workflows/4233.json","r");
  FILE* JSON_file2=fopen(".github/workflows/4233.json","r");
  FILE* JSON_file3=fopen(".github/workflows/2.json","r");
  FILE* JSON_file4=fopen(".github/workflows/34.json","r");

  currSite1=insert_site_in_hash(SiteTable,"www0.TestSite.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite1->Id_Hash_Array,idBucketsNum,"www0.TestSite.com//4233",JSON_file1,0);

  currSite2=insert_site_in_hash(SiteTable,"www1.TestSiteG4.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite2->Id_Hash_Array,idBucketsNum,"www1.TestSiteG4.com//4233",JSON_file2,1);

  currSite3=insert_site_in_hash(SiteTable,"www1.TestSiteG4.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite2->Id_Hash_Array,idBucketsNum,"www1.TestSiteG4.com//2",JSON_file3,2);

  currSite4=insert_site_in_hash(SiteTable,"www1.TestSiteG4.com",siteBucketsNum,idBucketsNum);
  insert_id_in_hash(currSite2->Id_Hash_Array,idBucketsNum,"www1.TestSiteG4.com//34",JSON_file4,3);
  /////////////////////////// EISAGWGH JSON FILES ///////////////////////////

  printf("%s\n","inserted JSON files" );

  /////////////////////////// ALLAGH KLIKAS ///////////////////////////
  Hashed_Id id_entry1=NULL;
  Hashed_Id id_entry2=NULL;
  complex tmp1=NULL;
  Hashed_Id tmp_complex=NULL;

  id_entry1=find_ID(SiteTable,siteBucketsNum,idBucketsNum,"www0.TestSite.com//4233") ;
  id_entry2=find_ID(SiteTable,siteBucketsNum,idBucketsNum,"www1.TestSiteG4.com//4233") ;

  id_entry1->Complex->tail->next=id_entry2->Complex->head ;
  id_entry1->Complex->tail=id_entry2->Complex->tail ;
  tmp1=id_entry2->Complex->head;
  while(tmp1!=NULL){
    if(strcmp("www1.TestSiteG4.com//4233",tmp1->id)!=0){
      tmp_complex=find_ID(SiteTable,siteBucketsNum,idBucketsNum,tmp1->id);
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

  test_print(SiteTable,siteBucketsNum,idBucketsNum) ;

  printf("\n%s\n","Creating and initializing Bag of Words" );

  int bow_test,vec_test ;

  BoW bow=NULL ;

  initialize_bow(SiteTable,&bow,4,siteBucketsNum,idBucketsNum) ;

  for(vec_test=0 ; vec_test<100 ; vec_test++){
      printf("%lf ",bow->values[0][vec_test] );
  }

  printf("%s\n","making tf-idf" );

  bow_to_tf_idf(bow) ;

  for(vec_test=0 ; vec_test<100 ; vec_test++){
      printf("%lf ",bow->values[0][vec_test] );
  }

  FILE* Train,*Test,*Validation ;
  // Train=fopen("Train.csv","r") ;
  // Test=fopen("Test.csv","r") ;
  // Validation=fopen("Validation.csv","r") ;
  Train=fopen("Train.csv","w") ;
  Test=fopen("Test.csv","w") ;
  Validation=fopen("Validation.csv","w") ;

  fprintf(Train, "%s,%s,%s\n","left_spec_id","right_spec_id","match" );
  fprintf(Test, "%s,%s,%s\n","left_spec_id","right_spec_id","match" );
  fprintf(Validation, "%s,%s,%s\n","left_spec_id","right_spec_id","match" );

  train_test_val_split(SiteTable,siteBucketsNum,idBucketsNum,Train,Test,Validation) ;

  fclose(Train) ;
  fclose(Test) ;
  fclose(Validation) ;

  Model Linear_Rigression ;
  initialize_model(&Linear_Rigression,bow->dict_len,0.5,0.1) ;

  printf("\n%s\n","weights before train :" );
  for(i=0 ; i<bow->dict_len ;i++){
    printf("%lf \n",Linear_Rigression->weights[i] );
  }

  fit(Linear_Rigression,bow,"Train.csv",SiteTable,siteBucketsNum,idBucketsNum) ;

  printf("\n%s\n","weights after train :" );
  for(i=0 ; i<bow->dict_len ;i++){
    printf("%lf \n",Linear_Rigression->weights[i] );
  }

  printf("\n\nPredicting Validation set\n" );

  predict(Linear_Rigression,bow,"Validation.csv",SiteTable,siteBucketsNum,idBucketsNum) ;

  printf("%s\n","Testing data .." );

  predict(Linear_Rigression,bow,"Test.csv",SiteTable,siteBucketsNum,idBucketsNum) ;

  delete(SiteTable,siteBucketsNum,idBucketsNum) ;         // free all structs
  delete_model(&Linear_Rigression) ;
  delete_bow(&bow) ;


  return 0 ;
}
