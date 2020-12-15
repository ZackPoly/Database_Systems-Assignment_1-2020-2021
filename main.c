#include "model.h"
// #include "bag_of_words.h"


//./main Datasets 2013_camera_specs < >.csv
int main(int argc, char** argv){

  DIR* DD;
  struct dirent* main_dir;

  DD=opendir(argv[1]);

  if(DD==NULL){
    printf("Can't open directory!\n");
    return 1;
  }

  char fullPath[100];
  getcwd(fullPath,100) ;
  strcat(fullPath,"/");
  strcat(fullPath,argv[1]);
  chdir(fullPath);
  char* dataset_path=NULL;

  while((main_dir=readdir(DD))!=NULL){

    if(strcmp(main_dir->d_name,".")==0){
      continue;
    }

    if(strcmp(main_dir->d_name,"..")==0){
      continue;
    }

    if(!strcmp(main_dir->d_name,argv[2])){
      dataset_path=malloc(strlen(main_dir->d_name)+2);          //dataset_path=camera_specs path
      strcpy(dataset_path,main_dir->d_name);
      //strcat(dataset_path,"/");

      break ;
    }
  }
  closedir(DD);

  DIR* FD;
  DIR* SD;

  struct dirent* in_file;
  struct dirent* d_files;

  FILE* JSON_file;
  FILE* DW;

  char* data=NULL;
  int length;

  char* site=NULL;
  char* id=NULL;
  char* full_id=NULL;
  char* tmp=NULL;
  Hashed_Site currSite=NULL;

  int siteBucketsNum=97;
  int idBucketsNum=197;

  char* path=NULL;
  char* path_ff=NULL;

  FD=opendir(dataset_path);                                     //open camera_specs/ directory
  free(dataset_path);

  if (FD==NULL){                                                //check if directory is open
    printf("Can't open directory!\n");
    return 1;
  }

  Hash_For_Site site_hash_table=NULL;

  site_hash_table=malloc(siteBucketsNum*sizeof(struct Sites_Bucket));

  int i=0;
  int filesNum=0;

  for(i=0;i<siteBucketsNum;i++){
    site_hash_table[i].root=NULL;
  }


  while((in_file=readdir(FD))!=NULL){                           //read all subdirectories (sites)

    if(strcmp(in_file->d_name,".")==0){
      continue;
    }

    if(strcmp(in_file->d_name,"..")==0){
      continue;
    }

    site=malloc(strlen(in_file->d_name)+1);
    strcpy(site,in_file->d_name);

    currSite=insert_site_in_hash(site_hash_table,site,siteBucketsNum,idBucketsNum);

    printf("Subdirectory: %s\n",in_file->d_name);


    path=malloc(strlen(argv[2])+strlen(in_file->d_name)+2);
    strcpy(path,argv[2]) ;
    strcat(path,"/");
    strcat(path,in_file->d_name);
    SD=opendir(path);                                           //open subdirectory


    if(SD==NULL){                                               //check if it is open
      printf("Can't open subdirectory!\n");
      return 1;
    }

    while((d_files=readdir(SD))!=NULL){

      if(strcmp(d_files->d_name,".")==0){
        continue;
      }

      if(strcmp(d_files->d_name,"..")==0){
        continue;
      }


      tmp=malloc(strlen(d_files->d_name)+1);
      strcpy(tmp,d_files->d_name);

      id=strtok(tmp,".json");

      full_id=malloc(strlen(site)+strlen(id)+3);
      strcpy(full_id,site);
      strcat(full_id,"//");
      strcat(full_id,id);

      path_ff=malloc(strlen(path)+strlen(d_files->d_name)+2);
      strcpy(path_ff,path);
      strcat(path_ff,"/");
      strcat(path_ff,d_files->d_name);

      //printf("Path for file is : %s \n ",path_ff);

      JSON_file=fopen(path_ff,"r");                             //open JSON file

      if(JSON_file==NULL){                                      //check if it is open

        printf("Can't open JSON file! \n");
        return 1;

      }

      // if(filesNum<1){
      //   insert_id_in_hash(currSite->Id_Hash_Array,idBucketsNum,full_id,JSON_file);
      //
      //   filesNum++ ;
      // }
                                                                // insert full_id in struct
      insert_id_in_hash(currSite->Id_Hash_Array,idBucketsNum,full_id,JSON_file,filesNum);

      filesNum++ ;

      fclose(JSON_file);
      JSON_file=NULL;

      free(data);
      data=NULL;

      free(path_ff);
      path_ff=NULL;

      //free(id);
      id=NULL;
      free(full_id);
      full_id=NULL;
      free(tmp);
      tmp=NULL;

    }

    closedir(SD);
    SD=NULL;
    free(path);
    path=NULL;
    free(site);
    site=NULL;

  }

  // printf("%d\n",filesNum );

  closedir(FD);
  FD=NULL;


  /*-------------- INITIALIZE BAG OF WORDS + CONVERT TO TF-IDF --------------*/
  int bow_test,vec_test ;

  BoW bow=NULL ;

  initialize_bow(site_hash_table,&bow,filesNum,siteBucketsNum,idBucketsNum) ;

  for(vec_test=0 ; vec_test<100 ; vec_test++){
      printf("%lf ",bow->values[0][vec_test] );
  }

  printf("%s\n","making tf-idf" );

  bow_to_tf_idf(bow) ;

  for(vec_test=0 ; vec_test<100 ; vec_test++){
      printf("%lf ",bow->values[0][vec_test] );
  }
  /*-------------- INITIALIZE BAG OF WORDS + CONVERT TO TF-IDF --------------*/


  /*----------------------- CREATE COMPLEXES AND SETS -----------------------*/
  // DW=fopen(argv[3],"r");
  //
  // char* line=NULL;
  // char* full_id_1=NULL;
  // char* full_id_2=NULL;
  // char* match=NULL;
  // char* cid=NULL;
  //
  // Hashed_Id id_entry1=NULL;
  // Hashed_Id id_entry2=NULL;
  // complex tmp1=NULL;
  // Hashed_Id tmp_complex=NULL;
  //
  // size_t line_buf_size=0;
  // int line_size=0;
  // int v=0;
  //
  // line_size=getline(&line,&line_buf_size,DW);                   // first line not a pair
  //
  // while(line_size>0){                                           // loop for each line
  //
  //   if(v==0){                                                   // skip first line
  //     line_size=getline(&line,&line_buf_size,DW);               // get next line
  //     v=1;
  //     continue;
  //   }
  //
  //
  //   full_id_1=strtok(line,",");
  //   full_id_2=strtok(NULL,",");
  //   match=strtok(NULL,",");
  //   match=strtok(match,"\n");
  //
  //   if(strcmp(match,"1")==0 && strcmp(full_id_1,full_id_2)!=0){                //if the products are same
  //
  //
  //     id_entry1=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_1);
  //     id_entry2=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_2);
  //
  //     if((id_entry1->Complex!=id_entry2->Complex) && !search_negative(id_entry1->Complex,id_entry2->Complex)){
  //
  //       id_entry1->Complex->tail->next=id_entry2->Complex->head ;   // update complex of full_id1
  //       id_entry1->Complex->tail=id_entry2->Complex->tail ;
  //
  //       tmp1=id_entry2->Complex->head;
  //
  //       while(tmp1!=NULL){                                        // make complex of all full_id in complex 2
  //                                                                 // be the same as complex 1
  //
  //         if(strcmp(full_id_2,tmp1->id)!=0){
  //
  //           tmp_complex=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,tmp1->id);
  //           tmp_complex->Complex=id_entry1->Complex;
  //         }
  //
  //         tmp1=tmp1->next;
  //
  //       }
  //
  //       change_negatives(id_entry1->Complex,id_entry2->Complex) ;
  //
  //       // search_neg_corr(site_hash_table,siteBucketsNum,idBucketsNum,id_entry2->Complex) ;
  //
  //       delete_negatives(id_entry2->Complex) ;
  //       free(id_entry2->Complex) ;
  //       id_entry2->Complex=NULL ;
  //       id_entry2->Complex=id_entry1->Complex;
  //     }
  //
  //
  //   }
  //   else if(strcmp(match,"0")==0 && strcmp(full_id_1,full_id_2)!=0){
  //
  //
  //     id_entry1=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_1);
  //     id_entry2=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_2);
  //
  //     if(id_entry1->Complex!=id_entry2->Complex)
  //       append_negative(id_entry1->Complex,id_entry2->Complex) ;
  //   }
  //
  //   line_size=getline(&line,&line_buf_size,DW);                 // get next line
  //
  // }
  // free(line) ;
  // fclose(DW) ;
  /*----------------------- CREATE COMPLEXES AND SETS -----------------------*/

  chdir("..") ;

  // FILE* Output;                                                 // create csv for output
  // Output=fopen("Output.csv","w") ;
  // fprintf(Output, "%s,%s\n","left_spec_id","right_spec_id" );
  // print_complexes(site_hash_table,siteBucketsNum,idBucketsNum,Output) ;
  // //print result in a new csv file
  //
  // fclose(Output) ;

  FILE* Train,*Test,*Validation ;
  Train=fopen("Train.csv","r") ;
  Test=fopen("Test.csv","r") ;
  Validation=fopen("Validatio.csv","r") ;

  // fprintf(Train, "%s,%s,%s\n","left_spec_id","right_spec_id","match" );
  // fprintf(Test, "%s,%s,%s\n","left_spec_id","right_spec_id","match" );
  // fprintf(Validation, "%s,%s,%s\n","left_spec_id","right_spec_id","match" );

  // train_test_val_split(site_hash_table,siteBucketsNum,idBucketsNum,Train,Test,Validation) ;

  fclose(Train) ;
  fclose(Test) ;
  fclose(Validation) ;

  delete(site_hash_table,siteBucketsNum,idBucketsNum) ;         // free all structs
  return 0;

}
