#include <stdio.h>
#include <math.h>

void FindSumSeq( size_t n );

int main()
{
  size_t n;
  printf("input: ");
  scanf("%u", &n);
  FindSumSeq( n );
  return 0;
}

void FindSumSeq( size_t n ) {
  size_t i, N, index;
  int S, a[n];
  for( i = 0; i < n; ++i )
    scanf( "%d", &a[ i ] );
  scanf( "%d", &S );
  // переберем все возможные суммы,
  // т.е. все возможные сочетания знаков слагаемых
  N = pow(2, n);
  for(index = 0; index < N; ++index) {
    // в двоичном преставлении индекса суммы,
    // единицы соответствуют слагаемым со знаком "-",
    // а нули - слагаемым со знаком "+", например, в случае
    // входной последовательности из 3 элементов { 15, 25, 30 }:
    // для 0-й суммы (000): +15 +25 +30 = 70;
    // для 1-й суммы (001): -15 +25 +30 = 40;
    // для 2-й суммы (010): +15 -25 +30 = 20;
    // ...
    // для 7-й суммы (111): -15 -25 -30 = ;
    // всего 2^3 = 8 возможных вариантов суммирования
    int sum = 0;
    for( i = 0; i < n; ++i ) {
      if( ( index >> i ) & 1 ) {
        sum -= a[ i ];
      } else {
        sum += a[ i ];
      }
    }
    if( sum == S ) {
      break;
    }
  } // for( index )
  if( index < N ) {
    printf( "%u", ( index & 1 ) ? -a[ 0 ] : a[ 0 ] );
    for( i = 1; i < n; ++i ) {
      printf( "%+d", ( ( index >> i ) & 1 ) ? -a[ i ] : a[ i ] );
    }
    printf( "=%d\n", S );
  } else {
    printf( "no solution\n" );
  }
}
