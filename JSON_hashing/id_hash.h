#include "hash.h"

Hashed_Id search_id_in_hash(Hash_For_Id,char*,int) ;

                                    //  id   file-data
Hashed_Id insert_id_in_hash(Hash_For_Id,int,char*,FILE*,int) ;

void append_negative(comp_head,comp_head) ;

int search_negative(comp_head,comp_head) ;

void delete_negatives(comp_head) ;

void change_negatives(comp_head,comp_head);

void delete_complex_node(Hashed_Id) ;


specs new_attribute(Hashed_Id,char*) ;

void push_value(specs,char*) ;
