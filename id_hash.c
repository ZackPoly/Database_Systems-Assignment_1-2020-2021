#include "id_hash.h"

/*
// Hash function for both site and full_id
*/
int hash_func(char* key,int bucketsNum){
  int i,v=0 ;
  for(i=0;i<=strlen(key);i++){
    v=v+key[i] ;
  }
  return v%bucketsNum ;
}

/*-------Utility Functions for Specs Initialization from JSON-------*/
int isspace(int argument){
  switch (argument) {
    case ' ':
      return 1 ;
    case '\n':
      return 1 ;
    case '\t':
      return 1 ;
    case '\v':
      return 1 ;
    case '\f':
      return 1 ;
    case '\r':
      return 1 ;
    default:
      return 0 ;
  }
}

void trim(char* s) {
    char* p=s;
    int l=strlen(p);

    while(isspace(p[l-1])) p[--l] = 0;
    while(*p && isspace(*p)) ++p, --l;

    memmove(s, p, l + 1);
}
/*-------Utility Functions for Specs Initialization from JSON-------*/

void initialize_specs(Hashed_Id IdNode,FILE*  JSON_file){
  char* line=NULL;
  char* token ; // this will hold the attributes
  char* rest ;  // rest of line (value of attribute)
  size_t line_buf_size=0;
  int line_size=0;

  specs currSpec=NULL ;

  while((line_size=getline(&line,&line_buf_size,JSON_file))>0){
    if(line[0]=='{' || line[0]=='}') continue ;


    token=strtok_r(line,":",&rest) ;
    trim(token) ;
    trim(rest) ;

    currSpec=new_attribute(IdNode,token) ;

    /* if array line has only one value */
    if(rest[0]=='['){

      do {
        line_size=getline(&line,&line_buf_size,JSON_file) ;
        trim(line) ;
        if(line[0]!=']'){
          push_value(currSpec,line) ;
        }

      } while(line[0]!=']');

    }
    else{
      push_value(currSpec,rest) ;
    }

  }
  free(line) ;
}


Hashed_Id search_id_in_hash(Hash_For_Id IdTable,char* full_id,int bucketsNum){
  int hashIndex=hash_func(full_id,bucketsNum);

  Hashed_Id currId=IdTable[hashIndex].root ;
  while(currId!=NULL){
    if(!strcmp(currId->full_id,full_id)) return currId ;

    currId=currId->next ;
  }
  return NULL ;
}

Hashed_Id insert_id_in_hash(Hash_For_Id IdTable,int bucketsNum,char* full_id,FILE* JSON_file,int filesNum){

  /*                     DUPLICATE ID REJECTED                     */
  if(search_id_in_hash(IdTable,full_id,bucketsNum)!=NULL) return NULL ;

  /*                  EMPTY BUCKET LIST                     */

  int hashIndex=hash_func(full_id,bucketsNum);

  if(IdTable[hashIndex].root==NULL){
    IdTable[hashIndex].root=malloc(sizeof(struct Hash_For_Id_Entry)) ;
    IdTable[hashIndex].root->next=NULL ;
    IdTable[hashIndex].root->full_id=malloc(strlen(full_id)+1) ;
    strcpy(IdTable[hashIndex].root->full_id,full_id) ;
    IdTable[hashIndex].root->tf_idf_index=filesNum ;

                      /* initialize Specs */
    IdTable[hashIndex].root->Specs=NULL ;
    initialize_specs(IdTable[hashIndex].root,JSON_file) ;

                      /* initialize Complex */
    IdTable[hashIndex].root->Complex=malloc(sizeof(struct Head_Of_Complex)) ;
    IdTable[hashIndex].root->Complex->head=malloc(sizeof(struct Comp_node)) ;
    IdTable[hashIndex].root->Complex->head->id=malloc(strlen(full_id)+1) ;
    strcpy(IdTable[hashIndex].root->Complex->head->id,full_id) ;
    IdTable[hashIndex].root->Complex->head->next=NULL ;
    IdTable[hashIndex].root->Complex->tail=IdTable[hashIndex].root->Complex->head ;
    IdTable[hashIndex].root->Complex->head_neg=NULL ;

    return IdTable[hashIndex].root ;
  }

  /*                  APPEND BUCKET LIST                     */
  Hashed_Id curId=IdTable[hashIndex].root ;

  while(curId->next!=NULL){ // collision
    curId=curId->next ;
  }
  curId->next=malloc(sizeof(struct Hash_For_Id_Entry)) ;
  curId->next->next=NULL ;
  curId->next->full_id=malloc(strlen(full_id)+1) ;
  strcpy(curId->next->full_id,full_id) ;
  curId->next->tf_idf_index=filesNum ;

                      /* initialize Specs */
  curId->next->Specs=NULL ;
  initialize_specs(curId->next,JSON_file) ;

                      /* initialize Complex */
  curId->next->Complex=malloc(sizeof(struct Head_Of_Complex)) ;
  curId->next->Complex->head=malloc(sizeof(struct Comp_node)) ;
  curId->next->Complex->head->id=malloc(strlen(full_id)+1) ;
  strcpy(curId->next->Complex->head->id,full_id) ;
  curId->next->Complex->head->next=NULL ;
  curId->next->Complex->tail=curId->next->Complex->head ;
  curId->next->Complex->head_neg=NULL ;

  return curId->next ;
}


