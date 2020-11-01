typedef struct str_node* str_list ;
struct str_node{
  char* str ;

  str_list next ;
};

typedef struct Spec_node* specs ;
struct Spec_node{
  char* attribute ;
  str_list values ;

  specs next ;
};
