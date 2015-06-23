 %{
#include"lex.yy.c"
 %}

%locations
%define parse.error verbose

%union {
struct MTnode* mtnode;
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

%%

Program : ExtDefList 
ExtDefList : ExtDef ExtDefList 
		   | %empty				{$$ = create_node(NULL,0,"",&@$,NONTERM);}
ExtDef : Specifier ExtDecList SEMI 
	   | Specifier SEMI 
	   | Specifier FunDec CompSt 
ExtDecList : VarDec 
		   | VarDec COMMA ExtDecList

Specifier : TYPE 
		  | StructSpecifier 
StructSpecifier : STRUCT OptTag LC DefList RC 
				| STRUCT Tag
OptTag : ID
	   | %empty				{$$ = create_node(NULL,0,"",&@$,NONTERM);}
Tag : ID

VarDec : ID 
	   | VarDec LB INT RB 
FunDec : ID LP VarList RP 
	   | ID LP RP 
VarList : ParamDec COMMA VarList 
		| ParamDec 
ParamDec : Specifier VarDec

CompSt : LC DefList StmtList RC 
StmtList : Stmt StmtList 
		 | %empty 				{$$ = create_node(NULL,0,"",&@$,NONTERM);}
Stmt : Exp SEMI 
	 | CompSt 
	 | RETURN Exp SEMI 
	 | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
	 | IF LP Exp RP Stmt ELSE Stmt 
	 | WHILE LP Exp RP Stmt

DefList : Def DefList 
		| %empty				{$$ = create_node(NULL,0,"",&@$,NONTERM);}
Def : Specifier DecList SEMI 
DecList : Dec 
		| Dec COMMA DecList 
Dec : VarDec 
	| VarDec ASSIGNOP Exp


Exp : Exp ASSIGNOP Exp 
	| Exp AND Exp
	| Exp OR Exp 
	| Exp RELOP Exp 
	| Exp PLUS Exp 
	| Exp MINUS Exp 
	| Exp STAR Exp 
	| Exp DIV Exp 
	| LP Exp RP 
	| MINUS Exp 
	| NOT Exp 
	| ID LP Args RP 
	| ID LP RP 
	| Exp LB Exp RB 
	| Exp DOT ID {MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"EXPR",&@1,NONTERM);
	}
	| ID 				{$$ = create_node(&$1,1,"EXPR",&@1,NONTERM);}	
	| INT 				{$$ = create_node(&$1,1,"EXPR",&@1,NONTERM);}
	| FLOAT				{$$ = create_node(&$1,1,"EXPR",&@1,NONTERM);}

Args : Exp COMMA Args 
	| Exp 				{$$ = create_node(&$1,1,"",&@1,NONTERM);}

%%