void append_negative(comp_head Complex1,comp_head Complex2){
  neg_corr currNeg=Complex1->head_neg ;

  if(Complex1->head_neg==NULL){
    Complex1->head_neg=malloc(sizeof(struct Negative_Correlation)) ;
    Complex1->head_neg->next=NULL ;
    Complex1->head_neg->corr=Complex2 ;

  }
  else{
    while(currNeg->next!=NULL){
      if(currNeg->corr==Complex2)                                   // if negative already exists
        return ;

      currNeg=currNeg->next ;
    }
    currNeg->next=malloc(sizeof(struct Negative_Correlation)) ;
    currNeg->next->next=NULL ;
    currNeg->next->corr=Complex2 ;

  }

  currNeg=Complex2->head_neg ;

  if(Complex2->head_neg==NULL){
    Complex2->head_neg=malloc(sizeof(struct Negative_Correlation)) ;
    Complex2->head_neg->next=NULL ;
    Complex2->head_neg->corr=Complex1 ;

  }
  else{
    while(currNeg->next!=NULL){
      if(currNeg->corr==Complex1)                                   // if negative already exists
        return ;

      currNeg=currNeg->next ;
    }
    currNeg->next=malloc(sizeof(struct Negative_Correlation)) ;
    currNeg->next->next=NULL ;
    currNeg->next->corr=Complex1 ;

  }
}

int search_negative(comp_head Complex1,comp_head Complex2){
  int re=0 ;

  neg_corr tmp_neg_corr1=Complex1->head_neg;
  while(tmp_neg_corr1!=NULL){
    if(tmp_neg_corr1->corr==Complex2) re++ ;

    tmp_neg_corr1=tmp_neg_corr1->next;
  }

  return re ;
}

void delete_negatives(comp_head Complex){
  neg_corr tmp_neg1=Complex->head_neg ;
  neg_corr tmp_neg2=NULL ;

  while(tmp_neg1!=NULL){                                  // delete negative correlations
    tmp_neg2=tmp_neg1->next ;
    free(tmp_neg1) ;
    tmp_neg1=NULL ;
    tmp_neg1=tmp_neg2 ;
  }
  Complex->head_neg=NULL ;
}

void delete_complex_from_list(comp_head Complex,comp_head ComplexToDel){

  if(Complex->head_neg==NULL) return ;

  neg_corr tmp_neg_corr1=NULL;
  neg_corr tmp_neg_corr2=NULL;

  tmp_neg_corr1=Complex->head_neg->next;
  tmp_neg_corr2=Complex->head_neg;

  if(Complex->head_neg->corr==ComplexToDel){

    free(Complex->head_neg);

    Complex->head_neg=tmp_neg_corr1;

    return;
  }

  while(tmp_neg_corr1!=NULL){

    if(tmp_neg_corr1->corr==ComplexToDel){

        tmp_neg_corr2->next=tmp_neg_corr1->next;

        free(tmp_neg_corr1);

        return;
    }

    tmp_neg_corr2=tmp_neg_corr2->next;
    tmp_neg_corr1=tmp_neg_corr1->next;

  }

}

