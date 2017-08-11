#ifndef HASH_H
#define HASH_H
#define MAX_TAM_BK 5

typedef struct _BUCKET BUCKET;
typedef struct _DIR_CELL DIR_CELL;
typedef struct _DIRETORIO DIRETORIO;

void start();
int make_address(int key, int prof);
int op_add(int key);
void bk_split(BUCKET *bucket);
void print_dir();
void find_new_range(struct _BUCKET *bucket, int *new_start, int *new_end);
void dir_ins_bucket(struct _BUCKET *bucket, int start, int end);

#endif
