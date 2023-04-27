//  Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include <omp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t count = 0;
  char charsToWritters = '\0';
  #pragma omp parallel shared(count)
  { // NOLINT (readability/braces)
    while (scanf("%c", &charsToWritters) == 1) {
      if (toupper(charsToWritters) == 'R') {
        printf("Reader got %zu\n", count);
      } else if (toupper(charsToWritters) == 'W') {
        #pragma omp critical
        count++;
        printf("Writer increased to %zu\n", count);
      }  
    }
  }
  return 0;
}
