/*
2.4. Для квадратной матрицы вычислить обратную.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// Возвращает минор матрицы matrix[]
void Minor( const int dim, const int row, const int col, double result[], const double matrix[] ) {
    int k = 0;
    for( int i = 0; i < dim; ++i ) {
      for( int j = 0; j < dim; ++j ) {
        if( row != i && col != j ) {
          result[ k++ ] = matrix[ ( i * dim ) + j ];
        }
      }
    }
}

// Вычисляет определитель матрицы matrix[]
double Discriminant( const double matrix[], const int dim ) {
  if( dim < 1 )
    return 0;
  if( dim == 1 )
    return matrix[ 0 ];
  double D = 0;
  double m[ ( dim - 1 ) * ( dim - 1 ) ];
  for( int row = 0; row < dim; ++row ) {
    int sign = 1 - ( ( row & 1 ) << 1 );
    Minor( dim, row, 0, m, matrix );
    D += ( sign * matrix[ row * dim ] * Discriminant( m, dim - 1 ) );
  }
  return D;
}

// Вычисляет матрицу, обратную матрице matrix[] и возвращает 1. Если её не существует, возвращает 0.
int ReciprocalMatrix( const int dim, const double matrix[], double result[] ) {
    // Буфер для хранения промежуточных миноров
    double m[( dim - 1 ) * ( dim - 1 )];
    // Определитель исходной матрицы
    double D = Discriminant( matrix, dim );
    if( fabs(D) < DBL_EPSILON )
      return 0;
    // В цикле вычисляем алгебраические дополнения и результирующие элементы
    for( int i = 0; i < dim; ++i ) {
      for( int j = 0; j < dim; ++j ) {
        // вычисляем минор очередного элемента
        Minor( dim, i, j, m, matrix );
        // минор превращается в алгебраическое дополнение установкой знака в зависимости от позиции
        int sign = ( 1 - ( ( i & 1 ) << 1 ) ) * ( 1 - ( ( j & 1 ) << 1 ) );
        // ставим знак и сразу делим каждый элемент на определитель исходной матрицы
        result[ j * dim + i ] = Discriminant( m, dim - 1 ) * sign / D;
        // транспонирование достигается за счёт обратной индексации (j * dim + i)
      }
    }
    return 1;
}

int main( int argc, char* argv[] ) {
  if( argc < 2 ) {
    printf(" usage: %s matrix\n", argv[ 0 ] );
    printf(" e.g:   %s 2 5 7 6 3 4 5 -2 -3\n", argv[ 0 ] );
    return 0;
  }
  
  const int count = argc - 1;
  const int dim = ( int )sqrt( count );
  
  if( dim * dim != count ) {
    fprintf( stderr, "matrix not square\n" );
    exit( EXIT_FAILURE );
  }
  
  printf("%d %dx%d\n", count, dim, dim );
  
  // Исходная матрица
  double matrix[count];
  // Буфер для хранения промежуточных результата
  double result[count];

  for( int i = 0; i < count; ++i )
    matrix[ i ] = strtod( argv[ i + 1 ], NULL );

  // Печатаем исходную матрицу
  printf( "matrix\n" );
  for( int i = 0; i < dim; ++i ) {
    for( int j = 0; j < dim; ++j ) {
      printf(" %.0f", matrix[ j * dim + i ]);
    }
    printf("\n");
  }
  
  // Вычисляем обратную матрицу
  if( ReciprocalMatrix( dim, matrix, result ) ) {
    // Выводим результат
    printf( "result:\n" );
    for( int i = 0; i < dim; ++i ) {
      for( int j = 0; j < dim; ++j ) {
        printf(" %.1f", result[ i * dim + j ]);
      }
      printf("\n");
    }
  } else {
    printf( "no reciprocal matrix\n" );
  }

  return 0;
}
