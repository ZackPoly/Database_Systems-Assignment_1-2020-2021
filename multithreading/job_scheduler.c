#include "job_scheduler.h"


void *master_thread(void *arg) {
  struct arg_struct* args = (struct arg_struct*)arg;

  int batches = get_batches_num("Train.csv");
  int test_batches = get_batches_num("Test.csv");
  int val_batches = get_batches_num("Validation.csv");

  int i,j,k;
  int epoch=3;
  // double* derivs=malloc(args->bow->dict_len*sizeof(double));

  // double** derivatives = malloc(batches*sizeof(double*));
  // for(i=0 ; i<batches ; i++){
  //   derivatives[i] = malloc(args->bow->dict_len*sizeof(double));
  // }
  // double** test_accs = malloc(test_batches*sizeof(double*));
  // for(i=0 ; i<test_batches ; i++){
  //   test_accs[i] = malloc(sizeof(double));
  // }
  // double** val_accs = malloc(val_batches*sizeof(double*));
  // for(i=0 ; i<val_batches ; i++){
  //   val_accs[i] = malloc(sizeof(double));
  // }

  FILE* training_file=fopen("Train.csv","r");
  FILE* testing_file=fopen("Test.csv","r");

  char* line=NULL;
  char* match=NULL;
  int line_size=0;
  size_t line_buf_size=0;

  double* derivatives = malloc(args->bow->dict_len*sizeof(double));


  Job* job;

  for(j=0 ; j<epoch ; j++){
    for(i=0 ; i<args->bow->dict_len ; i++){
      derivatives[i] = 0.0;
    }

    fseek(training_file,0,SEEK_SET);
    line_size=getline(&line,&line_buf_size,training_file); // first line

    for(i=0 ; i<batches ; i++){
      job = make_job(args->model,args->bow,training_file,args->SiteTable,args->siteBucketsNum,args->idBucketsNum,derivatives,train_job,args->sch);
      submit_job(args->sch, job);
    }

    execute_all_jobs(args->sch);
    wait_all_tasks_finish(args->sch);

    for(k=0;k<args->bow->dict_len;k++){
      derivatives[k]=derivatives[k]/batches;
      args->model->weights[k]=args->model->weights[k] - args->model->h*derivatives[k];
    }

    // validate
    // for(i=0 ; i<val_batches ; i++){
    //   job = make_job(args->model,args->bow,"Validate.csv",args->SiteTable,args->siteBucketsNum,args->idBucketsNum,derivatives[i],test_job,args->sch);
    //   submit_job(args->sch, job);
    // }
    // execute_all_jobs(args->sch);
    // wait_all_tasks_finish(args->sch);
  }

  double* accuracy = malloc(test_batches*sizeof(double));

  line_size=getline(&line,&line_buf_size,testing_file);
  for(i=0 ; i<test_batches ; i++){
    job = make_job(args->model,args->bow,testing_file,args->SiteTable,args->siteBucketsNum,args->idBucketsNum,accuracy+i,test_job,args->sch);
    submit_job(args->sch, job);
  }
  execute_all_jobs(args->sch);
  wait_all_tasks_finish(args->sch);


  for(i=1 ; i<=5 ; i++){
    job = make_job(NULL,NULL,NULL,NULL,0,0,NULL,terminate,NULL);

    submit_job(args->sch, job);
  }
  execute_all_jobs(args->sch);

  // for(i=1 ; i<=5 ; i++){
  //   pthread_join(args->sch->tids[i], NULL);
  // }

  fclose(training_file);
  fclose(testing_file);

  double correct=0;
  for(k=0;k<test_batches;k++){
    correct+=accuracy[k];
  }
  printf("%lf %%\n",(correct*100)/countlines("Test.csv") );

  free(accuracy);
  free(derivatives);
  free(line);
}


