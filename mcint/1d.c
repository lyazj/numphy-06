#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NTRAIL 1000

static void do_trail(int total);
static double do_int(int total);

int main(void)
{
  srand48(0);
  do_trail(10);
  do_trail(100);
  do_trail(1000);
  do_trail(10000);
  do_trail(100000);
  do_trail(1000000);
  return 0;
}

void do_trail(int total)
{
  int t;

  printf("%d:\n", total);
  fprintf(stderr, "%d points:\n", total);
  for(t = 0; t < NTRAIL; ++t)
  {
    printf("%+25.17e\n", do_int(total));
    if((t + 1) % (NTRAIL / 10) == 0)
      fprintf(stderr, "%4d / %d\n", t + 1, NTRAIL);
  }
}

double do_int(int total)
{
  double x, y;
  int p, n = 0;

  for(p = 0; p < total; ++p)
  {
    x = drand48();
    y = drand48();
    n += (y < exp(-100.0 * pow(x - 0.5, 2)));
  }
  return (double)n / total;
}
