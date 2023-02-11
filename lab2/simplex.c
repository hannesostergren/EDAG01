#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//branch and bound

typedef struct node_t node_t;

struct node_t
{
    int m;
    int n;
    int k;
    int h;
    double xh;
    double ak;
    double bk;
    double* min;
    double* max;
    double** a;
    double* b;
    double* x;
    double* c;
    double z;
    node_t* next;
};

node_t* initial_node(int m, int n, double** a, double* b, double* c){
    double* x;
    double* min;
    double* max;


    node_t* p = calloc(1, sizeof(*p));
    p->a = calloc(1, sizeof(*a));
    p->b = calloc(m+1, sizeof(*b));
    p->c = calloc(n+1, sizeof(*c));
    p->x = calloc(n+1, sizeof(*x));
    p->min = calloc(n, sizeof(*min));
    p->max = calloc(n, sizeof(*max));
    p->m = m;
    p->n = n;
    p->a = a; //vet inte om detta funkar
    p->b = b;
    p->c = c;

    for(int i = 0; i < n; i += 1){
        p->min[i] = -INFINITY;
        p->max[i] = INFINITY;
    }
    return p;
}

node_t* extend(node_t* p, int m, int n, double** a, double* b, double* c, int k, double ak, double bk){
    node_t* q = calloc(1, sizeof(*q));
    int i;
    int j;
    q->k = k;
    q->ak = ak;
    q->bk = bk;
    if (ak > 0 && p->max[k] < INFINITY){
        q->m = p->m;
    }
    else if (ak < 0 && p->min[k] > 0){
        q->m = p->m;
    }
    else{
        q->m = p->m + 1;
    }
    q->n = p->n;
    q->h = -1;
    q->a = calloc(q->m+1, sizeof(*q->a));
    for(i = 0; i < q->m+1; i += 1){
        a[i] = calloc(q->n+1, sizeof(*a[i]));
    }
    q->b = calloc(q->m+1, sizeof(*q->b));
    q->c = calloc(q->n+1, sizeof(*q->c));
    q->x = calloc(q->n+1, sizeof(*q->x));
    q->min = calloc(n, sizeof(*q->min));
    q->max = calloc(n, sizeof(*q->max));
    q->min = p->min;
    q->max = p->max;
    for(i = 0; i < m; i += 1){
        q->a[i] = a[i];
    }
    for(i = 0; i < m; i += 1){
        q->b[i] = b[i];
    }
    q->c = c;
    if (ak > 0){
        if (q->max[k] == INFINITY || bk < q->max[k]) {
            q->max[k] = bk;
        }
    }
    else if (q->min[k] == -INFINITY || -bk > q->min[k]){
        q->min[k] = -bk;
    }
    for (i = m, j = 0; j < n; j += 1){
        if (q->min[j] > -INFINITY){
            q->a[i][j] = -1;
            q->b[i] = -q->min[j];
            i += 1;
        }
        if (q->max[j] < INFINITY){
            q->a[i][j] = 1;
            q->b[i] = q->max[j];
            i += 1;
        }
        return q;
    }
}

int is_integer(double* xp){
    double	eps = 1e-6;
    double x = *xp;
    double r = round(x);
    if (abs(r - x) < eps){
        *xp = r;
        return 1;
    }
    else{
        return 0;
    }
}

int integer(node_t* p){
    int i;
    for (i = 0; i < p->n; i += 1){
        if (!is_integer(&p->x[i])){
            return 0;
        }
    }
    return 1;
}
int bound(node_t* p, int h, double* zp, double* x){
    node_t* q;
    if (p->z > *zp){
        *zp = p->z;
        x = p->x; //how do i save max 
        if(q->z < p->z){
            free(h); //hmmmm
        }
    }
}
int isfinite(double x){
    if (x == NAN || abs(x) == INFINITY){
        return 0;
    }
    else{
        return 1;
    }
}

int branch(node_t* q, double z){
    double min;
    double max;
    if (q->z < z){
        return 0;
    }
    for (int h = 0; h < q->n; h += 1){
        if (!is_integer(&q->x[h])){
            if (q->min[h] = -INFINITY){
                min = 0;
            }
            else{
                min = q->min[h];
            }
            max = q->max[h];
            if (floor(q->x[h]) < min || ceil(q->x[h]) > max){
                continue;
            }
            q->h = h;
            q->xh = q->x[h];
            for (int i = 0; i < q->n; i += 1){
                free(q->a[i]);
            }
            free(q->a);
            free(q->b);
            free(q->c);
            free(q->x);
            free(q);
            return 1;
        }
    return 0;
    }
}
int succ(node_t* p, node_t* h, int m, int n, double** a, double* b, double* c, int k, double ak, double bk, double* zp, double* x){
    node_t* q;
    node_t* h;
    h->next = p;
    q = extend(p, m, n, a, b, c, k, ak, bk);
    if (q == NULL){
        return 1;
    }
    q->z = simplex(q->m, q->n, q->a, q->b, q->c, q->x, 0);
    if (isfinite(q->z)){
        if(integer(q)){
            bound(q, h, zp, x);
        }
        else if (branch(q, *zp)){
            //add q to h
            while(p->next != NULL){
                p = p->next;
            }
            p->next = q;
            return 1;
        }
    }
    free(q);
}

int intopt(int m, int n, double** a, double* b, double* c, double* x){
    node_t* p;
    node_t* h;
    h = calloc(1, sizeof(*h));
    h->next = p;
    p = initial_node(m, n, a, b, c);
    double z = -INFINITY;
    p->z = simplex(p->m, p->n, p->a, p->b, p->c, p->x, 0);
    if (is_integer(p) || !isfinite(p->z)){
        z = p->z;
        if(is_integer(p)){
            x = p->x;
        }
        free(p);
        return z;
    }
    branch(p, z);
    while(h != NULL){
        p = h->next;
        h->next = p->next;
        succ(p, h, m, n, a, b ,c, p->h, 1, floor(p->xh), &z, x);
        succ(p, h, m, n, a, b, c, p->h, -1, -ceil(p->xh), &z, x);
        free(p);
    }
    if(z = -INFINITY){
        return NAN;
    }
    else{
        return z;
    }
}




//simplex
typedef struct simplex_t simplex_t;

int xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);
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

int initial(simplex_t** s2, int m, int n, int* var, double** a, double* b, double* c, double* x, double y){
    int i;
    int j;
    int k;
    double w;
    double* t;
    double	eps = 1e-6;
    simplex_t* s = *s2;
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
        s->c = 0;
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
        t = 0;
        s->x = 0;
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

int xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h) {
    simplex_t* s;
    int i, row, col;
    double	eps = 1e-6;

    s = calloc(1, sizeof(*s));


    if (!initial(&s, m, n, var, a, b, c, x, y)){
        free(s->var);
        return (int)NAN; 
    }
    while ((col = select_nonbasic(s)) >= 0){
        //col = select_nonbasic(s);
        row = -1;
        for (i = 0; i < m; i += 1){
            if (a[i][col] > eps && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col])){
                row = i;
            }
        }
        if (row < 0){
            s->var = 0;
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
        s->var = 0;
    }
    else{
        for (i = 0; i < n; i += 1){
            x[i] = 0;
        }
        for (i = n; i < n+m; i += 1){
            x[i] = s->b[i-n];
        }
    }
    return s->y;
}

int simplex(int m, int n, double** a, double* b, double* c, double* x, double y){
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