void *thread_routine(void *arg){
    JobScheduler* sch=(JobScheduler*)arg;
    Job * job=NULL;

    while(1){
      if (err = pthread_mutex_lock(&(sch->mtx))) {                              /* Lock mutex */
        perror2("pthread_mutex_lock", err); exit(1);
      }

      pthread_cond_wait(&(sch->cv), &(sch->mtx));                               // wait for cond variable

      if (err = pthread_mutex_unlock(&(sch->mtx))) {                            /* Unlock mutex */
        perror2("pthread_mutex_unlock", err); exit(1);
      }


      while (1) {                                                               // take jobs unlit queue is empty

        if (err = pthread_mutex_lock(&(sch->mtxQ))) {                           /* Lock queue mutex */
          perror2("pthread_mutex_lock", err); exit(1);
        }

        job=deQueue(sch->q);                                                    // remove job from queue

        if (err = pthread_mutex_unlock(&(sch->mtxQ))) {                         /* Unlock queue mutex */
          perror2("pthread_mutex_unlock", err); exit(1);
        }

        if(job!=NULL){
          if(job->ftpr==terminate){
            delete_job(job);
            pthread_exit((void *) 47);
          }
          (*(job->ftpr))(job->arg_ptr);                                         // execute job
          delete_job(job);

          if (err = pthread_mutex_lock(&(sch->mtxPJ))) {                           /* Lock queue mutex */
            perror2("pthread_mutex_lock", err); exit(1);
          }

          sch->pending_jobs--;
          if(!sch->pending_jobs){
            pthread_cond_signal(&(sch->cvPJ));
          }

          if (err = pthread_mutex_unlock(&(sch->mtxPJ))) {                         /* Unlock queue mutex */
            perror2("pthread_mutex_unlock", err); exit(1);
          }
        }
        else{

          break;
        }

      }

    }

}

JobScheduler* initialize_scheduler(int execution_threads){

  JobScheduler* sch=malloc(sizeof(JobScheduler));
  sch->execution_threads=execution_threads;
  sch->q=createQueue();
  sch->tids=malloc(execution_threads*sizeof(pthread_t));

  sch->pending_jobs=0;

  pthread_mutex_init(&(sch->mtx), NULL);
  pthread_cond_init(&(sch->cv), NULL);

  pthread_mutex_init(&(sch->mtxQ), NULL);
  pthread_mutex_init(&(sch->mtxPJ), NULL);
  pthread_cond_init(&(sch->cvPJ), NULL);

  pthread_mutex_init(&(sch->mtxDer), NULL);

  int i;
  for(i=0;i<execution_threads;i++){
      err = pthread_create(sch->tids+i, NULL, thread_routine, sch);
      if (err != 0) {
          /* Create a thread */
          perror2("pthread_create", err);   exit(1);
      }
  }

  return sch;
}

int submit_job(JobScheduler* sch, Job* j){
  if (err = pthread_mutex_lock(&(sch->mtxQ))) {                         /* Unlock queue mutex */
    perror2("pthread_mutex_unlock", err); exit(1);
  }// lock q mutex

  enQueue(sch->q, j);
  sch->pending_jobs++;

  if (err = pthread_mutex_unlock(&(sch->mtxQ))) {                         /* Unlock queue mutex */
    perror2("pthread_mutex_unlock", err); exit(1);
  }// unlock q mutex

  return 0;
}

int execute_all_jobs(JobScheduler* sch){
  pthread_cond_broadcast(&(sch->cv));

  return 0;
}

int wait_all_tasks_finish(JobScheduler* sch){
  // int i;
  // for(i=0;i<sch->execution_threads;i++){
  //     pthread_join(*(sch->tids+i), NULL);
  // }

  if (err = pthread_mutex_lock(&(sch->mtxPJ))) {                              /* Lock mutex */
    perror2("pthread_mutex_lock", err); exit(1);
  }

  while (sch->pending_jobs) {
    pthread_cond_wait(&(sch->cvPJ), &(sch->mtxPJ));
  }

  if (err = pthread_mutex_unlock(&(sch->mtxPJ))) {                            /* Unlock mutex */
    perror2("pthread_mutex_unlock", err); exit(1);
  }

  return 0;
}

int destroy_scheduler(JobScheduler* sch){
  free(sch->q);
  free(sch->tids);
  if((err=pthread_mutex_destroy(&(sch->mtx)))!=0){
    perror2("pthread_mutex_destroy", err);
    exit(1);
  }
  if((err=pthread_cond_destroy(&(sch->cv)))!=0){
    perror2("pthread_cond_destroy", err);
    exit(1);
  }
  if((err=pthread_mutex_destroy(&(sch->mtxQ)))!=0){
    perror2("pthread_mutex_destroy", err);
    exit(1);
  }
  if((err=pthread_mutex_destroy(&(sch->mtxPJ)))!=0){
    perror2("pthread_mutex_destroy", err);
    exit(1);
  }
  if((err=pthread_cond_destroy(&(sch->cvPJ)))!=0){
    perror2("pthread_cond_destroy", err);
    exit(1);
  }
  if((err=pthread_mutex_destroy(&(sch->mtxDer)))!=0){
    perror2("pthread_mutex_destroy", err);
    exit(1);
  }

  return 0;
}

