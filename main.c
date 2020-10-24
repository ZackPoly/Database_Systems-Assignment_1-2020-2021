#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

// prwto orisma to camera_specs kai deutero to csv

int main(int argc, char** argv){


  DIR* FD;
  DIR* SD;

  struct dirent* in_file;
  struct dirent* d_files;

  FILE* JSON_file;

  char* data;
  int length;

  char* path=NULL;
  char* path_ff=NULL;

  FD=opendir(argv[1]);                                      //open directory

  if (FD==NULL){                                            //check if directory is open
    printf("Can't open directory!\n");
    return 1;
  }


  while((in_file=readdir(FD))!=NULL){                       //read all subdirectories

    if(strcmp(in_file->d_name,".")==0){
      continue;
    }

    if(strcmp(in_file->d_name,"..")==0){
      continue;
    }


    printf("Subdirectory: %s\n",in_file->d_name);


    path=malloc(strlen(argv[1])+strlen(in_file->d_name)+2);
    strcpy(path,argv[1]);
    strcat(path,in_file->d_name);
    strcat(path,"/");
    SD=opendir(path);                            //open subdirectory


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


      path_ff=malloc(strlen(path)+strlen(d_files->d_name)+1);
      strcpy(path_ff,path);
      strcat(path_ff,d_files->d_name);

      printf("Path for file is : %s \n ",path_ff);

      JSON_file=fopen(path_ff,"r");             //open JSON file

      if(JSON_file==NULL){                              //check if it is open

        printf("Can't open JSON file! \n");
        return 1;

      }

      fseek(JSON_file,0,SEEK_END);
      length=ftell(JSON_file);                //how many bytes
      fseek(JSON_file,0,SEEK_SET);
      data=malloc(length);

      if(data!=NULL){

        fread(data,1,length,JSON_file);       //store specs
        printf("data: %s",data);

      }

      //insert(data)

      fclose(JSON_file);
      JSON_file=NULL;

      free(data);
      data=NULL;

      free(path_ff);
      path_ff=NULL;

    }

    closedir(SD);
    SD=NULL;
    free(path);
    path=NULL;

  }



  closedir(FD);
  FD=NULL;
  return 0;




}
