#ifndef __MARCHER_H__
#define __MARCHER_H__

#include "imgutils.h"
#include "minheap.h"

// You don't need to understand this syntax, but it essentially
// defines `WeightFunc` to be a function which expects an Image
// pointer and two pixel coordinates, and returns a double.
typedef double (*WeightFunc)(Image *im, int a, int b);

double findPath(Image *im, WeightFunc weight, int path[]);
double allColourWeight(Image *im, int a, int b);

#endif
