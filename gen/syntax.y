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
	| Exp DOT ID 
	| ID 		
	| INT 	
	| FLOAT
Args : Exp COMMA Args 
	| Exp 	

