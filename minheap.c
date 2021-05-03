#include "minheap.h"

/**
 * Allocate a new min heap of the given size.
 */
MinHeap *newMinHeap(int size)
{

  MinHeap *newMinHeap = calloc(sizeof(MinHeap), 1);
  newMinHeap->numItems = 0;
  newMinHeap->maxSize = size;

  newMinHeap->arr = calloc(sizeof(HeapElement), size);
  newMinHeap->indices = calloc(sizeof(int), size);

  for (int i = 0; i < size; i++)
    newMinHeap->indices[i] = -1;

  return newMinHeap; // Allocate and return heap.
}

/**
 * Swaps elements at indices `a` and `b` in the heap, and also updates their
 * indices. Assumes that `a` and `b` are valid.
 * 
 */
void swap(MinHeap *heap, int a, int b)
{
  // Swap the elements
  HeapElement temp = heap->arr[a];
  heap->arr[a] = heap->arr[b];
  heap->arr[b] = temp;

  // Refresh their indices
  heap->indices[heap->arr[a].val] = a;
  heap->indices[heap->arr[b].val] = b;
}

void percolate(MinHeap *heap, HeapElement heapElement)
{
  while (heap->indices[heapElement.val] != 0)
  {
    int indexOfParent = floor((heap->indices[heapElement.val] - 1) / 2);
    if (heapElement.priority < heap->arr[indexOfParent].priority)
    {
      swap(heap, indexOfParent, heap->indices[heapElement.val]);
    }
    else
    {
      break;
    }
  }
}

/**
 * Add a value with the given priority into the heap.
 * 
 */
void heapPush(MinHeap *heap, int val, double priority)
{

  HeapElement heapElement;
  heapElement.priority = priority;
  heapElement.val = val;

  heap->arr[heap->numItems] = heapElement;
  heap->indices[heapElement.val] = heap->numItems;
  heap->numItems++;
  percolate(heap, heapElement);

  return; // Push value to heap before returning
}

void heapify(MinHeap *heap, int heapStartIndex)
{
  int i = heapStartIndex;

  if (i < heap->numItems && (2 * i) + 1 < heap->numItems && (2 * i) + 2 < heap->numItems)
  {
    if (heap->arr[i].priority < heap->arr[(2 * i) + 1].priority &&
        heap->arr[i].priority < heap->arr[(2 * i) + 2].priority)
    {
      return;
    }
    else if (heap->arr[(2 * i) + 1].priority < heap->arr[(2 * i) + 2].priority)
    {
      swap(heap, i, (2 * i) + 1);
      heapify(heap, (2 * i) + 1);
    }
    else
    {
      swap(heap, i, (2 * i) + 2);
      heapify(heap, (2 * i) + 2);
    }
  }
  else if (i < heap->numItems && (2 * i) + 1 < heap->numItems)
  {
    if (heap->arr[(2 * i) + 1].priority < heap->arr[i].priority)
    {
      swap(heap, i, (2 * i) + 1);
      heapify(heap, (2 * i) + 1);
    }
    else
    {
      return;
    }
  }
  else
  {
    return;
  }
}
/**
 * Extract and return the value from the heap with the minimum priority. Store
 *  the priority for this value in `*priority`. 
 * 
 * For example, if `10` was the value with the lowest priority of `1.0`, then
 *  you would have something that is equivalent to:
 *      
 *        *priority = 1.0;
 *        return 10;
 */
int heapExtractMin(MinHeap *heap, double *priority)
{
  int returnValue = heap->arr[0].val;
  *priority = heap->arr[0].priority; // Set correct priority

  swap(heap, 0, heap->numItems - 1);

  heap->indices[heap->arr[heap->numItems - 1].val] = -1;
  heap->numItems--;

  heapify(heap, 0);

  return returnValue; // Return correct value
}

/**
 * Decrease the priority of the given value (already in the heap) with the
 * new priority.
 * 
 * NOTE: You will find it helpful here to first get the index of the value
 *       in the heap from the `indices` array.
 * 
 */
void heapDecreasePriority(MinHeap *heap, int val, double priority)
{
  heap->arr[heap->indices[val]].priority = priority;

  percolate(heap, heap->arr[heap->indices[val]]);
  return; // Decrease priority before return
}

/**
 * Free the data for the heap. This won't be marked, but it is always good
 * practice to free up after yourself when using a language like C.
 */
void freeHeap(MinHeap *heap)
{
  free(heap->arr);
  free(heap->indices);
  free(heap);
  return; // Free heap and it's arrays
}
