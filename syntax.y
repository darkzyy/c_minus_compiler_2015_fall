%{
#include"lex.yy.c"
int error_detected = 0;
enum {lexical_error = 1，
	syntax_error_known = 2,
	syntax_error_unknown = 3};
struct error_list{
	int error_type;
	int lineno;
	struct {
		char* error_token;
		char* error_type;
	} *error_node;//for lexical error	
	char* error_msg; //for known syntax error
	error_list* next;
}
error_list* head;
error_list* tail;
head = malloc(sizeof(error_list));
tail = head;
head->lineno = -1;
 %}

%locations
/*%define parse.error verbose
*/

%union {
struct MTnode* mtnode;
struct error_msg{
	char* error_token;
	char* error_type;
} *error_node;
}

/*declare tokens
 */
%token <mtnode>INT
%token <mtnode>FLOAT
%token <mtnode>ID
%token <mtnode>SEMI
%token <mtnode>COMMA
%token <mtnode>ASSIGNOP
%token <mtnode>RELOP
%token <mtnode>PLUS
%token <mtnode>MINUS
%token <mtnode>STAR
%token <mtnode>DIV
%token <mtnode>AND
%token <mtnode>OR
%token <mtnode>DOT
%token <mtnode>NOT
%token <mtnode>TYPE
%token <mtnode>LP
%token <mtnode>RP
%token <mtnode>LB
%token <mtnode>RB
%token <mtnode>LC
%token <mtnode>RC
%token <mtnode>STRUCT
%token <mtnode>RETURN
%token <mtnode>IF
%token <mtnode>ELSE
%token <mtnode>WHILE
%token <mtnode>ERROR

%right	ASSIGNOP
%left	OR
%left	AND
%left	RELOP
%left	PLUS MINUS
%left	STAR DIV
%right	NOT
%left	DOT LP RP LB RB

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* declared non-terminals */
%type <mtnode> Exp Args Dec DecList Def DefList 
%type <mtnode> Stmt StmtList CompSt VarDec FunDec VarList ParamDec
%type <mtnode> Tag OptTag StructSpecifier Specifier
%type <mtnode> ExtDecList ExtDef ExtDefList Program
/*%type <mtnode> SyntaxError*/

%%


Program : ExtDefList {
        MTnode** list=malloc(sizeof(void*)*1);
        list[0]=$1;
        $$ = create_node(list,1,"Program",&@1,Program);
		if(error_detected == 0){
			pre_tranverse($$);
		}
        }
ExtDefList : ExtDef ExtDefList {
           MTnode** list=malloc(sizeof(void*)*2);
           list[0]=$1;
           list[1]=$2;
           $$ = create_node(list,2,"ExtDefList",&@1,ExtDefList);
           }
		   | %empty				{$$ = create_node(NULL,0,"",&@$,EMPTY);}

ExtDef : Specifier ExtDecList SEMI {
       MTnode** list=malloc(sizeof(void*)*3);
       list[0]=$1;
       list[1]=$2;
       list[2]=$3;
       $$ = create_node(list,3,"ExtDef",&@1,ExtDef);
       }
	   | Specifier SEMI {
       MTnode** list=malloc(sizeof(void*)*2);
       list[0]=$1;
       list[1]=$2;
       $$ = create_node(list,2,"ExtDef",&@1,ExtDef);
       }
	   | Specifier FunDec CompSt {
       MTnode** list=malloc(sizeof(void*)*3);
       list[0]=$1;
       list[1]=$2;
       list[2]=$3;
       $$ = create_node(list,3,"ExtDef",&@1,ExtDef);
       }
	   /*
	   |error SEMI {
	   yyerror(2,"Broken External Definition");
	   }
	   */
ExtDecList : VarDec {
           MTnode** list=malloc(sizeof(void*)*1);
           list[0]=$1;
           $$ = create_node(list,1,"ExtDecList",&@1,ExtDecList);
           }
		   | VarDec COMMA ExtDecList {
           MTnode** list=malloc(sizeof(void*)*3);
           list[0]=$1;
           list[1]=$2;
           list[2]=$3;
           $$ = create_node(list,3,"ExtDecList",&@1,ExtDecList);
           }
Specifier : TYPE {
          MTnode** list=malloc(sizeof(void*)*1);
          list[0]=$1;
          $$ = create_node(list,1,"Specifier",&@1,Specifier);
          }
		  | StructSpecifier {
          MTnode** list=malloc(sizeof(void*)*1);
          list[0]=$1;
          $$ = create_node(list,1,"Specifier",&@1,Specifier);
          }
