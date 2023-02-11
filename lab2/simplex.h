typedef struct simplex_t simplex_t;

int init(simplex_t* s, int m, int n, int* var, double** a, double* b, double* c, double* x, double y);

int select_nonbasic(simplex_t* s);

int initial(simplex_t* s, int m, int n, int* var, double** a, double* b, double* c, double* x, double y);

int pivot(simplex_t* s, int row, int col);

int xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);

int simplex(int m, int n, double** a, double* b, double* c, double* x, double y);