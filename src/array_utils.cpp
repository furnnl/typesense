#include "array_utils.h"
#include <memory.h>

size_t ArrayUtils::and_scalar(const uint32_t *A, const size_t lenA,
                              const uint32_t *B, const size_t lenB, uint32_t **results) {
  if (lenA == 0 || lenB == 0) {
    return 0;
  }

  *results = new uint32_t[std::min(lenA, lenB)];
  uint32_t *out = *results;

  const uint32_t *const initout(out);
  const uint32_t *endA = A + lenA;
  const uint32_t *endB = B + lenB;

  while (1) {
    while (*A < *B) {
      SKIP_FIRST_COMPARE:
      if (++A == endA)
        return (out - initout);
    }
    while (*A > *B) {
      if (++B == endB)
        return (out - initout);
    }
    if (*A == *B) {
      *out++ = *A;
      if (++A == endA || ++B == endB)
        return (out - initout);
    } else {
      goto SKIP_FIRST_COMPARE;
    }
  }

  return (out - initout); // NOTREACHED
}

// merges two sorted arrays and also removes duplicates
size_t ArrayUtils::or_scalar(const uint32_t *A, const size_t lenA,
                             const uint32_t *B, const size_t lenB, uint32_t **out) {
    size_t indexA = 0, indexB = 0, res_index = 0;

    if(A == nullptr) {
        *out = new uint32_t[lenB];
        memcpy(*out, B, lenB * sizeof(uint32_t));
        return lenB;
    }

    uint32_t* results = new uint32_t[lenA+lenB];

    while (indexA < lenA && indexB < lenB) {
      if (A[indexA] < B[indexB]) {
        // check for duplicate
        if(res_index == 0 || results[res_index-1] != A[indexA]) {
          results[res_index] = A[indexA];
          res_index++;
        }
        indexA++;
      } else {
        if(res_index == 0 || results[res_index-1] != B[indexB]) {
          results[res_index] = B[indexB];
          res_index++;
        }
        indexB++;
      }
  }

  while (indexA < lenA) {
    if(res_index == 0 || results[res_index-1] != A[indexA]) {
      results[res_index] = A[indexA];
      res_index++;
    }

    indexA++;
  }

  while (indexB < lenB) {
    if(res_index == 0 || results[res_index-1] != B[indexB]) {
      results[res_index] = B[indexB];
      res_index++;
    }

    indexB++;
  }

  // shrink fit
  *out = new uint32_t[res_index];
  memcpy(*out, results, res_index * sizeof(uint32_t));
  delete[] results;

  return res_index;
}