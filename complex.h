typedef struct Comp_node* complex ;
struct Comp_node{
  char* id ;

  complex next ;
};

typedef struct Head_Of_Complex* comp_head ;
struct Head_Of_Complex{
  complex head ;
};
