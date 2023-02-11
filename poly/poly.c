#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "error.h"
#include "poly.h"

#include "string.h"

typedef struct poly_t poly_t;

struct poly_t{
    int* coeff;
    int* exp;
    int len;
};
    

poly_t*	new_poly_from_string(const char* in){
    
    poly_t* p = calloc(1, sizeof(*p));
    p->coeff = calloc(100, sizeof(*p->coeff));
    p->exp = calloc(100, sizeof(*p->exp));
    p->len = 0;
    int i, n, c, neg, one, co, ex;
    int len = strlen(in);

    i = 0;
    c = 0;
    neg = 0;
    one = 1;
    co = 1;
    ex = 0;


    while(i < len) {
		c = in[i];

		if (isdigit(c)) {
			n = 0;
			while(i < len && isdigit(c)) {
				n = (n*10) + (c - '0');
				i++;
				c = in[i];
			}

			if (co){
				if (neg)
					n *= -1;
				p->coeff[p->len] = n;
				one = 0;
			}
			else if (ex) {
				p->exp[p->len] = n;
			}
		}
        switch (c){
            case '+':
                p->len++; //next fukin term
                neg = 0; 
                co = 1;
                ex = 0;
                one = 1;
                break;
            case '-':
                p->len++; // next fukin term
                neg = 1;
                co = 1;
                ex = 0;
                one = 1;
                break;
            case '^':
                co = 0;
                ex = 1;
                if (one){
                    p->coeff[p->len] = 1;
                }
                break;
            case 'x':
                p->exp[p->len] = 1;
                break;
            }
        i++;
    }
    p->exp[p->len] = 0; //for constant
    p->len++;
    return p;

}

void free_poly(poly_t* p){
    free(p->exp);
    free(p->coeff);
    free(p);
}

poly_t* mul(poly_t* p1, poly_t* p2){
    poly_t* q = calloc(1, sizeof(*q));
    q->coeff = calloc(100, sizeof(*q->coeff));
    q->exp = calloc(100, sizeof(*q->exp));
    int i, j, n, m, match;
    int* co = calloc(p1->len*p2->len, sizeof(int));
    int* exp = calloc(p1->len*p2->len, sizeof(int));

    n = 0;
    m = 0;
    match = 0;

    //if (p1->len > p2->len){len = p1->len;} else {len = p2->len;} //oneliner is faster

    for(i = 0; i < p1->len; i++){ //coefficients and exponents for each term i read into co[n] and exp[n] (works)
        for(j = 0; j < p2->len; j++){
            co[n]  = p1->coeff[i] *  p2->coeff[j];
            exp[n] = p1->exp[i]   +  p2->exp[j];
            // printf("%d, ", co[n]);
            // printf("%d, ", exp[n]);
            // printf("\n");
            n++;
        }
    }
    for(i = 0; i < n; i++){ //outer loop
        for(j = i+1; j < n; j++){ // inner loop
            if(exp[i] == exp[j]){ //checks if there are matching exponents
                match = 1;
            }
            else{
                match = 0;
            }
            goto match;
        }
        match:
        if(match){ // if matching exponent
            q->coeff[m] = co[i]+co[j];
            co[j] = 0;
            q->exp[m] = exp[i];
            m++;
            q->len++;
        }
        else{
            q->coeff[m] = co[i];
            q->exp[m] = exp[i];
            m++;
            q->len++;
        }
    }
    free(co);
    free(exp);
    return q;
}

    
void print_poly(poly_t* p){
    int i;
    for(i = 0; i < p->len; i++){
        if(p->coeff[i] == 0){}
        else if(i == 0){
            if(p->coeff[i] == 1){
                printf("x^%d", p->exp[i]);
            }
            else if(p->exp[i] == 1){
                printf("%1dx", p->coeff[i]);
            }
            else{
                printf("%1dx^%d", p->coeff[i], p->exp[i]);
            }
        }
        else{
            if(p->exp[i] == 1){
                if(p->coeff[i] < 0){
                    printf(" - ");
                    printf("%dx", abs(p->coeff[i]));
                }
                else{
                    printf(" + ");
                    printf("%dx", p->coeff[i]);
                }
            }
            else if(p->exp[i] == 0){
                if(p->coeff[i] < 0){
                    printf(" - ");
                    printf("%d", p->coeff[i]);
                }
                else{
                    printf(" + ");
                    printf("%d", p->coeff[i]);
                }
            }
            else if(p->coeff[i] == 1){
                if(p->coeff[i] < 0){
                    printf(" - ");
                    printf("x^%d ", abs(p->exp[i]));
                }
                else{
                    printf(" + ");
                    printf("x^%d", p->exp[i]);
                }
            }
            else{
                if(p->coeff[i] < 0){
                    printf(" - ");
                    printf("%dx^%d", abs(p->coeff[i]), p->exp[i]);
                }
                else{
                    printf(" + ");
                    printf("%dx^%d", p->coeff[i], p->exp[i]);
                }
                
            }
        }
    }
    printf("\n");
}

// int main(int argc, char* argv[]){

//     poly_t*		p;
// 	poly_t*		q;
// 	poly_t*		r;

//     p = new_poly_from_string("x^2 - 7x + 1");
// 	q = new_poly_from_string("3x + 2");

//     r = mul(p, q);

//     print_poly(p);
//     print_poly(q);
//     print_poly(r);

// }