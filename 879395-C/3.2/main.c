/*
3.2. Для заданного каталога и его подкаталогов напечатать таблицу имён файлов и их длины.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void printDir( const char* path, int indent ) {
  // последовательность пробелов для отступа при печати
  char indentation[ 64 ];
  // полный путь к объекту файловой системы
  char full_path[ 512 ];
  // переменная для получения информации об объекте файловой системы
  struct stat stat_buf;
  // указатель но позицию в текущем каталоге
  struct dirent *d;

  memset( indentation, ' ', indent );
  indentation[ indent ] = '\0';
  // копируем каталог в начало полного пути
  strcpy( full_path, path );
  size_t path_len = strlen( full_path );
  // при необходимости дополняем слешем
  if( full_path[ path_len - 1 ] != '/' ) {
    full_path[ path_len++ ] = '/';
    full_path[ path_len ] = '\0';
  }
  printf( "%sDIR: \"%s\"\n", indentation, path );
  // открываем ресурс текщего каталога
  DIR *dir = opendir( path );
  if( dir == NULL ) {
    perror( "opendir" );
    return;
  }
  // читаем одну за другой все позиции в списке текущего каталога 
  while( ( d = readdir( dir ) ) != NULL ) {
    if( !strcmp( d->d_name, "." ) || !strcmp( d->d_name, ".." ) )
      continue;
    // формируем полный путь к очередному объекту файловой системы
    strcpy( full_path + path_len, d->d_name );
    printf( "%s%8s\t", indentation, d->d_name );
    // получаем информацию об объекте
    if( -1 == lstat( full_path, &stat_buf ) ) {
      fflush( stdout );
      perror( "stat" );
      break;
    }
    int ifmt = stat_buf.st_mode & S_IFMT;
    switch(ifmt) {
    case S_IFLNK: // ссылка
      printf( "link\n" );
      break;
    case S_IFREG: // файл, печатаем размер
      printf("%lu\n", stat_buf.st_size);
      break;
    case S_IFDIR: // каталог, рекурсивно уходим вглубь иерархии
      printf( "dir\n" );
      printDir( full_path, indent + 1 );
      break;
    default: // что-то другое (устройство и т.д.)
      printf( "?\n" );
      break;
    }
  }
  // закрываем ресурс текщего каталога
  closedir( dir );
}

int main( int argc, char* argv[] ) {

  // распечатываем переданный в аргументе либо текущий каталог
  printDir( ( argc < 2 ) ? "./" : argv[ 1 ], 0 );

  return 0;
}
