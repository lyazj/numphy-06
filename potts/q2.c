#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __GNUC__
# define likely(e)   (__builtin_expect((e), 1))
# define unlikely(e) (__builtin_expect((e), 0))
#else  /* __GNUC__ */
# ifndef __attribute__
#  define __attribute__(discarded)
# endif  /* __attribute__ */
# define likely(e)   (e)
# define unlikely(e) (e)
#endif  /* __GNUC__ */

static unsigned rand48(unsigned u)
{
  return drand48() * u;
}

static void linit(int q, int n, int L[n][n], int *Ep, int *Mp)
{
  int E = 0;
  int M = -n * n * (q - 1) / 2;
  int i, j;

  L[0][0] = rand48(q);
  M += L[0][0];
  for(j = 1; j < n; ++j)
  {
    L[0][j] = rand48(q);
    E -= L[0][j] == L[0][j - 1];
    M += L[0][j];
  }
  for(i = 1; i < n; ++i)
  {
    L[i][0] = rand48(q);
    E -= L[i][0] == L[i - 1][0];
    M += L[i][0];
    for(j = 1; j < n; ++j)
    {
      L[i][j] = rand48(q);
      E -= L[i][j] == L[i][j - 1];
      E -= L[i][j] == L[i - 1][j];
      M += L[i][j];
    }
  }
  *Ep = E;
  *Mp = M;
}

static void lmove(int n, int L[n][n], int i, int j, int v, int *Ep, int *Mp, double T)
{
  int E = *Ep;
  int M = *Mp;

  if(likely(i > 0))
    E -= (v == L[i - 1][j]) - (L[i][j] == L[i - 1][j]);
  if(likely(i < n - 1))
    E -= (v == L[i + 1][j]) - (L[i][j] == L[i + 1][j]);
  if(likely(j > 0))
    E -= (v == L[i][j - 1]) - (L[i][j] == L[i][j - 1]);
  if(likely(j < n - 1))
    E -= (v == L[i][j + 1]) - (L[i][j] == L[i][j + 1]);
  if(E <= *Ep || drand48() < exp((*Ep - E) / T))
  {
    M += v - L[i][j];
    L[i][j] = v;
    *Ep = E;
    *Mp = M;
  }
}

static void ltrans(int q, int n, int L[n][n], int *Ep, int *Mp, double T)
{
  int i, j, v;

  i = rand48(n);
  j = rand48(n);
  v = rand48(q);
  if(L[i][j] != v)
    lmove(n, L, i, j, v, Ep, Mp, T);
}

static void lsim(int q, int n, int L[n][n], int *Ep, int *Mp, double T,
                 int cycles, double *Emp, double *Mmp)
{
  int c;
  double sE;
  double sM;

  sE = 0;
  sM = 0;
  for(c = 0; c < cycles; ++c)
  {
    ltrans(q, n, L, Ep, Mp, T);
    sE += *Ep;
    sM += *Mp;
  }
  *Emp = sE / cycles;
  *Mmp = sM / cycles;
}

__attribute__((used))
static void lshow(int n, int L[n][n], int E)
{
  int i, j;

  for(i = 0; i < n; ++i)
  {
    for(j = 0; j < n; ++j)
      printf("%2d", L[i][j]);
    printf("\n");
  }
  printf("%d\n", E);
}

__attribute__((used))
static void ltest(void)
{
  int q = 2;
  int n = 4;
  int L[n][n];
  int E;
  int M;

  linit(q, n, L, &E, &M);
  lshow(n, L, E);
  lmove(n, L, 0, 0, 1, &E, &M, +INFINITY);
  lshow(n, L, E);
  lmove(n, L, 0, 3, 0, &E, &M, +INFINITY);
  lshow(n, L, E);
  lmove(n, L, 3, 0, 0, &E, &M, +INFINITY);
  lshow(n, L, E);
  lmove(n, L, 3, 0, 0, &E, &M, +INFINITY);
  lshow(n, L, E);
  lmove(n, L, 3, 3, 0, &E, &M, +INFINITY);
  lshow(n, L, E);
  lmove(n, L, 1, 1, 0, &E, &M, +INFINITY);
  lshow(n, L, E);
}

static void lproc(int q, int n, double TL, double TH, int NT, int cycles)
{
  double T0, T1;
  int IT;
  double E0, E1, C, M0, M1, m;
  int L[n][n];
  int E, M;

  fprintf(stderr, "Executing: %s(%d, %d, %lf, %lf, %d, %d)\n",
          __func__, q, n, TL, TH, NT, cycles);
  srand48(1428576);
  linit(q, n, L, &E, &M);
  T0 = TL;
  lsim(q, n, L, &E, &M, T0, cycles, &E0, &M0);
  printf("%d\n", n);
  printf("%16s%16s%16s%16s%16s\n", "T", "E", "C", "M", "m");
  for(IT = 1; IT <= NT + 1; ++IT)
  {
    T1 = TL + (TH - TL) * IT / NT;
    lsim(q, n, L, &E, &M, T1, cycles, &E1, &M1);
    C = (E1 - E0) / (T1 - T0);
    m = (M1 - M0) / (T1 - T0);
    printf("%16lf%16lf%16lf%16lf%16lf\n", T0, E0, C, M0, m);
    T0 = T1;
    E0 = E1;
    M0 = M1;
    fprintf(stderr, "%4d / %4d\r", IT, NT);
  }
  fprintf(stderr, "%-11s\n", "Finished.");
  printf("\n");
}

int main(void)
{
  lproc(2, 10, 0.5, 1.5, 100, 10000000);
  lproc(2, 40, 0.5, 1.5, 100, 10000000);
  lproc(2, 80, 0.5, 1.5, 100, 10000000);
  return 0;
}
