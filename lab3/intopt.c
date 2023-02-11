#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct simplex_t simplex_t;

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);
int pivot(simplex_t* s, int row, int col);

struct simplex_t{
    int m;
    int n;
    int* var;
    double** a;
    double* b;
    double* x;
    double* c;
    double y;
};

int init(simplex_t* s, int m, int n, int* var, double** a, double* b, double* c, double* x, double y){
    int i;
    int k;

    s->m = m;
    s->n = n;
    s->var = var;
    s->a = a;
    s->b = b;
    s->c = c;
    s->x = x;
    s->y = y;

    if (s->var == NULL){
        s->var = calloc(m+n+1, sizeof(int));
        for(i = 0; i < m+n; i += 1){
            s->var[i] = i;
        }
    }
    k = 0;
    for(i = 1; i < m; i += 1){
        if (b[i] < b[k]){
            k = i;
        }
        return k;
    }
}

int select_nonbasic(simplex_t* s){
    double eps = 1e-6;
    int i;

    for(i = 0; i < s->n; i += 1){
        if (s->c[i] > eps){
            return i;
        }
    }
    return -1;
}

int prepare(simplex_t* s, int k){
    int m = s->m;
    int n = s->n;
    int i;

    for (i = m+n; i > n; i -= 1){
        s->var[i] = s->var[i-1];
    }
    s->var[n] = m + n;
    n += 1;
    for (i = 0; i < m; i += 1){
        s->a[i][n-1] -= 1;
    }
    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n-1] = -1;
    s->n = n;
    pivot(s, k, n-1);
    return 1;
}

int initial(simplex_t* s, int m, int n, int* var, double** a, double* b, double* c, double* x, double y){
    int i;
    int j;
    int k;
    double w;
    double* t;
    double	eps = 1e-6;
    //simplex_t* s = *s2;
    k = init(s, m, n, var, a, b, c, x, y);
    
    if (b[k] >= 0){
        return 1;
    }
    prepare(s, k);
    n = s->n;
    s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);
    for(i = 0; i < m+n; i += 1){
        if(s->var[i] == m+n+1){
            if(s->x[i] > eps){
                free(s->x);
                free(s->c);
                return 0;
            }
            else{
                break;
            }
        }
        if (i >= n){
            for(j = k = 0; k < n; k += 1){
                if(s->a[i-n][k] > s->a[i-n][j]){
                    j = k;
                }
            }
            pivot(s,i-n,j);
            i = j;
        }
        if(i < n-1){
            k = s->var[i];
            s->var[i] = s->var[n-1];
            s->var[n-1] = k;
            for(k = 0; k < m; k += 1){
                w = s->a[k][n-1]; s->a[k][n-1] = s->a[k][i]; s->a[k][i] = w;
            }
        }
        free(s->c);
        s->c = c;
        s->y = y;
        for (k = n-1; k < n+m-1; k += 1){
            s->var[k] = s->var[k+1];
        }
        n = s->n = s->n - 1;
        t = calloc(n, sizeof(double));
        for (k = 0; k < n; k += 1){
            for (j = 0; j < n; j += 1){
                if (k == s->var[j]){
                    t[j] = t[j] + s->c[k];
                    goto next_k;
                }
            }
            for (j = 0; j < m; j += 1){
                if (s->var[n+j] == k){
                    break;
                }
            }
            s->y = s->y + s->c[k] * s->b[j];
            for (i = 0; i < n; i += 1){
                t[i] = t[i] - s->c[k] * s->a[j][i];
            }
        }
        next_k:;
        for (i = 0; i < n; i += 1){
            s->c[i] = t[i];
        }
        free(t);
        free(s->x);
        return 1;
    }
}

int pivot(simplex_t* s, int row, int col){
    double** a = s->a;
    double* b = s->b;
    double* c = s->c;
    int m = s->m;
    int n = s->n;
    int i,j,t;

    t = s->var[col];
    s->var[col] = s->var[n+row];
    s->var[n+row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];
    for (i = 0; i < n; i += 1){
        if (i != col){
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }
    }
    c[col] = -c[col] / a[row][col];
    for (i = 0; i < m; i += 1){
        if (i != row){
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }
    for (i = 0; i < m; i = i + 1){
        if (i != row){
            for (j = 0; j < n; j = j + 1){
                if (j != col){
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }
    for (i = 0; i < m; i = i + 1){
        if (i != row){
            a[i][col] = -a[i][col] / a[row][col];
        }
    }
    for (i = 0; i < n; i = i + 1){
        if (i != col){
            a[row][i] = a[row][i] / a[row][col];
        }
    }
    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];   
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h) {
    simplex_t* s;
    int i, row, col;
    double	eps = 1e-6;

    s = calloc(1, sizeof(*s));

    if (!initial(s, m, n, var, a, b, c, x, y)){
        free(s->var);
        return NAN; 
    }
    while ((col = select_nonbasic(s)) >= 0){
        row = -1;
        for (i = 0; i < m; i += 1){
            if (a[i][col] > eps && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col])){
                row = i;
            }
        }
        if (row < 0){
            free(s->var);
            return -1;
        }
        pivot(s, row, col);
    }
    if (h == 0){
        for (i = 0; i < n; i += 1){
            if (s->var[i] < n){
                x[s->var[i]] = 0;
            }
        }
        for (i = 0; i < m; i += 1){
            if (s->var[n+i] < n){
                x[s->var[n+i]] = s->b[i];
            }
        }
        free(s->var);
    }
    else{
        for (i = 0; i < n; i += 1){
            x[i] = 0;
        }
        for (i = n; i < n+m; i += 1){
            x[i] = s->b[i-n];
        }
    }
    y = s->y;
    free(s);
    return y;
}

int simplex(int m, int n, double** a, double* b, double* c, double* x, double y){
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

int intopt(int m, int n, double** a, double* b, double* c, double* x) {
    return simplex(m,n,a,b,c,x,0);
}

int main(int argc, char** argv){
    int m;
    int n;
    int i;
    int j;
    int r;
    double* x = 0;
    double y = 0;
    double* c;
    double* b;
    double** a;
    scanf("%d %d\n", &m, &n);

    c = calloc(n, sizeof(*c));
    a = calloc(n+m, sizeof(*a));
    b = calloc(m, sizeof(*b));
    x = calloc(n+1, sizeof(*x));

    for(i = 0; i < n; i += 1){
        r = scanf("%lf", &c[i]);
    }
    for(i = 0; i < m; i += 1){
        a[i] = calloc(n, sizeof(*a[i]));
        for(j = 0; j < n; j += 1){
            r = scanf("%lf", &a[i][j]);
        }
    }
    for(i = 0; i < m; i += 1){
        r = scanf("%lf", &b[i]);
    }
    printf("%d\n", simplex(m, n, a, b, c, x, y));
    for(i = 0; i < n+m; i += 1){
        free(a[i]);
    }
    free(a);
    free(b);
    free(c);
    free(x);
    return 0;
}