StructSpecifier : STRUCT OptTag LC DefList RC {
                MTnode** list=malloc(sizeof(void*)*5);
                list[0]=$1;
                list[1]=$2;
                list[2]=$3;
                list[3]=$4;
                list[4]=$5;
                $$ = create_node(list,5,"StructSpecifier",&@1,StructSpecifier);
                }
				| STRUCT Tag {
                MTnode** list=malloc(sizeof(void*)*2);
                list[0]=$1;
                list[1]=$2;
                $$ = create_node(list,2,"StructSpecifier",&@1,StructSpecifier);
                }
OptTag : ID {
       MTnode** list=malloc(sizeof(void*)*1);
       list[0]=$1;
       $$ = create_node(list,1,"OptTag",&@1,OptTag);
       }
	   | error '\n' {
	   yyerror(2,"broken struct specify");
	   }
	   | %empty				{$$ = create_node(NULL,0,"",&@$,EMPTY);}
	

Tag : ID {
    MTnode** list=malloc(sizeof(void*)*1);
    list[0]=$1;
    $$ = create_node(list,1,"Tag",&@1,Tag);
    }
VarDec : ID {
       MTnode** list=malloc(sizeof(void*)*1);
       list[0]=$1;
       $$ = create_node(list,1,"VarDec",&@1,VarDec);
       }
	   | VarDec LB INT RB {
       MTnode** list=malloc(sizeof(void*)*4);
       list[0]=$1;
       list[1]=$2;
       list[2]=$3;
       list[3]=$4;
       $$ = create_node(list,4,"VarDec",&@1,VarDec);
       }
FunDec : ID LP VarList RP {
       MTnode** list=malloc(sizeof(void*)*4);
       list[0]=$1;
       list[1]=$2;
       list[2]=$3;
       list[3]=$4;
       $$ = create_node(list,4,"FunDec",&@1,FunDec);
       }
	   | ID LP RP {
       MTnode** list=malloc(sizeof(void*)*3);
       list[0]=$1;
       list[1]=$2;
       list[2]=$3;
       $$ = create_node(list,3,"FunDec",&@1,FunDec);
       }
VarList : ParamDec COMMA VarList {
        MTnode** list=malloc(sizeof(void*)*3);
        list[0]=$1;
        list[1]=$2;
        list[2]=$3;
        $$ = create_node(list,3,"VarList",&@1,VarList);
        }
		| ParamDec {
        MTnode** list=malloc(sizeof(void*)*1);
        list[0]=$1;
        $$ = create_node(list,1,"VarList",&@1,VarList);
        }
ParamDec : Specifier VarDec {
         MTnode** list=malloc(sizeof(void*)*2);
         list[0]=$1;
         list[1]=$2;
         $$ = create_node(list,2,"ParamDec",&@1,ParamDec);
         }
CompSt : LC DefList StmtList RC {
       MTnode** list=malloc(sizeof(void*)*4);
       list[0]=$1;
       list[1]=$2;
       list[2]=$3;
       list[3]=$4;
       $$ = create_node(list,4,"CompSt",&@1,CompSt);
       }
	   |LC DefList error RC {
	   yyerror(2,"Statement Error Between { and }");
	   }
StmtList : Stmt StmtList {
         MTnode** list=malloc(sizeof(void*)*2);
         list[0]=$1;
         list[1]=$2;
         $$ = create_node(list,2,"StmtList",&@1,StmtList);
         }
		 | %empty 				{$$ = create_node(NULL,0,"",&@$,EMPTY);}

Stmt : Exp SEMI {
     MTnode** list=malloc(sizeof(void*)*2);
     list[0]=$1;
     list[1]=$2;
     $$ = create_node(list,2,"Stmt",&@1,Stmt);
     }
	 | CompSt {
     MTnode** list=malloc(sizeof(void*)*1);
     list[0]=$1;
     $$ = create_node(list,1,"Stmt",&@1,Stmt);
     }
	 | RETURN Exp SEMI {
     MTnode** list=malloc(sizeof(void*)*3);
     list[0]=$1;
     list[1]=$2;
     list[2]=$3;
     $$ = create_node(list,3,"Stmt",&@1,Stmt);
     }
	 | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
     MTnode** list=malloc(sizeof(void*)*5);
     list[0]=$1;
     list[1]=$2;
     list[2]=$3;
     list[3]=$4;
     list[4]=$5;
     $$ = create_node(list,5,"Stmt",&@1,Stmt);
     }
	 | IF LP Exp RP Stmt ELSE Stmt {
     MTnode** list=malloc(sizeof(void*)*7);
     list[0]=$1;
     list[1]=$2;
     list[2]=$3;
     list[3]=$4;
     list[4]=$5;
     list[5]=$6;
     list[6]=$7;
     $$ = create_node(list,7,"Stmt",&@1,Stmt);
     }
	 | WHILE LP Exp RP Stmt {
     MTnode** list=malloc(sizeof(void*)*5);
     list[0]=$1;
     list[1]=$2;
     list[2]=$3;
     list[3]=$4;
     list[4]=$5;
     $$ = create_node(list,5,"Stmt",&@1,Stmt);
     }
   	 |error{yyerrok;} ERROR {yyerror(1);} error SEMI {yyerrok;}
