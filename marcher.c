#include "marcher.h"

/**
 * Input:
 *  - im:   The image we are working on. (Look at ImgUtils.h for struct defn.)
 * 
 *  - weightFunc: 
 *      This is the weight *function*. You are supposed to use this to find 
 *      the energy required for each step by the Pixel Marcher. The function 
 *      takes in the 1-D coordinates of 2 pixels[*] and returns a double 
 *      representing the cost to go from pixel a to pixel b
 *             usage: `double cost = weight(im, a, b)`
 *
 *  [*] In memory, all the pixels are stored in an array (since memory is 1-D). 
 *  Given a pixel with the coordinates (x,y), we can compute it's 'pixel index' 
 *  (an integer) as follows:
 *
 *        int pixelIndex = x + y * (image width) = x + y * im->sx
 *
 * For sake of not having to deal with tuples of (x,y) coordinates (since this
 * is hard in C), we will just deal with the index of a pixel from now on. If 
 * needed, you can extract the (x,y) coords from the index as follows:
 *
 *          x = pixelIndex % im->sx
 *          y = pixelIndex / im->sx   (integer division)
 *
 * In general, the energy needed to go from pixel a->b is not the same as the 
 * energy needed to go from pixel b->a. *We will only deal with non-negative
 * costs*.
 *
 *  Requirements:
 *    - Find a least-energy path from pixel (0,0) to pixel (sx-1, sy-1), and
 *      store it in `path`. (sx,sy) are the x/y dimensions of the image. (These
 *      are stored in 'im') [**]
 *    - Find the energy needed by the above path, and return it. This answer 
 *      should be unique, even if there are multiple shortest paths.
 *
 *  From each pixel, it is possible to step to at most 4 other pixels, namely 
 *  the ones on it's top, right, bottom and left. All of these steps may
 *  require different amounts of energy, and you have to use the given weight
 *  function to compute these.
 *
 * 
 *  [**] You should store the resulting least-energy path in the array `path`,
 *       such that
 *      - path[0]    = start location = index of (0,0)
 *      - path[i]    = the i-th node along the shortest path.
 *      - path[L-1]  = end location = index of (sx-1, sy-1)
 *      - path[L]    =     -1   // To mark the end of the path
 *   where `L` is the number of nodes in your shortest path. Any valid
 *  shortest path is fine. You may assume there is already a valid amount of
 *  space allocated for the `path` array.
 *
 *  Each test case in the ones provided shouldn't take more than 2 seconds on
 *  the lab machines in BV473. (You can test remotely through SSH.)
 */
double findPath(Image *mp, WeightFunc weight, int path[])
{

  path[0] = -1; // Terminate path

  MinHeap *minHeap = newMinHeap(mp->sx + (mp->sy * mp->sx));
  int pixelIndex;

  for (int i = 0; i < (mp->sx + (mp->sy * mp->sx)); i++)
  {
    if ((i % mp->sx) == 0 && (i / mp->sx) == 0)
    {
      heapPush(minHeap, i, 0.0);
    }
    else
    {
      heapPush(minHeap, i, INFINITY);
    }
  }

  int parentArray[mp->sx + (mp->sy * mp->sx)];
  double priority;
  int endPixelIndex;

  double pathWeight;
  while (minHeap->numItems != 0)
  {
    pixelIndex = heapExtractMin(minHeap, &priority);

    int sx = pixelIndex % mp->sx;
    int sy = pixelIndex / mp->sx;

    if (sx == mp->sx - 1 && sy == mp->sy - 1)
    {
      pathWeight = priority;
      endPixelIndex = pixelIndex;
      break;
    }
    int value;
    double totalPriority;
    if (sx > 0)
    {
      value = (sx - 1) + sy * mp->sx;
      totalPriority = priority + weight(mp, pixelIndex, value);
      if (minHeap->indices[value] != -1)
      {
        if (totalPriority < minHeap->arr[minHeap->indices[value]].priority)
        {
          heapDecreasePriority(minHeap, value, totalPriority);
          parentArray[value] = pixelIndex;
        }
      }
    }

    if (sy > 0)
    {
      value = sx + (sy - 1) * mp->sx;
      totalPriority = priority + weight(mp, pixelIndex, value);
      if (minHeap->indices[value] != -1)
      {
        if (totalPriority < minHeap->arr[minHeap->indices[value]].priority)
        {
          heapDecreasePriority(minHeap, value, totalPriority);
          parentArray[value] = pixelIndex;
        }
      }
    }

    if (sx < mp->sx - 1)
    {
      value = (sx + 1) + sy * mp->sx;
      totalPriority = priority + weight(mp, pixelIndex, value);
      if (minHeap->indices[value] != -1)
      {
        if (totalPriority < minHeap->arr[minHeap->indices[value]].priority)
        {
          heapDecreasePriority(minHeap, value, totalPriority);
          parentArray[value] = pixelIndex;
        }
      }
    }

    if (sy < mp->sy - 1)
    {
      value = sx + (sy + 1) * mp->sx;
      totalPriority = priority + weight(mp, pixelIndex, value);
      if (minHeap->indices[value] != -1)
      {
        if (totalPriority < minHeap->arr[minHeap->indices[value]].priority)
        {
          heapDecreasePriority(minHeap, value, totalPriority);
          parentArray[value] = pixelIndex;
        }
      }
    }
  }

  int pathSize = 0;

  pixelIndex = endPixelIndex;

  while (pixelIndex != 0)
  {
    pixelIndex = parentArray[pixelIndex];
    pathSize++;
  }

  // when 2 nodes pathSize = 1

  path[pathSize + 1] = -1;
  path[pathSize] = endPixelIndex;

  for (int i = pathSize - 1; i >= 0; i--)
  {
    path[i] = parentArray[path[i + 1]];
  }

  return pathWeight; // Replace with cost pf shortest path
}

