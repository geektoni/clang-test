//
// Created by uriel on 27/09/16.
//

#include "Result.h"

Result * Result::inst = 0;

Result * Result::instance() {
  return inst ? inst : inst = new Result();
}