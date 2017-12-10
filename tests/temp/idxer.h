#ifndef IDXER_H
#define IDXER_H

#include "../../mth.h"

struct Idxer{
  inline static UINT
  row(UINT i, UINT n){ UINT x=(UINT)i/n; return x; };
  inline static UINT
  col(UINT i, UINT n){ UINT x=(UINT)i/n; UINT y=i-x*n; return y; };
  inline static UINT
  idx(UINT x, UINT y, UINT n){ return x*y+n; };
};


#endif
