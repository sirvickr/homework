#include "utils.h"
#include <ctype.h>

/*	61 a
	65 e
	69 i
	6F o
	75 u
	79 y */

// находит в строке количество согласных
int consonant_count( const char* text ) {
	if( NULL == text )
		return 0;
	int result = 0;
	size_t length = strlen( text );
	for( size_t i = 0; i < length; ++i ) {
		int ch = tolower( text[ i ] );
		//if( 0x61 == ch || 0x65 == ch || 0x69 == ch || 0x6F == ch || 0x75 == ch || 0x79 == ch ) {
		if( 0x62 == ch || 0x63 == ch || 0x64 == ch || 0x66 == ch || 0x67 == ch || 0x68 == ch || 0x6A == ch 
		 || 0x6B == ch || 0x6C == ch || 0x6D == ch || 0x6E == ch || 0x70 == ch || 0x71 == ch || 0x72 == ch 
		 || 0x73 == ch || 0x74 == ch || 0x76 == ch || 0x77 == ch || 0x78 == ch || 0x7A == ch ) {
			++result;
		}
	}
	return result;
}
