#include <stdio.h>
#include "func_to_test.h"

// main funcion
int main()
{
  int res1 = Factorial(5);
  int res2 = IsPrime(4);
  double res3 = squareRoot(36.0);

  printf("Factorial(5)=%d, IsPrime(4)=%d, squareRoot(36.0)=%f\n", res1, res2, res3);

  return 0;
}
