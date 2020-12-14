#include "model.h"


// void initialize_model(Model model,int vector_size,float constant,float learning_rate){
//
//   int i=0;
//   model->weights=malloc(vector_size*sizeof(float));
//
//   for(i=0;i<vector_size;i++){
//     model->wights[i]=0.0;
//   }
//
//   model->b=constant;
//   model->h=learning_rate;
//
// }
//
//
// void fit(Model model,BoW bow,char* testing_file){
//
//   FILE* test_file_ptr=NULL;
//
//   test_file_ptr=fopen(testing_file,"r");
//
//   char* line=NULL;
//   char* full_id_1=NULL;
//   char* full_id_2=NULL;
//   char* match=NULL;
//   int line_size=0;
//   size_t= line_buf_size=0;
//
//   int i=0;
//   int vec_index_1=0;
//   int vec_index_2=0;
//
//
//   float f=0.0;                  //f(x)
//   float p=0.0;                  //p(x)
//
//   float e=expf(1.0);
//
//   int num_of_pairs=0;
//   //na brw plh8os zeugariwn, lines arxeiou prin thn while?
//
//   float* p_values=NULL;                                                         //store p(x) for each pair
//   p_values=malloc(num_of_pairs*sizeof(float));
//   for(i=0;i<num_of_pairs;i++){
//     p_values[i]=0.0;
//   }
//
//   int l=0;                                                                      //pair index
//
//   float* real_match=malloc(num_of_pairs*sizeof(float));                         //vector for real match of each pair
//
//
//   float** vector_of_pair=NULL;
//   vector_of_pair=malloc(num_of_pairs*sizeof(float*));
//   for(i=0;i<num_of_pairs;i++){
//     vector_of_pair[i]=malloc(bow->bow_len*sizeof(float));
//     for(j=0;j<bow_len;j++){
//       vector_of_pair[i][j]=0.0;
//     }
//   }
//
//   int j=0;
//
//   float* derivatives=malloc(bow->bow_len*sizeof(float));
//   for(i=0;i<bow->bow_len;i++){
//     derivatives[i]=0.0;
//   }
//
//   while((line_size=getline(&line,&line_buf_size,test_file_ptr))>0){
//
//     full_id_1=strtok(line,",");
//     full_id_2=strtok(NULL,",");
//     match=strtok(NULL,",");
//     match=strtok(match,"\n");
//
//     real_match[l]=float(atoi(match));
//
//     //briskw thn 8esh tou arxeiou1 ston pinaka values = vec_index_1
//
//     //briskw thn 8esh tou arxeiou2 ston pinaka values = vec_index_2
//
//     for(i=0;i<bow->bow_len,i++){
//       vector_of_pair[l][i]=//epeksergasia tou bow->values[vec_index_1][i] xwse symbolo bow->values[vec_index_2][i]
//     }
//
//     l++;
//
//   }
//
//   fclose(test_file_ptr);
//
//   int iterations=15;
//   int k=0;
//   l=0;
//
//   while(k<iterations){
//
//     for(l=0;l<num_of_pairs;l++){
//       f=0 ;
//
//       for(i=0;i<bow_len;i++){
//         f=f+model->weigths[i]*vector_of_pair[l][i];
//       }
//
//       f=f+b;
//
//       p=1/(1+expf(-f));
//
//       p_values[l]=p;
//     }
//
//     for(i=0;i<bow->bow_len;i++){
//       for(j=0;j<num_of_pairs;j++){
//         derivatives[i]=derivatives[i]+ (p_values[j]-real_match[j])*vectors_of_pairs[j][i];
//       }
//     }
//
//
//     for(j=0;j<bow->bow_len;j++){
//       weights[j]=weigths[j] - model->h*derivatives[j];
//     }
//     k++;
//   }
//
// }
