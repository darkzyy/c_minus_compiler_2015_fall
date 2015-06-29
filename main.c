#include<stdio.h>
#include"syntax.tab.h"

//#define __LEXER_DEBUG__
#define __PARSER_DEBUG__


extern FILE* yyin;
extern void error_report();
extern void yyrestart(FILE* f);
extern int lineno_init();

int enable_debug = 1;

#ifdef __LEXER_DEBUG__
int main(int argc,char** argv){
	if(argc>0){
		if(!(yyin = fopen(argv[1],"r"))){
			perror(argv[1]);
			return 1;
		}
	}
	while(yylex() != 0);
	return 0;
}
#endif

#ifdef __PARSER_DEBUG__
int main(int argc,char** argv){
	if (argc <= 1) return 1; 
	FILE* f = fopen(argv[1], "r"); 
	if (!f) { 
		perror(argv[1]); 
		return 1; 
	} 
	yyrestart(f);	
#if YYDEBUG
	yydebug = 1;
#endif
	yyparse(); 
	return 0; 
}
#endif

/*
yyerror(char* msg) {
	fprintf(stderr, "error: %s\n", msg);
}
*/
