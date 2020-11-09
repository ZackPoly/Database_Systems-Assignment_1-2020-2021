#include "site_hash.h"

void print_complexes(Hash_For_Site,int,int,FILE*) ;


//./main Datasets
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
      dataset_path=malloc(strlen(main_dir->d_name)+2);        //dataset_path=camera_specs path
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

  FD=opendir(dataset_path);                                      //open camera_specs/ directory
  free(dataset_path);

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


    path=malloc(strlen(argv[2])+strlen(in_file->d_name)+2);
    strcpy(path,argv[2]) ;
    strcat(path,"/");
    strcat(path,in_file->d_name);
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

      path_ff=malloc(strlen(path)+strlen(d_files->d_name)+2);
      strcpy(path_ff,path);
      strcat(path_ff,"/");
      strcat(path_ff,d_files->d_name);

      //printf("Path for file is : %s \n ",path_ff);

      JSON_file=fopen(path_ff,"r");             //open JSON file

      if(JSON_file==NULL){                              //check if it is open

        printf("Can't open JSON file! \n");
        return 1;

      }


      /*
      fseek(JSON_file,0,SEEK_END);            //go to the end of file
      length=ftell(JSON_file);                //how many bytes
      fseek(JSON_file,0,SEEK_SET);            //go to the beggining of file
      data=malloc(length+1);

      if(data!=NULL){

        fread(data,length,1,JSON_file);       //store specs
        data[length]='\0' ;
        insert_id_in_hash(currSite->Id_Hash_Array,idBucketsNum,full_id,data);


      }
      */

      insert_id_in_hash(currSite->Id_Hash_Array,idBucketsNum,full_id,JSON_file);

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

  closedir(FD);
  FD=NULL;


  DW=fopen(argv[3],"r");

  char* line=NULL;
  char* full_id_1=NULL;
  char* full_id_2=NULL;
  char* match=NULL;
  char* cid=NULL;

  Hashed_Id complex1=NULL;
  Hashed_Id complex2=NULL;
  complex tmp1=NULL;
  Hashed_Id tmp_complex=NULL;

  size_t line_buf_size=0;
  int line_size=0;
  int v=0;

  line_size=getline(&line,&line_buf_size,DW);       //first line not a pair

  while(line_size>0){      //loop for each line

    if(v==0){               //skip first line
      line_size=getline(&line,&line_buf_size,DW);   //get next line
      v=1;
      continue;
    }


    full_id_1=strtok(line,",");
    full_id_2=strtok(NULL,",");
    match=strtok(NULL,",");
    match=strtok(match,"\n");

    if(strcmp(match,"1")==0 && strcmp(full_id_1,full_id_2)!=0){


      complex1=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_1);
      complex2=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,full_id_2);

      if(complex1->Complex==complex2->Complex){
        line_size=getline(&line,&line_buf_size,DW);

        continue ;
      }

      complex1->Complex->tail->next=complex2->Complex->head ;
      complex1->Complex->tail=complex2->Complex->tail ;

      /*
      tmp1=complex1->Complex->head;

      while(tmp1->next!=NULL){
        tmp1=tmp1->next;
      }

      tmp1->next=complex2->Complex->head;
      */

      tmp1=complex2->Complex->head;

      while(tmp1!=NULL){


        if(strcmp(full_id_2,tmp1->id)!=0){

          tmp_complex=search_complex(site_hash_table,siteBucketsNum,idBucketsNum,tmp1->id);
          tmp_complex->Complex=complex1->Complex;
        }

        tmp1=tmp1->next;

      }

      free(complex2->Complex) ;
      complex2->Complex=complex1->Complex;

    }
    line_size=getline(&line,&line_buf_size,DW);   //get next line

  }
  free(line) ;
  fclose(DW) ;

  chdir("..") ;

  FILE* Output;
  Output=fopen("Output.csv","w") ;
  fprintf(Output, "%s,%s\n","left_spec_id","right_spec_id" );
  print_complexes(site_hash_table,siteBucketsNum,idBucketsNum,Output) ;
  //print result in a new csv file

  delete(site_hash_table,siteBucketsNum,idBucketsNum) ;
  fclose(Output) ;
  return 0;

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
