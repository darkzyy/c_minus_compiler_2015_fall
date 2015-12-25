#include<stdio.h>
#include<string.h>
#include"syntax.tab.h"
#include"block.h"
#include"peephole.h"

//#define __LEXER_DEBUG__
#define __PARSER_DEBUG__


extern FILE* yyin;
extern void error_report();
extern void yyrestart(FILE* f);
extern int lineno_init();
extern void print_code();
extern void dag_opti();

int enable_debug = 0;
int wall = 0;

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
	if (argc <= 2) return 1; 
    int i;
    for(i = 3;i<argc;i++){
        if (strcmp(argv[i],"-d")==0){
            enable_debug = 1;
        }
        if (strcmp(argv[i],"-Wall")==0){
            wall = 1;
        }
    }
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
    if(enable_debug){
        print_code();
        printf("-------------------------------\n");
    }
    peep_opti();
    if(enable_debug){
        print_code();
        printf("-------------------------------\n");
    }
    find_leader();
    dag_opti();
    peep_opti();
    print_code();
	return 0; 
}
#endif
