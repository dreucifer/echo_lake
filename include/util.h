#ifndef UTIL_H
#define UTIL_H

#define VEC2(x, y) = (union vector2){x, y}

union vector2 {
  struct {
    int x, y;
  }
  int v[2];
}

#endif