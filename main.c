#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>


#include "site_hash.h"
// first arg DatasetX/ and second .csv file

int main(int argc, char** argv){


  char* dataset_path=NULL;                      //change dir to open camera_specs
  dataset_path=malloc(strlen(argv[1])+1);
  strcpy(dataset_path,argv[1]);
  strcat(dataset_path,"/");
  chdir(dataset_path);

  DIR* DD;
  struct dirent* main_dir;

  DD=opendir(dataset_path);

  if(DD=NULL){
    printf("Can't open directory!\n");
    return 1;
  }

  free(dataset_path);

  while((main_dir=readdir(DD))!=NULL){

    if(strcmp(main_dir->d_name,".")==0){
      continue;
    }

    if(strcmp(main_dir->d_name,"..")==0){
      continue;
    }

    dataset_path=malloc(strlen(main_dir->d_name)+2);        //dataset_path=camera_specs path
    strcpy(dataset_path,main_dir->d_name);
    strcat(dataset_path,"/");
  }

  DIR* FD;
  DIR* SD;

  struct dirent* in_file;
  struct dirent* d_files;

  FILE* JSON_file;
  FILE* DW;

  char* data;
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

  FD=opendir(dataset_path);                                      //open camera_specs/ directory

  if (FD==NULL){                                            //check if directory is open
    printf("Can't open directory!\n");
    return 1;
  }


  Hash_For_Site site_hash_table=NULL;

  site_hash_table=malloc(siteBucketsNum*sizeof(struct Sites_Bucket));

  int i=0;

  for(i=0;i<siteBucketsNum;i++){
    site_hash_table[i].root=NULL;
  }


  while((in_file=readdir(FD))!=NULL){                       //read all subdirectories (sites)

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


    path=malloc(strlen(argv[1])+strlen(in_file->d_name)+2);
    strcpy(path,argv[1]);
    strcat(path,in_file->d_name);
    strcat(path,"/");
    SD=opendir(path);                                         //open subdirectory


    if(SD==NULL){                                            //check if it is open
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

      path_ff=malloc(strlen(path)+strlen(d_files->d_name)+1);
      strcpy(path_ff,path);
      strcat(path_ff,d_files->d_name);

      printf("Path for file is : %s \n ",path_ff);

      JSON_file=fopen(path_ff,"r");             //open JSON file

      if(JSON_file==NULL){                              //check if it is open

        printf("Can't open JSON file! \n");
        return 1;

      }

      fseek(JSON_file,0,SEEK_END);            //go to the end of file
      length=ftell(JSON_file);                //how many bytes
      fseek(JSON_file,0,SEEK_SET);            //go to the beggining of file
      data=malloc(length);

      if(data!=NULL){

        fread(data,1,length,JSON_file);       //store specs
        insert_id_in_hash(currSite->Id_Hash_Array,idBucketsNum,full_id,data);
        printf("data: %s",data);

      }

      fclose(JSON_file);
      JSON_file=NULL;

      free(data);
      data=NULL;

      free(path_ff);
      path_ff=NULL;

      free(id);
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

  closedir(FD);
  FD=NULL;


  DW=fopen(argv[2],"r");

  char* line=NULL;
  char* full_id_1=NULL;
  char* full_id_2=NULL;
  char* match=NULL;
  char*site1=NULL;
  char* site2=NULL;
  char* cid=NULL;

  Hashed_Id complex1=NULL;
  Hashed_Id complex2=NULL;
  complex tmp1=NULL;
  Hashed_Id tmp_complex=NULL;

  size_t line_buf_size=0;
  int line_size=0;
  int v=0;

  line_size=getline(&line,&line_buf_size,DW);       //first line not a pair


  while(line_size>=0){      //loop for each line

    if(v==0){               //skip first line
      v=1;
      continue;
    }

    full_id_1=strtok(line,",");
    full_id_2=strtok(NULL,",");
    match=strtok(NULL,",");

    if(strcmp(match,"1")==0){

      get_site_from_id(site1,full_id_1);
      get_site_from_id(site2,full_id_2);

      complex1=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_1);
      complex2=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_2);

      tmp1=complex1->Complex;

      while(tmp1->next!=NULL){
        tmp1=tmp1->next;
      }

      tmp1->next=complex2->Complex;

      tmp1=complex2->Complex;

      while(1){

        if(tmp1==NULL){
          break;
        }

        if(strcmp(full_id_2,tmp1->id)==0){
          continue;
        }

        tmp_complex=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,tmp1->id);
        tmp_complex->Complex=complex1->Complex;

        tmp1=tmp1->next;

      }

      complex2->Complex=complex1->Complex;

    }

    line_size=getline(&line,&line_buf_size,DW);   //get next line


  }


  //print result in a new csv file


  return 0;

}
