 %{
 #include"lex.yy.c"
 %}

%locations
%define parse.error verbose

%union {
int intval;
float floatval;
char* strval;
}

 /*declare tokens
 */
%token <intval> INT
%token <floatval> FLOAT
%token <strval> ID
%token SEMI
%token COMMA
%token ASSIGNOP
%token <strval> RELOP
%token PLUS
%token MINUS
%token STAR
%token DIV
%token AND
%token OR
%token DOT
%token NOT
%token TYPE
%token LP
%token RP
%token LB
%token RB
%token LC
%token RC
%token STRUCT
%token RETURN
%token IF
%token ELSE
%token WHILE

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

%%

Program : ExtDefList 
ExtDefList : ExtDef ExtDefList 
		   | %empty
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
	   | %empty
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
		 | %empty 
Stmt : Exp SEMI 
	 | CompSt 
	 | RETURN Exp SEMI 
	 | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
	 | IF LP Exp RP Stmt ELSE Stmt 
	 | WHILE LP Exp RP Stmt

DefList : Def DefList 
		| %empty
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
	| Exp DOT ID 
	| ID	
	| INT 
	| FLOAT

Args : Exp COMMA Args 
	| Exp

%%

