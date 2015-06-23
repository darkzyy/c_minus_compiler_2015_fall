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


Program : ExtDefList {
		MTnode** list=malloc(sizeof(void*)*1);
		list[0]=$1;
		$$ = create_node(list,1,"Program",&@1,NONTERM);
		pre_tranverse($$);
		}
ExtDefList : ExtDef ExtDefList {
		   MTnode** list=malloc(sizeof(void*)*2);
		   list[0]=$1;
		   list[1]=$2;
		   $$ = create_node(list,2,"ExtDefList",&@1,NONTERM);
		   }
		   | %empty                {$$ = create_node(NULL,0,"",&@$,NONTERM);}

ExtDef : Specifier ExtDecList SEMI {
	   MTnode** list=malloc(sizeof(void*)*3);
	   list[0]=$1;
	   list[1]=$2;
	   list[2]=$3;
	   $$ = create_node(list,3,"ExtDef",&@1,NONTERM);
	   }
	   | Specifier SEMI {
	   MTnode** list=malloc(sizeof(void*)*2);
	   list[0]=$1;
	   list[1]=$2;
	   $$ = create_node(list,2,"ExtDef",&@1,NONTERM);
	   }
	   | Specifier FunDec CompSt {
	   MTnode** list=malloc(sizeof(void*)*3);
	   list[0]=$1;
	   list[1]=$2;
	   list[2]=$3;
	   $$ = create_node(list,3,"ExtDef",&@1,NONTERM);
	   }
ExtDecList : VarDec {
		   MTnode** list=malloc(sizeof(void*)*1);
		   list[0]=$1;
		   $$ = create_node(list,1,"ExtDecList",&@1,NONTERM);
		   }
		   | VarDec COMMA ExtDecList {
		   MTnode** list=malloc(sizeof(void*)*3);
		   list[0]=$1;
		   list[1]=$2;
		   list[2]=$3;
		   $$ = create_node(list,3,"ExtDecList",&@1,NONTERM);
		   }
Specifier : TYPE {
		  MTnode** list=malloc(sizeof(void*)*1);
		  list[0]=$1;
		  $$ = create_node(list,1,"Specifier",&@1,NONTERM);
		  }
		  | StructSpecifier {
		  MTnode** list=malloc(sizeof(void*)*1);
		  list[0]=$1;
		  $$ = create_node(list,1,"Specifier",&@1,NONTERM);
		  }
StructSpecifier : STRUCT OptTag LC DefList RC {
				MTnode** list=malloc(sizeof(void*)*5);
				list[0]=$1;
				list[1]=$2;
				list[2]=$3;
				list[3]=$4;
				list[4]=$5;
				$$ = create_node(list,5,"StructSpecifier",&@1,NONTERM);
				}
				| STRUCT Tag {
				MTnode** list=malloc(sizeof(void*)*2);
				list[0]=$1;
				list[1]=$2;
				$$ = create_node(list,2,"StructSpecifier",&@1,NONTERM);
				}
OptTag : ID {
	   MTnode** list=malloc(sizeof(void*)*1);
	   list[0]=$1;
	   $$ = create_node(list,1,"OptTag",&@1,NONTERM);
	   }
	   | %empty                {$$ = create_node(NULL,0,"",&@$,NONTERM);}

Tag : ID {
	MTnode** list=malloc(sizeof(void*)*1);
	list[0]=$1;
	$$ = create_node(list,1,"Tag",&@1,NONTERM);
	}
VarDec : ID {
	   MTnode** list=malloc(sizeof(void*)*1);
	   list[0]=$1;
	   $$ = create_node(list,1,"VarDec",&@1,NONTERM);
	   }
	   | VarDec LB INT RB {
	   MTnode** list=malloc(sizeof(void*)*4);
	   list[0]=$1;
	   list[1]=$2;
	   list[2]=$3;
	   list[3]=$4;
	   $$ = create_node(list,4,"VarDec",&@1,NONTERM);
	   }
FunDec : ID LP VarList RP {
	   MTnode** list=malloc(sizeof(void*)*4);
	   list[0]=$1;
	   list[1]=$2;
	   list[2]=$3;
	   list[3]=$4;
	   $$ = create_node(list,4,"FunDec",&@1,NONTERM);
	   }
	   | ID LP RP {
	   MTnode** list=malloc(sizeof(void*)*3);
	   list[0]=$1;
	   list[1]=$2;
	   list[2]=$3;
	   $$ = create_node(list,3,"FunDec",&@1,NONTERM);
	   }
VarList : ParamDec COMMA VarList {
		MTnode** list=malloc(sizeof(void*)*3);
		list[0]=$1;
		list[1]=$2;
		list[2]=$3;
		$$ = create_node(list,3,"VarList",&@1,NONTERM);
		}
		| ParamDec {
		MTnode** list=malloc(sizeof(void*)*1);
		list[0]=$1;
		$$ = create_node(list,1,"VarList",&@1,NONTERM);
		}
ParamDec : Specifier VarDec {
		 MTnode** list=malloc(sizeof(void*)*2);
		 list[0]=$1;
		 list[1]=$2;
		 $$ = create_node(list,2,"ParamDec",&@1,NONTERM);
		 }
