// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include "rt/world/uuid.h"

namespace rt {

uint64_t Uuid() {
  static uint64_t id = 0;
  return id++;
}

}  // namespace rt