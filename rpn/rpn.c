#define N		(10)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int len = 0;
int n;
int buf[N];
int line = 1;
int op = 0;


int handleChar(int c){
	n = 0;
	if(isdigit(c)){
		while(isdigit(c)){
			n = (n * 10) + (c - '0');
			c = getchar();
		}
		if(len < N){ //check no space left on stack
			buf[len] = n;
			len++;
		}
		else{ //check invalid character
			printf("line %d: error at %d\n", line, n);
			len = 0;
			line++;
		}
	}
	if(c == '\n' && len == 1 && op == 1){
		printf("line %d: %d\n", line, buf[len-1]);
		line++;
		len -= 1;
	}
	
	else if (len > 1){ //check zero or one number on the stack
		switch(c){
			case '+':
				op = 1;
				buf[len-2] =  buf[len-1] + buf[len-2];
				len -= 1;
				break;
			case '-':
				op = 1;
				buf[len-2] =  buf[len-2] - buf[len-1];
				len -= 1;
				break;
			case '*':
				op = 1;
				buf[len-2] =  buf[len-1] * buf[len-2];
				len -= 1;
				break;
			case '/':
				op = 1;
				if(buf[len-1] != 0){ //check no divide by zero
					buf[len-2] =  buf[len-2] / buf[len-1];
					len -= 1;
				}
				break;
			default:
				if(c == '\n'){
					if(len == 2){ //check exactly 1 element on stack
						printf("line %d: error at \\n\n", line);
						line++;
					}
					else{ //check invalid character
						printf("line %d: error at %c\n", line, buf[len]+2);
						len = 0;
						line++;
					}
				}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	
	int c = getchar();
	while(c != EOF){
		handleChar(c);
		c = getchar();
	}
	
	
	return 0;
}