void change_negatives(comp_head Complex1,comp_head Complex2){

  neg_corr tmp_neg_corr1=Complex2->head_neg;
  neg_corr tmp_neg_corr2=NULL;

  int i ;

  while(tmp_neg_corr1!=NULL){
    i=search_negative(tmp_neg_corr1->corr,Complex2);
    // printf("%d\n",i );
    for(i;i>0;i--){
      delete_complex_from_list(tmp_neg_corr1->corr,Complex2) ;
    }
    // delete_complex_from_list(tmp_neg_corr1->corr,Complex2) ;
    //if(search_negative(tmp_neg_corr1->corr,Complex2)) printf("%s\n","something is Wrong" );
    append_negative(Complex1,tmp_neg_corr1->corr) ;

    tmp_neg_corr1=tmp_neg_corr1->next ;
  }

  // tmp_neg_corr2=Complex1->head_neg;
  // while(tmp_neg_corr1!=NULL){
  //   if(search_negative(tmp_neg_corr1->corr,Complex2)) printf("%s\n","MonkaS" );
  //   // append_negative(Complex1,tmp_neg_corr1->corr) ;
  //
  //   tmp_neg_corr1=tmp_neg_corr1->next ;
  // }

}


// void change_negatives(comp_head Complex1,comp_head Complex2){
//
//   if(Complex2->head_neg==NULL){
//     return;
//   }
//
//   neg_corr tmp_neg_corr1=NULL;
//   neg_corr tmp_neg_corr2=NULL;
//   neg_corr tmp_neg_corr3=NULL;
//
//   if(Complex1->head_neg!=NULL){                                                 // if complex 1 has negative correlations
//     tmp_neg_corr1=Complex2->head_neg;
//     tmp_neg_corr3=Complex2->head_neg;
//
//     int flag ;
//
//     while(tmp_neg_corr1!=NULL){                                                 // scan negative correlations of complex 2
//
//         tmp_neg_corr2=tmp_neg_corr1->corr->head_neg;
//
//         flag = 0 ;
//
//         while(tmp_neg_corr2!=NULL){                                             // scan negative correlations of neg. corr.
//           if(tmp_neg_corr2->corr==Complex1){
//
//               delete_complex_from_list(tmp_neg_corr1->corr,Complex2) ;          // delete
//
//               if(Complex2->head_neg==tmp_neg_corr1){
//
//                   tmp_neg_corr1=tmp_neg_corr1->next;
//
//                   free(Complex2->head_neg);  printf("%s\n","deleted" );
//
//                   Complex2->head_neg=tmp_neg_corr1;
//
//                   tmp_neg_corr1=Complex2->head_neg;
//                   tmp_neg_corr3=Complex2->head_neg;
//
//               }else{
//                 tmp_neg_corr3->next=tmp_neg_corr1->next;
//
//                 free(tmp_neg_corr1);  printf("%s\n","deleted" );
//
//                 tmp_neg_corr1=tmp_neg_corr3->next;
//
//               }
//
//               flag = 1 ;
//               break ;
//           }
//           else{
//             tmp_neg_corr2=tmp_neg_corr2->next;
//           }
//         }
//
//         if(!flag){
//           if(tmp_neg_corr3!=tmp_neg_corr1){
//             tmp_neg_corr3=tmp_neg_corr3->next;
//           }
//
//           tmp_neg_corr1=tmp_neg_corr1->next;
//
//         }
//
//     }
//   }
//
//
//   tmp_neg_corr1=Complex2->head_neg;
//
//   while(tmp_neg_corr1!=NULL){                                                   //for each complex in updated negative correlations of Complex2,change the negative correlation to Complex1
//     tmp_neg_corr2=tmp_neg_corr1->corr->head_neg;
//     while(tmp_neg_corr2!=NULL){
//       if(tmp_neg_corr2->corr==Complex2){
//         tmp_neg_corr2->corr=Complex1;
//         break;
//       }
//       tmp_neg_corr2=tmp_neg_corr2->next;
//     }
//     tmp_neg_corr1=tmp_neg_corr1->next;
//   }
//
//   if(Complex1->head_neg==NULL){
//     Complex1->head_neg=Complex2->head_neg;
//   }
//   else{
//     tmp_neg_corr1=Complex1->head_neg;
//
//     while(tmp_neg_corr1->next!=NULL){                                             //go to the last complex (node) of the negative correlations of Complex1
//       tmp_neg_corr1=tmp_neg_corr1->next;
//     }
//
//     tmp_neg_corr1->next=Complex2->head_neg;                                       //append the negative correlations of Complex2 to Complex1 negative correlations
//   }
//
//   Complex2->head_neg=NULL;
//
//   return;
//
//
// }

