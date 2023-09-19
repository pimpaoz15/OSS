#include <stdio.h>
#include "func_to_test.h"

// main funcion
int main()
{
  int res1 = Factorial(5);
  int res2 = IsPrime(4);
  double res3 = squareRoot(36.0);

  double sqr1 = squareRoot(123.145);
  double sqr2 = squareRoot(63924.12356);
  double sqr3 = squareRoot(72346.18452);

  printf("Factorial(5)=%d, IsPrime(4)=%d, squareRoot(36.0)=%f\n", res1, res2, res3);
  printf("squareRoot(123.145)=%f, squareRoot(63924.12356)=%f, squareRoot(72346.18452)=%f\n", sqr1, sqr2, sqr3);

  return 0;
}
