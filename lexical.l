%{
  #include "string.h"
  #include "syntax.tab.h"
  #include "multi_tree.h"
  #include "error.h"
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
<COMMENTS>(.|\n)		{;}
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
	yylval.mtnode = create_node(NULL,0,str,&yylloc,RELOP);
	return RELOP;
}
<INITIAL>"=" {
	yylval.mtnode = create_node(NULL,0,"ASSIGNOP",&yylloc,ASSIGNOP);
	return ASSIGNOP;
}
<INITIAL>";" {
	yylval.mtnode = create_node(NULL,0,"SEMI",&yylloc,SEMI);
	return SEMI;
}
<INITIAL>"," {
	yylval.mtnode = create_node(NULL,0,"COMMA",&yylloc,COMMA);
	return COMMA;
}
<INITIAL>"+" {
	yylval.mtnode = create_node(NULL,0,"PLUS",&yylloc,PLUS);
	return PLUS;
}
<INITIAL>"-" {
	yylval.mtnode = create_node(NULL,0,"MINUS",&yylloc,MINUS);
	return MINUS;
}
<INITIAL>"*" {
	yylval.mtnode = create_node(NULL,0,"STAR",&yylloc,STAR);
	return STAR;
}
<INITIAL>"/" {
	yylval.mtnode = create_node(NULL,0,"DIV",&yylloc,DIV);
	return DIV;
}
<INITIAL>"&&" {
	yylval.mtnode = create_node(NULL,0,"AND",&yylloc,AND);
	return AND;
}
<INITIAL>"||" {
	yylval.mtnode = create_node(NULL,0,"OR",&yylloc,OR);
	return OR;
}
<INITIAL>"." {
	yylval.mtnode = create_node(NULL,0,"DOT",&yylloc,DOT);
	return DOT;
}
<INITIAL>"!" {
	yylval.mtnode = create_node(NULL,0,"NOT",&yylloc,NOT);
	return NOT;
}
<INITIAL>"(" {
	yylval.mtnode = create_node(NULL,0,"LP",&yylloc,LP);
	return LP;
}
<INITIAL>")" {
	yylval.mtnode = create_node(NULL,0,"RP",&yylloc,RP);
	return RP;
}
<INITIAL>"[" {
	yylval.mtnode = create_node(NULL,0,"LB",&yylloc,LB);
	return LB;
}
<INITIAL>"]" {
	yylval.mtnode = create_node(NULL,0,"RB",&yylloc,RB);
	return RB;
}
<INITIAL>"{" {
	yylval.mtnode = create_node(NULL,0,"LC",&yylloc,LC);
	return LC;
}
<INITIAL>"}" {
	yylval.mtnode = create_node(NULL,0,"RC",&yylloc,RC);
	return RC;
}
<INITIAL>"struct" {
	yylval.mtnode = create_node(NULL,0,"STRUCT",&yylloc,STRUCT);
	return STRUCT;
}
<INITIAL>"return" {
	yylval.mtnode = create_node(NULL,0,"RETURN",&yylloc,RETURN);
	return RETURN;
}
<INITIAL>"if" {
	yylval.mtnode = create_node(NULL,0,"IF",&yylloc,IF);
	return IF;
}
<INITIAL>"else" {
	yylval.mtnode = create_node(NULL,0,"ELSE",&yylloc,ELSE);
	return ELSE;
}
<INITIAL>"while" {
	yylval.mtnode = create_node(NULL,0,"WHILE",&yylloc,WHILE);
	return WHILE;
}
<INITIAL>"int"|"float" {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,TYPE);
	return TYPE;
}


 /*build-in types
 */
<INITIAL>{digit}*"."{digit}*[Ee][+-]?{digit}+ {
	char* str = strdup(yytext);
	yylval.error_node->error_token = str;
	yylval.error_node->error_type = "scientific notation(unsupported)";
	return ERROR;
}
<INITIAL>{digit}+"."{digit}+ {
	char* str = strdup(yytext);
	yylval.error_node->error_token = str;
	yylval.error_node->error_type = "float(unsupported)";
	return ERROR;
}
<INITIAL>{digit}{10}{digit}+ {
	char* str = strdup(yytext);
	yylval.error_node->error_token = str;
	yylval.error_node->error_type = "too large number";
	return ERROR;
}
<INITIAL>{digit_0}{digit}{0,9}|"0" {
	if(atoll(yytext)>0xffffffff){
		char* str = strdup(yytext);
		yylval.error_node->error_token = str;
		yylval.error_node->error_type = "too large number";
		return ERROR;
	}
	else{
		char* str = strdup(yytext);
		yylval.mtnode = create_node(NULL,0,str,&yylloc,INT);
		return INT;
	}
}
<INITIAL>"0x"[0-9a-f]{1,8}|"0X"[0-9A-F]{1,8} {//hex
	char* str = strdup(yytext);
	yylval.error_node->error_token = str;
	yylval.error_node->error_type = "hex (unsupported)";
	return ERROR;
}
<INITIAL>"0"[0-7]{1,16} { //oct
	char* str = strdup(yytext);
	yylval.error_node->error_token = str;
	yylval.error_node->error_type = "oct (unsupported)";
	return ERROR;
}

<INITIAL>{alpha_}({alpha_}|{digit})* {
	char* str = strdup(yytext);
	yylval.mtnode = create_node(NULL,0,str,&yylloc,ID);
	return ID;
}
 /*
 *error
 */
<INITIAL>. {
	char* str = strdup(yytext);
	yylval.error_node->error_token = str;
	yylval.error_node->error_type = "mysterious token";
	return ERROR;
}

%%