DefList : Def DefList {
        MTnode** list=malloc(sizeof(void*)*2);
        list[0]=$1;
        list[1]=$2;
        $$ = create_node(list,2,"DefList",&@1,DefList);
        }
		| %empty				{$$ = create_node(NULL,0,"",&@$,EMPTY);}

Def : Specifier DecList SEMI {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Def",&@1,Def);
    }
	|Specifier error SEMI{
	yyerror(2,"broken Declaration");
	}
DecList : Dec {
        MTnode** list=malloc(sizeof(void*)*1);
        list[0]=$1;
        $$ = create_node(list,1,"DecList",&@1,DecList);
        }
		| Dec COMMA DecList {
        MTnode** list=malloc(sizeof(void*)*3);
        list[0]=$1;
        list[1]=$2;
        list[2]=$3;
        $$ = create_node(list,3,"DecList",&@1,DecList);
        }
Dec : VarDec {
    MTnode** list=malloc(sizeof(void*)*1);
    list[0]=$1;
    $$ = create_node(list,1,"Dec",&@1,Dec);
    }
	| VarDec ASSIGNOP Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Dec",&@1,Dec);
    }
Exp : Exp ASSIGNOP Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp AND Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp OR Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp RELOP Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp PLUS Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp MINUS Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp STAR Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp DIV Exp {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| LP Exp RP {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| MINUS Exp {
    MTnode** list=malloc(sizeof(void*)*2);
    list[0]=$1;
    list[1]=$2;
    $$ = create_node(list,2,"Exp",&@1,Exp);
    }
	| NOT Exp {
    MTnode** list=malloc(sizeof(void*)*2);
    list[0]=$1;
    list[1]=$2;
    $$ = create_node(list,2,"Exp",&@1,Exp);
    }
	| ID LP Args RP {
    MTnode** list=malloc(sizeof(void*)*4);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    list[3]=$4;
    $$ = create_node(list,4,"Exp",&@1,Exp);
    }
	| ID LP RP {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| Exp LB Exp RB {
    MTnode** list=malloc(sizeof(void*)*4);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    list[3]=$4;
    $$ = create_node(list,4,"Exp",&@1,Exp);
    }
	| Exp DOT ID {
    MTnode** list=malloc(sizeof(void*)*3);
    list[0]=$1;
    list[1]=$2;
    list[2]=$3;
    $$ = create_node(list,3,"Exp",&@1,Exp);
    }
	| ID{
    MTnode** list=malloc(sizeof(void*)*1);
    list[0]=$1;
    $$ = create_node(list,1,"Exp",&@1,Exp);
    }
	| INT 	{
    MTnode** list=malloc(sizeof(void*)*1);
    list[0]=$1;
    $$ = create_node(list,1,"Exp",&@1,Exp);
    }
	| FLOAT {
    MTnode** list=malloc(sizeof(void*)*1);
    list[0]=$1;
    $$ = create_node(list,1,"Exp",&@1,Exp);
    }
Args : Exp COMMA Args {
     MTnode** list=malloc(sizeof(void*)*3);
     list[0]=$1;
     list[1]=$2;
     list[2]=$3;
     $$ = create_node(list,3,"Args",&@1,Args);
     }
	| Exp 	{
     MTnode** list=malloc(sizeof(void*)*1);
     list[0]=$1;
     $$ = create_node(list,1,"Args",&@1,Args);
     }
%%

#define __MY_YYERROR__
#ifdef __MY_YYERROR__

yyerror(int error_type,char* msg){
	error_detected = 1;
	if(error_type==1){
		fprintf(stderr,"Error type A at Line %d: %s '%s'\n",
			yylloc.first_line,yylval.error_node->error_type,yylval.error_node->error_token);
	}
	else if(error_type==2){
		fprintf(stderr,"Error type B at Line %d: %s\n",
			yylloc.first_line,msg);
	}
	else if(error_type==3){
	}
	else {
		fprintf(stderr,"Default call Error type B at Line %d: Unknown Error\n",
			yylloc.first_line);
	}
		
}
/*
myyyerr(int error_type,char* msg){
	error_detected = 1;
	if(error_type==1){
		fprintf(stderr,"Error type A at %d: %s '%s'\n",
			yylloc.first_line,yylval.error_node->error_type,yylval.error_node->error_token);
	}
	else{
		fprintf(stderr,"Error type B at %d: %s\n",
			yylloc.first_line,msg);
	}
}
*/
#endif
