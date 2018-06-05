#include <stdio.h>
#define MY_PI 3.1415
int main()
{
	#define SOME_VAL 1024
	printf("%s %s\n", "MY_PI", "SOME_VAL");
	#undef MY_PI
	printf("%s %s\n", "MY_PI", "SOME_VAL");
	return 0;
}
