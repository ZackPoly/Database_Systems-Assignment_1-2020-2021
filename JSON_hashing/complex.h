typedef struct Comp_node* complex ;
struct Comp_node{
  char* id ;

  complex next ;
};

typedef struct Negative_Correlation* neg_corr ;
struct Negative_Correlation ;

typedef struct Head_Of_Complex* comp_head ;
struct Head_Of_Complex{
  complex head ;
  complex tail ;

  neg_corr head_neg ;
};

// typedef struct Negative_Correlation* neg_corr ;
struct Negative_Correlation{
  comp_head corr ;

  neg_corr next ;
};
