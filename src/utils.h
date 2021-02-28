#include <stdlib.h>

template <typename T, size_t N> 
T array_max( T (&arr) [N] )
{
  T array_max = arr[0];
  for (unsigned short i = 0; i < N; i++)
    array_max = (arr[i] < array_max) ? array_max : arr[i];
  return array_max;
}

template <typename T, size_t N> 
T array_min( T (&arr) [N] )
{
  T array_min = arr[0];
  for (unsigned short i = 0; i < N; i++)
    array_min = (arr[i] > array_min) ? array_min : arr[i];
  return array_min;
}

template <typename T, size_t N> 
short array_len( T (&arr) [N] )
{
  return N;
}

template <typename T, size_t N> 
T array_sum( T (&arr) [N] )
{
  T array_sum = 0;
  for (unsigned short i = 0; i < N; i++)
    array_sum += arr[i]; 
  return array_sum;
}

template <typename T, size_t N> 
T array_mean( T (&arr) [N] )
{
  return array_sum(arr) / array_len(arr);
}

template <typename T, size_t N>
void array_center( T (&arr) [N] )
{
  T mu = array_mean(arr);
  for (unsigned short i = 0; i < N; i++) {
    arr[i] = arr[i] - mu;
  }
}