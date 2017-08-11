#include "hash.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int id;

struct _BUCKET {
  int prof;
  int cont;
  int chaves[MAX_TAM_BK];
  int id;
};

struct _DIR_CELL {
  struct _BUCKET *bucket_ref;
};

struct _DIRETORIO {
  int prof;
  struct _DIR_CELL *celulas;
};

struct _DIRETORIO dir;

void start(){
  id = 0;
  dir.prof = 0;
  dir.celulas = (struct _DIR_CELL *) malloc(sizeof(struct _DIR_CELL));
  dir.celulas->bucket_ref = (struct _BUCKET*) malloc(sizeof(struct _BUCKET));
  dir.celulas->bucket_ref->id = id;
  dir.celulas->bucket_ref->prof = 0;
  dir.celulas->bucket_ref->cont = 0;
  id++;
}

void dir_double(){
  int size, new_size, i;
  struct _DIR_CELL *new_cell;
  size = pow(2, dir.prof);
  new_size = 2*size;

  new_cell = (struct _DIR_CELL *) malloc(new_size*sizeof(struct _DIR_CELL));
  for(i = 0; i < size; i++){
    new_cell[2*i].bucket_ref = dir.celulas[i].bucket_ref;
    new_cell[2*i+1].bucket_ref = dir.celulas[i].bucket_ref;
  }
  free(dir.celulas);
  dir.celulas = new_cell;
  dir.prof++;
}

int make_address(int key, int prof){
  int retval, mask, i, hashval, lowbit;
  mask = 1;
  retval = 0;
  hashval = key;
  for(i = 0; i < prof; i++ ){
    retval = retval << 1;
    lowbit = hashval & mask;
    retval = retval | lowbit;
    hashval = hashval >> 1;
  }
  return retval;
}

int op_find(int key, struct _BUCKET **found_bucket){
  int address, i;
  address = make_address(key, dir.prof);
  *found_bucket = dir.celulas[address].bucket_ref;
  for(i = 0; i < (*found_bucket)->cont; i++){
    if((*found_bucket)->chaves[i] == key){
      return 1;
    }
  }
  return 0;
}

void bk_add_key(struct _BUCKET *bucket, int key){
  if(bucket->cont < MAX_TAM_BK){
    bucket->chaves[bucket->cont] = key;
    bucket->cont++;
  }else{
    bk_split(bucket);
    op_add(key);
  }
}

void bk_split(struct _BUCKET *bucket){
  int new_start, new_end, i, aux;
  struct _BUCKET *end_novo_bucket;
  if(dir.prof == bucket->prof){
    dir_double();
  }
  end_novo_bucket = (struct _BUCKET*) malloc(sizeof(struct _BUCKET));
  end_novo_bucket->id = id;
  end_novo_bucket->cont = 0;
  id++;
  find_new_range(bucket, &new_start, &new_end);
  dir_ins_bucket(end_novo_bucket, new_start, new_end);
  bucket->prof++;
  end_novo_bucket->prof = bucket->prof;
  aux = bucket->cont;
  bucket->cont = 0;
  for(i = 0; i < aux; i++){
    op_add(bucket->chaves[i]);
  }
}

int op_add(int key){
  struct _BUCKET *found_bucket;
  if(op_find(key, &found_bucket)){
    return 0;
  }
  bk_add_key(found_bucket, key);
  return 1;
}

void find_new_range(struct _BUCKET *old_bucket, int *new_start, int *new_end){
  int shared_address, mask, new_shared, bits_to_fill, i;
  mask = 1;
  shared_address = make_address(old_bucket->chaves[old_bucket->cont - 1], old_bucket->prof);
  new_shared = shared_address << 1;
  new_shared = new_shared | mask;
  bits_to_fill = dir.prof - (old_bucket->prof + 1);
  (*new_start) = new_shared;
  (*new_end) = new_shared;
  for(i = 0; i < bits_to_fill; i++){
    (*new_start) = (*new_start) << 1;
    (*new_end) =  (*new_end) << 1;
    (*new_end) =  (*new_end) | mask;
  }
}

void dir_ins_bucket(struct _BUCKET *bucket, int start, int end){
  int i;
  for(i = start; i <= end; i++){
    dir.celulas[i].bucket_ref = bucket;
  }
}

void print_dir(){
  int i, j, size;
  size = pow(2, dir.prof);
  printf("directory:\n");
  for(i = 0; i < size; i++){
    printf("  dir[%d] = bucket #%d\n", i, dir.celulas[i].bucket_ref->id);
  }
  printf("\n");
  printf("  directory current size = %d\n", size);
  printf("  number of buckets = %d \n", id);
  printf("  depth = %d\n\n", dir.prof);
  printf("\n");
  for(i = 0; i < size; i++){
      if(i > 0 && dir.celulas[i].bucket_ref->id != dir.celulas[i-1].bucket_ref->id){
        printf("== bucket #%d ==\n", dir.celulas[i].bucket_ref->id);
        printf("  #id = %d    depth = %d\n",dir.celulas[i].bucket_ref->id, dir.celulas[i].bucket_ref->prof);
        for(j = 0; j < dir.celulas[i].bucket_ref->cont; j++){
          printf("  chave[%d] = %d\n", j, dir.celulas[i].bucket_ref->chaves[j]);
        }
        printf("\n");
      }else if(i == 0){
        printf("== bucket #%d ==\n", dir.celulas[i].bucket_ref->id);
        printf("  #id = %d    depth = %d\n",dir.celulas[i].bucket_ref->id, dir.celulas[i].bucket_ref->prof);
        for(j = 0; j < dir.celulas[i].bucket_ref->cont; j++){
          printf("  chave[%d] = %d\n", j, dir.celulas[i].bucket_ref->chaves[j]);
        }
        printf("\n");
      }
    }
}
