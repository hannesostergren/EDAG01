#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int n = 0, l = 0;
	char* w = malloc(0);
	char* v = malloc(0);

	char c = getchar();
	while(c != EOF){
		if(isalpha(c)){
			l++;
			v = realloc(v, l*sizeof(char));
			v[l-1] = c;
		}
		else{
			if(l > n){
				n = l;
				w = realloc(w, l*sizeof(char));
				strcpy(w, v);
			}
			l = 0;
			free(v);
			v = malloc(0*sizeof(char));
		}
		c = getchar();
	}
	printf("%d characters in longest word: %s\n", n, w);
	free(w);
	free(v);
	return 0;
}