CompSt : LC DefList StmtList RC {
	   MTnode** list=malloc(sizeof(void*)*4);
	   list[0]=$1;
	   list[1]=$2;
	   list[2]=$3;
	   list[3]=$4;
	   $$ = create_node(list,4,"CompSt",&@1,NONTERM);
	   }
StmtList : Stmt StmtList {
		 MTnode** list=malloc(sizeof(void*)*2);
		 list[0]=$1;
		 list[1]=$2;
		 $$ = create_node(list,2,"StmtList",&@1,NONTERM);
		 }
		 | %empty                 {$$ = create_node(NULL,0,"",&@$,NONTERM);}

Stmt : Exp SEMI {
	 MTnode** list=malloc(sizeof(void*)*2);
	 list[0]=$1;
	 list[1]=$2;
	 $$ = create_node(list,2,"Stmt",&@1,NONTERM);
	 }
	 | CompSt {
	 MTnode** list=malloc(sizeof(void*)*1);
	 list[0]=$1;
	 $$ = create_node(list,1,"Stmt",&@1,NONTERM);
	 }
	 | RETURN Exp SEMI {
	 MTnode** list=malloc(sizeof(void*)*3);
	 list[0]=$1;
	 list[1]=$2;
	 list[2]=$3;
	 $$ = create_node(list,3,"Stmt",&@1,NONTERM);
	 }
	 | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
	 MTnode** list=malloc(sizeof(void*)*5);
	 list[0]=$1;
	 list[1]=$2;
	 list[2]=$3;
	 list[3]=$4;
	 list[4]=$5;
	 $$ = create_node(list,7,"Stmt",&@1,NONTERM);
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
	 $$ = create_node(list,7,"Stmt",&@1,NONTERM);
	 }
	 | WHILE LP Exp RP Stmt {
	 MTnode** list=malloc(sizeof(void*)*5);
	 list[0]=$1;
	 list[1]=$2;
	 list[2]=$3;
	 list[3]=$4;
	 list[4]=$5;
	 $$ = create_node(list,5,"Stmt",&@1,NONTERM);
	 }
DefList : Def DefList {
		MTnode** list=malloc(sizeof(void*)*2);
		list[0]=$1;
		list[1]=$2;
		$$ = create_node(list,2,"DefList",&@1,NONTERM);
		}
		| %empty                {$$ = create_node(NULL,0,"",&@$,NONTERM);}

Def : Specifier DecList SEMI {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Def",&@1,NONTERM);
	}
DecList : Dec {
		MTnode** list=malloc(sizeof(void*)*1);
		list[0]=$1;
		$$ = create_node(list,1,"DecList",&@1,NONTERM);
		}
		| Dec COMMA DecList {
		MTnode** list=malloc(sizeof(void*)*3);
		list[0]=$1;
		list[1]=$2;
		list[2]=$3;
		$$ = create_node(list,3,"DecList",&@1,NONTERM);
		}
Dec : VarDec {
	MTnode** list=malloc(sizeof(void*)*1);
	list[0]=$1;
	$$ = create_node(list,1,"Dec",&@1,NONTERM);
	}
	| VarDec ASSIGNOP Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Dec",&@1,NONTERM);
	}
Exp : Exp ASSIGNOP Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp AND Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp OR Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp RELOP Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp PLUS Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp MINUS Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp STAR Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp DIV Exp {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| LP Exp RP {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| MINUS Exp {
	MTnode** list=malloc(sizeof(void*)*2);
	list[0]=$1;
	list[1]=$2;
	$$ = create_node(list,2,"Exp",&@1,NONTERM);
	}
	| NOT Exp {
	MTnode** list=malloc(sizeof(void*)*2);
	list[0]=$1;
	list[1]=$2;
	$$ = create_node(list,2,"Exp",&@1,NONTERM);
	}
	| ID LP Args RP {
	MTnode** list=malloc(sizeof(void*)*4);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	list[3]=$4;
	$$ = create_node(list,4,"Exp",&@1,NONTERM);
	}
	| ID LP RP {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| Exp LB Exp RB {
	MTnode** list=malloc(sizeof(void*)*4);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	list[3]=$4;
	$$ = create_node(list,4,"Exp",&@1,NONTERM);
	}
	| Exp DOT ID {
	MTnode** list=malloc(sizeof(void*)*3);
	list[0]=$1;
	list[1]=$2;
	list[2]=$3;
	$$ = create_node(list,3,"Exp",&@1,NONTERM);
	}
	| ID {
	MTnode** list=malloc(sizeof(void*)*1);
	list[0]=$1;
	$$ = create_node(list,1,"Exp",&@1,NONTERM);
	}
	| INT {
	MTnode** list=malloc(sizeof(void*)*1);
	list[0]=$1;
	$$ = create_node(list,1,"Exp",&@1,NONTERM);
	}
	| FLOAT {
	MTnode** list=malloc(sizeof(void*)*1);
	list[0]=$1;
	$$ = create_node(list,1,"Exp",&@1,NONTERM);
	}
Args : Exp COMMA Args {
	 MTnode** list=malloc(sizeof(void*)*3);
	 list[0]=$1;
	 list[1]=$2;
	 list[2]=$3;
	 $$ = create_node(list,3,"Args",&@1,NONTERM);
	 }
	 | Exp {
	 MTnode** list=malloc(sizeof(void*)*1);
	 list[0]=$1;
	 $$ = create_node(list,1,"Args",&@1,NONTERM);
	 }

%%
