#pragma once

namespace jhu::thrax {

using IndexType = int16_t;

struct Span {
  const IndexType start = 0;
  const IndexType end = 0;
};

struct SpanPair {
  const Span src, tgt;
};

}
