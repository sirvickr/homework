#include <iomanip>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

void FindSumSeq( istream& stream );

int main()
{
#if 0
	stringstream iss;
	iss << 3 << " " << 15 << " " << 25 << " " << 30 << " " << 10;
//	iss << 2 << " " << 10 << " " << 10 << " " << 100;
//	iss << 3 << " " <<  7 << " " <<  7 << " " <<  7 << " " <<  7;
//	iss << 2 << " " << 10 << " " << 15 << " " << -5;
	FindSumSeq( iss );
#else
	cout << "input: ";
	FindSumSeq( cin );
#endif
	return 0;
}

void FindSumSeq( istream& stream ) {
	size_t n;
	stream >> n;
	int a[n];
	for( size_t i = 0; i < n; ++i )
		stream >> a[i];
	int S;
	stream >> S;
#if 0
	cout << "input =";
	for( size_t i = 0; i < n; ++i )
		cout << " " << a[i];
	cout << " S = " << S << endl;
#endif
	// переберем все возможные суммы,
	// т.е. все возможные сочетания знаков слагаемых
	size_t N = pow(2, n);
	size_t index;
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
		for( size_t i = 0; i < n; ++i ) {
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
		cout << ( ( index & 1 ) ? -a[ 0 ] : a[ 0 ] );
		for( size_t i = 1; i < n; ++i ) {
			cout << showpos << ( ( ( index >> i ) & 1 ) ? -a[ i ] : a[ i ] );
		}
		cout << "=" << noshowpos << S << endl;
	} else {
		cout << "no solution" << endl;
	}
}
