#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 256

int sigma[TABLE_SIZE] = { 0 };

/*int max (int a, int b) {
	return ( (a>b) ? a : b);
}
 
void Delta(char *s, int **D) {
        int i,j,k,n=strlen(s);
        for (i=0,k=0; i<n;i++,k++) {
                for (j=0;j<26;j++) D[i][j]=n;
                for (j=0;j<n-k;j++) {
                        D[i][s[j]-'a']=n-j-1;
                }
        }
}
 
void BMSubst (char *S, char *T) {
	int len=strlen(S),lenT=strlen(T), k=len-1,i,j;
	//int **D=(int**)malloc(len*sizeof(int*));
	//for (i=0;i<len;i++) D[i]=(int*)malloc(TABLE_SIZE*sizeof(int));
	int D[len][256];
	//Delta(S,D);
	for (i=0;i<len;i++) {
		for (j=0;j<26;j++) 
			D[i][j]=len;
		for (j=0;j<i;j++) {
			D[i][S[j]-'a']=len-j-1;
		}
	}
	while (k<lenT) {
		i=len-1;
		while (T[k]==S[i]) {
			if (i == 0) {
				printf("%d ", k);
				return;
			}
			i--;
			k--;
		}
		k += max(D[i][T[k]-'a'], len - i);
	}
	printf("%d",lenT);
	//for (i=0;i<len;i++) free(D[i]);
	//free(D);
}*/

int BM(const char* t, const char* s) {
	int i, j, k;
	int t_len = strlen(t);
	int s_len = strlen(s);
	int result = t_len;
	//printf("T = %s S = %s\n", t, s);
	if(s_len < t_len) {
		for(i = 0; i < 256; i++)
			sigma[i] = s_len;
		for(i = 1; i < s_len; i++)
			sigma[s[i]] = s_len - i;
		i = s_len;
		j = i;
		while(j > 0 && i <= t_len) {
			j = s_len;
			k = i;
			while(j > 0 && t[k] == s[j]) {
				k--;
				j--;
			}
			i += sigma[t[i]];
		}
		if (k > t_len - s_len) {
			result = t_len;
		} else {
			result = k + 1;
		}
	} // if
	return result;
}

int main(int argc, char* argv[]) {
	size_t len, i;
	if (argc < 3) {
		fprintf(stderr, "expecting two strings\n");
		return 1;
	}
	const char* T = argv[1];
	const char* S = argv[2];
	
	printf("%d\n", BM(T, S));

//	BMSubst(argv[1], argv[2]);

	return 0;
}
