%{
  #include "syntax.tab.h"
  #include "multi_tree.h"
  int yycolumn = 1;
  #define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; \
	yylloc.last_column = yycolumn + yyleng - 1; \
	yycolumn += yyleng;
%}

digit	[0-9]
digit_0	[1-9]
alpha_	[a-zA-Z_]

%option yylineno
%START COMMENTS
%START STRING

%%

 /*comments
  */
<INITIAL>\/\/.*	{;}
<INITIAL>"/*"	{BEGIN COMMENTS;}
<COMMENTS>"*/"	{BEGIN INITIAL;}
<COMMENTS>(.|\n)*		{;}
<COMMENTS><<EOF>>	{
	BEGIN INITIAL;
	printf("Error type  at Line %d : comments met EOF\'%s\'\n",
	yylineno,yytext);
}

 /*spaces
 */
<INITIAL>[\ \t\b\f\r]	{;}
<INITIAL>\n		{ yycolumn = 1;}

 /*relops
 */
<INITIAL>">="|"<="|"=="|"!="|">"|"<" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return RELOP;
}
<INITIAL>"=" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return ASSIGNOP;
}
<INITIAL>";" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return SEMI;
}
<INITIAL>"," {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return COMMA;
}
<INITIAL>"+" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return PLUS;
}
<INITIAL>"-" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return MINUS;
}
<INITIAL>"*" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return STAR;
}
<INITIAL>"/" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return DIV;
}
<INITIAL>"&&" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return AND;
}
<INITIAL>"||" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return OR;
}
<INITIAL>"." {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return DOT;
}
<INITIAL>"!" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return NOT;
}
<INITIAL>"(" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return LP;
}
<INITIAL>")" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return RP;
}
<INITIAL>"[" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return LB;
}
<INITIAL>"]" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return RB;
}
<INITIAL>"{" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return LC;
}
<INITIAL>"}" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return RC;
}
<INITIAL>"struct" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return STRUCT;
}
<INITIAL>"return" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return RETURN;
}
<INITIAL>"if" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return IF;
}
<INITIAL>"else" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return ELSE;
}
<INITIAL>"while" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return WHILE;
}
<INITIAL>"int"|"float" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return TYPE;
}


 /*build-in types
 */
<INITIAL>{digit}*"."{digit}*[Ee][+-]?{digit}+ {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return ERROR;
}
<INITIAL>{digit}+"."{digit}+ {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return ERROR;
}
<INITIAL>{digit}{10}{digit}+ {
	printf("Error type A at Line %d : value of numebr is too large\'%s\'\n",
	yylineno,yytext);
	return ERROR;
}
<INITIAL>{digit_0}{digit}{0,9}|"0" {
	if(atoll(yytext)>0xffffffff){
		printf("Error type A at Line %d : value of numebr is too large\'%s\'\n",
		yylineno,yytext);
		return ERROR;
	}
	else{
		char* str = strdup(yytext);
		yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
		return INT;
	}
}
<INITIAL>"0x"[0-9a-f]{1,8}|"0X"[0-9A-F]{1,8} {//hex
	return ERROR;
}
<INITIAL>"0"[0-7]{1,16} { //oct
	return ERROR;
}

<INITIAL>{alpha_}({alpha_}|{digit})* {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TERMINALS);
	return ID;
}
 /*
 *error
 */
<INITIAL>. {
	printf("Error type at Line %d : mysterious token: '%s'\n",
	yylineno,yytext);
	return ERROR;
}

%%
