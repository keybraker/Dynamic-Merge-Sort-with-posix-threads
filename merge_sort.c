#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

struct merge_argmnt
{ 
  int **A;
  int n,m,sort_by;
};

struct merge_argmnt *
crt_new(int **A, int n, int m, int sort_by)
{
  struct merge_argmnt* new = (struct merge_argmnt*) 
  malloc (sizeof(struct merge_argmnt));

  new->A = A;
  new->n = n;
  new->m = m;
  new->sort_by = sort_by;

  return new;
}

void
printer(int n, int m, int** A, char *name)
{
  int i,j;
  printf("%s\n", name);  
  for(i=0;i<n;i++){
    printf("|\t");
    for(j=0;j<m;j++)
      printf("%d\t|\t", A[i][j]);
    printf("\n");
  }
}

/********************************************/

void
merge(int **A, int **L, int **R, int n, int m,
 int sort_by, int leftCount, int rightCount)
{
  int i,j,k,l;
  i = j = k = l = 0;
 
  while(i<leftCount && j<rightCount) {
   
    if(L[i][sort_by] < R[j][sort_by])
    {
      for(l = 0;l<m;l++) A[k][l] = L[i][l];
      k++; i++;
    }
    else
    {
      for(l = 0;l<m;l++) A[k][l] = R[j][l];
      k++; j++;
    }
  }
  
  while(i < leftCount)
  {
    for(l = 0;l<m;l++) A[k][l] = L[i][l];
    k++; i++;
  }
  
  while(j < rightCount)
  {
    for(l = 0;l<m;l++) A[k][l] = R[j][l];
    k++; j++;
  }
}

void* 
merge_sort(void *input) 
{
  struct merge_argmnt *reader = input;

  int **A = reader->A;
  int n = reader->n;
  int m = reader->m;
  int sort_by = reader->sort_by;

  int mid, nmid, i, j;

  if(n < 2) return NULL; // array cannot be broken any further
  mid = n/2; // mid is the first half of the array 
  
  nmid = n-mid;

  /* Creating the two half sized arrays with size mid and n-mid */
  int **L = (int **) malloc (mid * sizeof(int*));
  int **R = (int **) malloc (nmid * sizeof(int*)); 

  for (i=0; i<mid; i++){
    L[i] = (int *) malloc (m * sizeof(int));
    /* Initiallizing everything with zero */
    for(j = 0;j<m;j++)
      L[i][j] = 0;
  }

  for (i=0; i<n-mid; i++){
    R[i] = (int *) malloc (m * sizeof(int));
    for(j = 0;j<m;j++)
      /* Initiallizing everything with zero */
      R[i][j] = 0;
  }

  for(i = 0;i<mid;i++)
    for(j = 0;j<m;j++)
      L[i][j] = A[i][j];
    
  for(i = mid;i<n;i++)
    for(j = 0;j<m;j++)
      R[i-mid][j] = A[i][j];
    
  struct merge_argmnt* merge_args_a = crt_new(L, mid, m, sort_by);
  struct merge_argmnt* merge_args_b = crt_new(R, n-mid, m, sort_by);
  
  pthread_t thread_mergesort_a;
  pthread_t thread_mergesort_b;
  
  if(pthread_create(&thread_mergesort_a, NULL, merge_sort, merge_args_a)) return NULL;
  if(pthread_create(&thread_mergesort_b, NULL, merge_sort, merge_args_b)) return NULL;

  if(pthread_join(thread_mergesort_a, NULL)) return NULL;
  if(pthread_join(thread_mergesort_b, NULL)) return NULL;

  merge(A, L, R, n, m, sort_by, mid, n-mid);  

  free(L);
  free(R);
}

/********************************************/


int
main(int argc, char **argv)
{
  int i, j, n, m, sort_by;
  
  n = 20; /*number of rows*/
  m = 5; /*number of columns*/
  sort_by = 0; /*column based on which sorting will happen*/

  srand(time(NULL));  

  int **A = (int **) malloc (n * sizeof(int*));
  for (i=0; i<n; i++)
    A[i] = (int *) malloc (m * sizeof(int));

  for(i=0;i<n;i++)
    for(j=0;j<m;j++)
      A[i][j]=rand() % 100 + 1;
  
  printer(n,m,A,"Before");

  merge_sort(crt_new(A, n, m, sort_by));
 
  printer(n,m,A,"After");

  return 0;
}