/**
 *  Input:
 *    - im:  The image we are working on.
 *    - a,b: Two (arbitrary, and valid) pixel 'indices' (look above, [*]),
 *           where you want to return the weight from pixel a to pixel b
 *
 *  Requirements:
 *
 *  Define your own weight function here so that when "25colours.ppm" is run
 *  with this function, the least-energy path in the image satisfies the
 *  following constraints:
 *
 *   (1) The least energy path must visit every one of the 25 colours in
 *       the image. The order in which the path visits these colours does *not*
 *       matter, as long as it visits them all. Be careful - missing even one 
 *       colour will result in 0 for this function.
 *
 *   (2) The path can stay on one colour for as many steps as necessary,
 *       however once the path leaves a colour, it can NEVER go through another
 *       pixel of the same colour again. (Said in another way, it can only
 *       enter/exit each coloured box once)
 *
 *   (3) For any two given pixels, the energy required to step between them 
 *       *must* be non-negative. If you have negative energies, this function
 *       may not work as intended.
 *
 *    There is no restriction on path length, it can be as long or as short as
 *  needed - as long as it satisfies the conditions above. Also, the amount of
 *  energy to step from 'a' to 'b' does not have to be the same as the energy
 *  to step from 'b' to 'a'. This is left up to you.
 *
 *  NOTE 1: This weight function will NOT be tested with your solution to the 
 *     first part of the question. This will be passed into my code and should
 *     still produce the results as above, so do not try to change your other 
 *     methods to help with this.
 *
 *  NOTE 2: This function will be tested ONLY on the specified image, so you do
 *     not have to worry about generalizing it. Just make sure that it does not
 *     depend on anything else in your code other than the arguments passed in.
 */
double allColourWeight(Image *im, int a, int b)
{

  if (4000 <= a && a <= 4199)
  {
    if (4179 <= a)
    {
      if (b == a + 200)
        return 0.0;
      else
        return 1;
    }
    else
    {
      if (b == a + 1)
        return 0.0;
      else
        return 1;
    }
  }

  if (12000 <= a && a <= 12199)
  {
    if (a <= 12020)
    {
      if (b == a + 200)
        return 0.0;
      else
        return 1;
    }
    else
    {
      if (b == a - 1)
        return 0.0;
      else
        return 1;
    }
  }

  if (20000 <= a && a <= 20199)
  {
    if (20179 <= a)
    {
      if (b == a + 200)
        return 0.0;
      else
        return 1;
    }
    else
    {
      if (b == a + 1)
        return 0.0;
      else
        return 1;
    }
  }

  if (28000 <= a && a <= 28199)
  {
    if (a <= 28020)
    {
      if (b == a + 200)
        return 0.0;
      else
        return 1;
    }
    else
    {
      if (b == a - 1)
      {
        return 0.0;
      }
      else
        return 1;
    }
  }

  if (36000 <= a && a <= 36199)
  {
    if (b == a + 1)
      return 0.0;
    else
      return 1;
  }

  return 1; // Default return value
}