void delete_job(Job* job){
  free(job->arg_ptr[4]);
  free(job->arg_ptr[5]);
  free(job->arg_ptr);
  free(job);
}

int get_batches_num(char* file_name){

  int source_lines=countlines(file_name) - 1;

  int num_of_batches=source_lines/BATCH_SIZE;

  if(source_lines%BATCH_SIZE!=0){
    num_of_batches++;
  }

  return num_of_batches;
}

Job* make_job(Model model,BoW bow,FILE* file,Hash_For_Site SiteTable,int siteBucketsNum,int idBucketsNum,double* derivatives,void (*ftpr)(void **),JobScheduler* sch){
  Job* job = malloc(sizeof(Job));
  job->ftpr=ftpr;

  job->arg_ptr=malloc(8*sizeof(void*));
  // job->arg_ptr[0] = malloc(sizeof(Model));
  job->arg_ptr[0] = (void*)model;
  job->arg_ptr[1] = (void*)bow;
  job->arg_ptr[2] = (void*)file;
  job->arg_ptr[3] = (void*)SiteTable;
  job->arg_ptr[4] = malloc(sizeof(int));
  *(int*)job->arg_ptr[4]=siteBucketsNum;
  job->arg_ptr[5] = malloc(sizeof(int));
  *(int*)job->arg_ptr[5]=idBucketsNum;
  job->arg_ptr[6] = (void*)derivatives;

  job->arg_ptr[7] = (void*)sch;

  return job;

}

void train_job(void *args[8]){

  Model model=(Model)args[0];
  BoW bow = (BoW)args[1] ;
  // char* training_file=(char*)args[2];
  FILE* training_file=(FILE*)args[2];
  Hash_For_Site SiteTable=(Hash_For_Site)args[3];
  int siteBucketsNum=*(int*)args[4];
  int idBucketsNum=*(int*)args[5];
  double* derivatives=(double*)args[6];

  JobScheduler* sch=(JobScheduler*)args[7];

  // int num_of_pairs=countlines(training_file);
  int num_of_pairs=0;

  // FILE* train_file_ptr=NULL;
  // train_file_ptr=fopen(training_file,"r");

  char* line=NULL;
  char* full_id_1=NULL;
  char* full_id_2=NULL;
  char* match=NULL;
  int line_size=0;
  size_t line_buf_size=0;
  char* rest1;
  char* rest2;

  int i=0;
  int j=0;

  double f=0.0;                  //f(x)
  double p=0.0;                  //p(x)

  double* p_values=NULL;                                                         //store p(x) for each pair
  p_values=malloc(BATCH_SIZE*sizeof(double));
  for(i=0;i<BATCH_SIZE;i++){
    p_values[i]=0.0;
  }

  int l=0;                                                                      //pair index

  double* real_match=malloc(BATCH_SIZE*sizeof(double));                       //vector for real match of each pair


  double** vector_of_pair=NULL;
  vector_of_pair=malloc(BATCH_SIZE*sizeof(double*));
  for(i=0;i<BATCH_SIZE;i++){
    vector_of_pair[i]=malloc(bow->dict_len*sizeof(double));
    for(j=0;j<bow->dict_len;j++){
      vector_of_pair[i][j]=0.0;
    }
  }

  // double* derivatives=malloc(bow->dict_len*sizeof(double));
  // for(i=0;i<bow->dict_len;i++){
  //   derivatives[i]=0.0;
  // }

  Hashed_Id left_id ;
  Hashed_Id right_id ;

  Hashed_Word tmp_word=NULL;


  while(num_of_pairs<BATCH_SIZE && (line_size=getline(&line,&line_buf_size,training_file))>0){            //for each pair in file,find its vector and store it

    full_id_1=strtok_r(line,",",&rest1);
    full_id_2=strtok_r(rest1,",",&rest2);
    match=strtok_r(rest2,"\n",&rest1);
    //match=strtok_r(match,"\n");

    real_match[l]=atof(match);

    left_id=find_ID(SiteTable,siteBucketsNum,idBucketsNum,full_id_1) ;
    right_id=find_ID(SiteTable,siteBucketsNum,idBucketsNum,full_id_2) ;

    for(i=0 , j=0 ;i<BOW_B_NUM;i++){

      tmp_word=bow->dict[i].root;

      while(tmp_word!=NULL){
        vector_of_pair[l][j]=fabs(bow->values[left_id->tf_idf_index][tmp_word->index] - bow->values[right_id->tf_idf_index][tmp_word->index]) ;
        j++ ;
        tmp_word=tmp_word->next;
      }
    }

    l++;
    num_of_pairs++;

  }

  // fclose(train_file_ptr);

  for(l=0;l<num_of_pairs;l++){                                                  //for each pair store its p(x)=Ïƒ(f(x))
    f=0 ;

    for(i=0;i<bow->dict_len;i++){
      f=f+model->weights[i]*vector_of_pair[l][i];
    }

    f=f+model->b;

    p=1/(1+expf(-f));

    p_values[l]=p;
  }

  double temp_der=0;

  for(i=0;i<bow->dict_len;i++){                                                 //for each component of vector,find its derivative
    temp_der=0;
    for(j=0;j<num_of_pairs;j++){
      // derivatives[i]=derivatives[i]+ (p_values[j]-real_match[j])*vector_of_pair[j][i];
      temp_der=temp_der+ (p_values[j]-real_match[j])*vector_of_pair[j][i];
    }
    // derivatives[i]=derivatives[i]/num_of_pairs;
    // temp_der=temp_der/num_of_pairs;

    if (err = pthread_mutex_lock(&(sch->mtxDer))) {                              /* Lock mutex */
      perror2("pthread_mutex_lock", err); exit(1);
    }

    derivatives[i]+=temp_der;

    if (err = pthread_mutex_unlock(&(sch->mtxDer))) {                            /* Unlock mutex */
      perror2("pthread_mutex_unlock", err); exit(1);
    }
  }

  free(p_values) ;
  free(real_match) ;
  for(i=0;i<BATCH_SIZE;i++){
    free(vector_of_pair[i]) ;
  }
  free(vector_of_pair) ;
  free(line) ;
  // free(args[4]);
  // free(args[5]);

  //derivatives = the average derivative of each vector-component, from the current training file
}

