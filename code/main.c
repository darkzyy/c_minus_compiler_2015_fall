#include<stdio.h>
#include<string.h>
#include"syntax.tab.h"
#include"block.h"
#include"peephole.h"
#include"debug.h"

//#define __LEXER_DEBUG__
#define __PARSER_DEBUG__


extern FILE* yyin;
extern void error_report();
extern void yyrestart(FILE* f);
extern int lineno_init();
extern void print_code();
extern void dag_opti();
extern void asmgen();
extern void print_asm();

int enable_debug = 0;
int wall = 0;
int output_intercode = 0;

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
    int i;
    for(i = 2;i<argc;i++){
        if (strcmp(argv[i],"-d")==0){
            enable_debug = 1;
        }
        if (strcmp(argv[i],"-Wall")==0){
            wall = 1;
        }
        if (strcmp(argv[i],"-ic")==0){
            output_intercode = 1;
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
    peep_opti();
    find_leader();
    dag_opti();
    peep_opti();
    if(output_intercode){
        print_code();
    }
    else{
        if(enable_debug){
            print_code();
        }
        asmgen();
        print_asm();
    }
	return 0; 
}
#endif
