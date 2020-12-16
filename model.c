#include "model.h"

int countlines(const char *filename);

void initialize_model(Model* model,int vector_size,double b,double learning_rate){
  (*model)=malloc(sizeof(struct model)) ;

  int i=0;
  (*model)->weights=malloc(vector_size*sizeof(double));

  for(i=0;i<vector_size;i++){
    (*model)->weights[i]=0.0;
  }

  (*model)->b=b;
  (*model)->h=learning_rate;

}


void fit(Model model,BoW bow,char* training_file,Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum){

  int num_of_pairs=countlines(training_file);
  //briskw plh8os zeugariwn, lines arxeiou prin thn while

  FILE* train_file_ptr=NULL;

  train_file_ptr=fopen(training_file,"r");

  char* line=NULL;
  char* full_id_1=NULL;
  char* full_id_2=NULL;
  char* match=NULL;
  int line_size=0;
  size_t line_buf_size=0;

  int i=0;
  int j=0;
  // int vec_index_1=0;
  // int vec_index_2=0;


  double f=0.0;                  //f(x)
  double p=0.0;                  //p(x)

  //double e=expf(1.0);


  double* p_values=NULL;                                                         //store p(x) for each pair
  p_values=malloc(num_of_pairs*sizeof(double));
  for(i=0;i<num_of_pairs;i++){
    p_values[i]=0.0;
  }

  int l=0;                                                                      //pair index

  double* real_match=malloc(num_of_pairs*sizeof(double));                         //vector for real match of each pair


  double** vector_of_pair=NULL;
  vector_of_pair=malloc(num_of_pairs*sizeof(double*));
  for(i=0;i<num_of_pairs;i++){
    vector_of_pair[i]=malloc(bow->dict_len*sizeof(double));
    for(j=0;j<bow->dict_len;j++){
      vector_of_pair[i][j]=0.0;
    }
  }

  double* derivatives=malloc(bow->dict_len*sizeof(double));
  for(i=0;i<bow->dict_len;i++){
    derivatives[i]=0.0;
  }

  Hashed_Id left_id ;
  Hashed_Id right_id ;

  Hashed_Word tmp_word=NULL;

  line_size=getline(&line,&line_buf_size,train_file_ptr) ;                       // first line
  while((line_size=getline(&line,&line_buf_size,train_file_ptr))>0){

    full_id_1=strtok(line,",");
    full_id_2=strtok(NULL,",");
    match=strtok(NULL,",");
    match=strtok(match,"\n");

    real_match[l]=atof(match);

    //briskw thn 8esh tou arxeiou1 ston pinaka values = vec_index_1
    left_id=find_ID(SiteTable,siteBucketsNum,idBucketsNum,full_id_1) ;

    //briskw thn 8esh tou arxeiou2 ston pinaka values = vec_index_2
    right_id=find_ID(SiteTable,siteBucketsNum,idBucketsNum,full_id_2) ;

    // for(i=0;i<bow->dict_len;i++){
    //   vector_of_pair[l][i]=//epeksergasia tou bow->values[vec_index_1][i] xwse symbolo bow->values[vec_index_2][i]
    // }
    for(i=0 , j=0 ;i<BOW_B_NUM;i++){

      tmp_word=bow->dict[i].root;

      while(tmp_word!=NULL){
        vector_of_pair[l][j]=fabs(bow->values[left_id->tf_idf_index][tmp_word->index] - bow->values[right_id->tf_idf_index][tmp_word->index]) ;

        j++ ;
        tmp_word=tmp_word->next;
      }
    }

    l++;

  }

  fclose(train_file_ptr);

  int iterations=1;
  int k=0;
  l=0;

  while(k<iterations){

    for(l=0;l<num_of_pairs;l++){
      f=0 ;

      for(i=0;i<bow->dict_len;i++){
        f=f+model->weights[i]*vector_of_pair[l][i];
      }

      f=f+model->b;

      p=1/(1+expf(-f));

      p_values[l]=p;
    }

    for(i=0;i<bow->dict_len;i++){
      for(j=0;j<num_of_pairs;j++){
        derivatives[i]=derivatives[i]+ (p_values[j]-real_match[j])*vector_of_pair[j][i];
      }
    }

    printf("\n%s\n","finished calculating weights" );
    for(j=0;j<bow->dict_len;j++){
      model->weights[j]=model->weights[j] - model->h*derivatives[j];
      //printf("%f\n",model->weights[j] );
    }
    k++;
  }

  free(p_values) ;
  free(derivatives) ;
  free(real_match) ;
  for(i=0;i<num_of_pairs;i++){
    free(vector_of_pair[i]) ;
  }
  free(vector_of_pair) ;
  free(line) ;

}


void predict(Model model,BoW bow,char* testing_file,Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum){

  int num_of_pairs=countlines(testing_file);

  FILE* test_file_ptr=NULL;
  int i,j ;

  test_file_ptr=fopen(testing_file,"r");

  char* line=NULL;
  char* full_id_1=NULL;
  char* full_id_2=NULL;
  char* match=NULL;
  int line_size=0;
  size_t line_buf_size=0;

  double f=0.0;                  //f(x)
  double p=0.0;                  //p(x)

  int correct=0 ;

  double* x=malloc(bow->dict_len*sizeof(double));
  int real_match ;

  Hashed_Id left_id ;
  Hashed_Id right_id ;

  Hashed_Word tmp_word=NULL;

  line_size=getline(&line,&line_buf_size,test_file_ptr) ;                       // first line
  while((line_size=getline(&line,&line_buf_size,test_file_ptr))>0){

    full_id_1=strtok(line,",");
    full_id_2=strtok(NULL,",");
    match=strtok(NULL,",");
    match=strtok(match,"\n");


    //briskw thn 8esh tou arxeiou1 ston pinaka values = vec_index_1
    left_id=find_ID(SiteTable,siteBucketsNum,idBucketsNum,full_id_1) ;

    //briskw thn 8esh tou arxeiou2 ston pinaka values = vec_index_2
    right_id=find_ID(SiteTable,siteBucketsNum,idBucketsNum,full_id_2) ;


    // concat vectors
    for(i=0 , j=0 ;i<BOW_B_NUM;i++){

      tmp_word=bow->dict[i].root;

      while(tmp_word!=NULL){
        x[j]=fabs(bow->values[left_id->tf_idf_index][tmp_word->index] - bow->values[right_id->tf_idf_index][tmp_word->index]) ;

        j++ ;
        tmp_word=tmp_word->next;
      }
    }

    // predict
    f=0 ;
    for(i=0;i<bow->dict_len;i++){
      f=f+model->weights[i]*x[i];
    }

    f=f+model->b;

    p=1/(1+expf(-f));

    real_match=atoi(match) ;
    if(p<0.5 && real_match==0) correct++ ;
    else if(p>=0.5 && real_match==1)  correct++ ;

  }
  printf("%d Correct predictions out of %d pairs\n",correct,num_of_pairs);
  printf("Model accuracy = %d%% \n",(correct*100)/num_of_pairs );

  free(x) ;
  free(line) ;
  fclose(test_file_ptr) ;

}


void delete_model(Model* model){

  free((*model)->weights);
  (*model)->weights=NULL;

  free((*model)) ;

}


int countlines(const char *filename)
{
  // count the number of lines in the file called filename
  FILE *fp = fopen(filename,"r");
  int ch=0;
  int lines=0;

  if (fp == NULL){
    fclose(fp) ;
    return 0;
  }

  lines++;
  while ((ch = fgetc(fp)) != EOF)
    {
      if (ch == '\n')
    lines++;
    }
  fclose(fp);
  return lines;
}
