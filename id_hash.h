#include "hash.h"

Hashed_Id search_id_in_hash(Hash_For_Id,char*,int) ;

                                    //  id   file-data
Hashed_Id insert_id_in_hash(Hash_For_Id,int,char*,FILE*) ;

void append_negative(Hashed_Id, Hashed_Id) ;

void delete_negatives(comp_head) ;

void change_negatives(comp_head,comp_head);

void delete_complex_node(Hashed_Id) ;

void delete_specs_list(Hashed_Id) ;

specs new_attribute(Hashed_Id,char*) ;

void push_value(specs,char*) ;
