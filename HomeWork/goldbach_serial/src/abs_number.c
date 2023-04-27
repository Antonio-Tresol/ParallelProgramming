// Copyright  2022 A.Badilla Olivas GNU GENERAL PUBLIC LICENSE
#include "abs_number.h"
uint64_t abs_number(int64_t number) {
  if ( number < 0 ) {
    return number * -1;
  } else {
    return number;
  }
}
