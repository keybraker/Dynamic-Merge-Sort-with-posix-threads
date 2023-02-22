#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

#define SEC2MS(x) (x * 1000)

struct merge_argument
{
  int **A;
  int n, m, sort_by;
};

struct merge_argument *
crt_new(int **A, int n, int m, int sort_by)
{
  struct merge_argument *new = (struct merge_argument *)
      malloc(sizeof(struct merge_argument));

  new->A = A;
  new->n = n;
  new->m = m;
  new->sort_by = sort_by;

  return new;
}

double
get_ms(struct timeval start_time_)
{
  struct timeval end_time;
  int rc = gettimeofday(&end_time, NULL);
  assert(rc == 0);
  return SEC2MS((end_time.tv_sec - start_time_.tv_sec + (double)(end_time.tv_usec - start_time_.tv_usec) / 1e6));
}

void printer(int n, int m, int **A, char *name)
{
  int i, j;
  printf("%s\n", name);
  for (i = 0; i < n; i++)
  {
    printf("|\t");
    for (j = 0; j < m; j++)
      printf("%d\t|\t", A[i][j]);
    printf("\n");
  }
}

void merge(int **A, int **L, int **R, int n, int m,
           int sort_by, int leftCount, int rightCount)
{
  int i, j, k, l;
  i = j = k = l = 0;

  while (i < leftCount && j < rightCount)
  {

    if (L[i][sort_by] < R[j][sort_by])
    {
      for (l = 0; l < m; l++)
        A[k][l] = L[i][l];
      k++;
      i++;
    }
    else
    {
      for (l = 0; l < m; l++)
        A[k][l] = R[j][l];
      k++;
      j++;
    }
  }

  while (i < leftCount)
  {
    for (l = 0; l < m; l++)
      A[k][l] = L[i][l];
    k++;
    i++;
  }

  while (j < rightCount)
  {
    for (l = 0; l < m; l++)
      A[k][l] = R[j][l];
    k++;
    j++;
  }
}

void *
merge_sort(void *input)
{
  struct merge_argument *reader = input;

  int **A = reader->A;
  int n = reader->n;
  int m = reader->m;
  int sort_by = reader->sort_by;

  int mid, nmid, i, j;

  if (n < 2)
    return NULL; // array cannot be broken any further
  mid = n / 2;   // mid is the first half of the array

  nmid = n - mid;

  /* Creating the two half sized arrays with size mid and n-mid */
  int **L = (int **)malloc(mid * sizeof(int *));
  int **R = (int **)malloc(nmid * sizeof(int *));

  for (i = 0; i < mid; i++)
  {
    L[i] = (int *)malloc(m * sizeof(int));
    /* Initializing everything with zero */
    for (j = 0; j < m; j++)
      L[i][j] = 0;
  }

  for (i = 0; i < n - mid; i++)
  {
    R[i] = (int *)malloc(m * sizeof(int));
    for (j = 0; j < m; j++)
      /* Initializing everything with zero */
      R[i][j] = 0;
  }

  for (i = 0; i < mid; i++)
    for (j = 0; j < m; j++)
      L[i][j] = A[i][j];

  for (i = mid; i < n; i++)
    for (j = 0; j < m; j++)
      R[i - mid][j] = A[i][j];

  merge_sort(crt_new(L, mid, m, sort_by));
  merge_sort(crt_new(R, n - mid, m, sort_by));

  merge(A, L, R, n, m, sort_by, mid, n - mid);

  free(L);
  free(R);

  return NULL;
}

int main(int argc, char **argv)
{
  struct timeval start_time;
  int i, j, n, m, sort_by;

  n = 30000;   /*number of rows*/
  m = 3000;    /*number of columns*/
  sort_by = 0; /*column based on which sorting will happen*/

  srand(time(NULL));

  int rc = gettimeofday(&start_time, NULL);
  assert(rc == 0);

  int **A = (int **)malloc(n * sizeof(int *));
  for (i = 0; i < n; i++)
    A[i] = (int *)malloc(m * sizeof(int));

  for (i = 0; i < n; i++)
    for (j = 0; j < m; j++)
      A[i][j] = rand() % 100 + 1;

  // printer(n,m,A,"Before");

  merge_sort(crt_new(A, n, m, sort_by));

  // printer(n,m,A,"After");

  printf("Elapsed time: %f sec\n", get_ms(start_time) / 1000);

  return 0;
}