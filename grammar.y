%{

%}

%%



Program1 :  ExtDefList
ExtDefList1 :  ExtDef ExtDefList
ExtDefList2 :  %empty
ExtDef1 :  Specifier ExtDecList SEMI
ExtDef2 :  Specifier SEMI
ExtDef3 :  Specifier FunDec CompSt
ExtDef4 :  Specifier FunDec SEMI
ExtDecList1 :  VarDec
ExtDecList2 :  VarDec COMMA ExtDecList


Specifier1 :  TYPE
Specifier2 :  StructSpecifier
StructSpecifier1 :  STRUCT OptTag LC DefList RC
StructSpecifier2 :  STRUCT Tag
OptTag1 :  ID
OptTag2 :  %empty
Tag1 :  ID


VarDec1 :  ID
VarDec2 :  VarDec LB INT RB
FunDec1 :  ID LP VarList RP
FunDec2 :  ID LP RP
VarList1 :  ParamDec COMMA VarList
VarList2 :  ParamDec
ParamDec1 :  Specifier VarDec


CompSt1 :  LC DefList StmtList RC
StmtList1 :  Stmt StmtList
StmtList2 :  %empty
Stmt1 :  Exp SEMI
Stmt2 :  CompSt
Stmt3 :  RETURN Exp SEMI
Stmt4 :  IF LP Exp RP Stmt
Stmt5 :  IF LP Exp RP Stmt ELSE Stmt
Stmt6 :  WHILE LP Exp RP Stmt


DefList1 :  Def DefList
DefList2 :  %empty
Def1 :  Specifier DecList SEMI
DecList1 :  Dec
DecList2 :  Dec COMMA DecList
Dec1 :  VarDec
Dec2 :  VarDec ASSIGNOP Exp


Exp1 :  Exp ASSIGNOP Exp
Exp2 :  Exp AND Exp
Exp3 :  Exp OR Exp
Exp4 :  Exp RELOP Exp
Exp5 :  Exp PLUS Exp
Exp6 :  Exp MINUS Exp
Exp7 :  Exp STAR Exp
Exp8 :  Exp DIV Exp
Exp9 :  LP Exp RP
Exp10 :  MINUS Exp
Exp11 :  NOT Exp
Exp12 :  ID LP Args RP
Exp13 :  ID LP RP
Exp14 :  Exp LB Exp RB
Exp15 :  Exp DOT ID
Exp16 :  ID
Exp17 :  INT
Exp18 :  FLOAT
Args1 :  Exp COMMA Args
Args2 :  Exp
%%

