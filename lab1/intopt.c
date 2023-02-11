#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    //variable
    int m;
    int n;
    int i;
    int j;
    int r;
    double* c;
    double* b;
    double** a;
    FILE* in;

    in = fopen("i", "r");
	if (in == NULL)
		return 0;
    //scanf("%d %d\n", &m, &n);

    fscanf(in, "%d %d", &m, &n);

    c = calloc(n, sizeof(*c));
    a = calloc(n+m, sizeof(*a));
    b = calloc(m, sizeof(*b));

    for(i = 0; i < n; i += 1){
        r = fscanf(in, "%lf", &c[i]);
    }
    for(i = 0; i < m; i += 1){
        a[i] = calloc(n, sizeof(*a[i]));
        for(j = 0; j < n; j += 1){
            r = fscanf(in, "%lf", &a[i][j]);
        }
    }
    for(i = 0; i < m; i += 1){
        r = fscanf(in, "%lf", &b[i]);
    }

    //print
    printf("m = %d, n = %d\n", m, n);
    printf("max z = ");
    for(i = 0; i < n; i += 1){
        if(i != n-1){
            printf("%10.3lfx_%d ", c[i], i);
            printf(" + ");
        }
        else{
            printf("%10.3lfx_%d ", c[i], i);
        }
        
    }
    printf("\n");
    for(i = 0; i < m; i += 1){
        for(j = 0; j < n; j += 1){
            if(j != n-1){
                printf("%10.3lfx_%d", a[i][j], j);
                printf(" + ");
            }
            else{
                printf("%10.3lfx_%d", a[i][j], j);

            printf("  \u2264 %10.3lf\n", b[i]);
            }
        }
        printf("\n");
    }
    free(a);
    free(b);
    free(c);
    return 0;

}