// this function deletes one id from its complex
// knowing that a complex has at least one non-NULL node
void delete_complex_node(Hashed_Id IdNode){

  complex tmp_complex1=NULL;
  complex tmp_complex2=NULL;

  tmp_complex1=IdNode->Complex->head->next;
  tmp_complex2=IdNode->Complex->head;

  if(strcmp(IdNode->Complex->head->id,IdNode->full_id)==0){
    free(IdNode->Complex->head->id) ;
    free(IdNode->Complex->head) ;
    IdNode->Complex->head=tmp_complex1 ;
  }
  else{
    while(strcmp(tmp_complex1->id,IdNode->full_id)!=0){
      tmp_complex2=tmp_complex1;
      tmp_complex1=tmp_complex1->next;

    }
    tmp_complex2->next=tmp_complex1->next ;
    free(tmp_complex1->id) ;
    free(tmp_complex1) ;
  }

  if(IdNode->Complex->head==NULL){
    delete_negatives(IdNode->Complex) ;

    free(IdNode->Complex) ;
    IdNode->Complex=NULL ;
  }

}


/*----------specs_list funcs implementations---------*/
specs new_attribute(Hashed_Id IdNode,char* attr){
  // attr[0]=' ' ;
  // attr[strlen(attr)-1]=' ' ;
  trim(attr) ;

  specs currSpec=IdNode->Specs ;

  if(IdNode->Specs==NULL){
    IdNode->Specs=malloc(sizeof(struct Spec_node)) ;
    IdNode->Specs->next=NULL ;
    IdNode->Specs->values=NULL ;
    IdNode->Specs->attribute=malloc(strlen(attr)+1) ;
    strcpy(IdNode->Specs->attribute,attr) ;

    return IdNode->Specs ;
  }
  else{
    while(currSpec->next!=NULL) currSpec=currSpec->next ;
    currSpec->next=malloc(sizeof(struct Spec_node)) ;
    currSpec->next->next=NULL ;
    currSpec->next->values=NULL ;
    currSpec->next->attribute=malloc(strlen(attr)+1) ;
    strcpy(currSpec->next->attribute,attr) ;

    return currSpec->next ;
  }

}

void push_value(specs spec,char* value){
  // value[0]=' ' ;
  // value[strlen(value)-1]=' ' ;
  trim(value) ;

  str_list currVal=spec->values ;
  if(value[strlen(value)-1]==',') value[strlen(value)-1]='\0' ;

  if(spec->values==NULL){
    spec->values=malloc(sizeof(struct str_node)) ;
    spec->values->next=NULL ;
    spec->values->str=malloc(strlen(value)+1) ;
    strcpy(spec->values->str,value) ;

  }
  else{
    while(currVal->next!=NULL) currVal=currVal->next ;
    currVal->next=malloc(sizeof(struct str_node)) ;
    currVal->next->next=NULL ;
    currVal->next->str=malloc(strlen(value)+1) ;
    strcpy(currVal->next->str,value) ;

  }
}

void delete_specs_list(Hashed_Id IdNode){
  specs tmp_spec=NULL;
  specs tmp_spec2=NULL;
  str_list tmp_val=NULL;
  str_list tmp_val2=NULL;

  tmp_spec=IdNode->Specs;

  while(tmp_spec!=NULL){                      //delete spec list of this id

    tmp_spec2=tmp_spec->next;
    free(tmp_spec->attribute);
    tmp_spec->attribute=NULL;

    tmp_val=tmp_spec->values;

    while(tmp_val!=NULL){

      tmp_val2=tmp_val->next;
      free(tmp_val->str);
      free(tmp_val);

      tmp_val=tmp_val2;
    }

    free(tmp_spec);
    tmp_spec=tmp_spec2;

  }
}
/*----------specs_list funcs implementations---------*/