void test_job(void *args[8]){

  Model model=(Model)args[0];
  BoW bow = (BoW)args[1] ;
  FILE* testing_file=(FILE*)args[2];
  Hash_For_Site SiteTable=(Hash_For_Site)args[3];
  int siteBucketsNum=*(int*)args[4];
  int idBucketsNum=*(int*)args[5];
  double* accuracy=(double*)args[6];

  JobScheduler* sch=(JobScheduler*)args[7];

  // int num_of_pairs=countlines(testing_file);

  // FILE* test_file_ptr=NULL;

  // test_file_ptr=fopen(testing_file,"r");

  int i,j ;
  int num_of_pairs=0;

  char* line=NULL;
  char* full_id_1=NULL;
  char* full_id_2=NULL;
  char* match=NULL;
  int line_size=0;
  size_t line_buf_size=0;
  char* rest1;
  char* rest2;

  double f=0.0;                  //f(x)
  double p=0.0;                  //p(x)

  int correct=0 ;

  double* x=malloc(bow->dict_len*sizeof(double));
  int real_match ;

  Hashed_Id left_id ;
  Hashed_Id right_id ;

  Hashed_Word tmp_word=NULL;


  while(num_of_pairs<BATCH_SIZE && (line_size=getline(&line,&line_buf_size,testing_file))>0){             //for each pair,predict its label

    full_id_1=strtok_r(line,",",&rest1);
    full_id_2=strtok_r(rest1,",",&rest2);
    match=strtok_r(rest2,"\n",&rest1);
    //match=strtok_r(match,"\n");

    left_id=find_ID(SiteTable,siteBucketsNum,idBucketsNum,full_id_1) ;
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

    f=0 ;
    for(i=0;i<bow->dict_len;i++){
      f=f+model->weights[i]*x[i];
    }

    f=f+model->b;

    p=1/(1+expf(-f));

    real_match=atoi(match) ;
    if(p<0.5 && real_match==0)
     correct++ ;
    else if(p>=0.5 && real_match==1)
     correct++ ;

    num_of_pairs++;

  }

  // *accuracy=(correct*100)/num_of_pairs;
  *accuracy=correct;

  free(x) ;
  free(line) ;

  // free(args[4]);
  // free(args[5]);
}

void terminate(void *args[1]) {
  // free(args[4]);
  // free(args[5]);

  while (1) {
    sleep(10);
  }
}
