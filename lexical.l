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
	//yylval.strval = strdup(yytext);
	//printf("relationship operators %s\n",yytext);
	return RELOP;
}
<INITIAL>"=" {
	return ASSIGNOP;
}
<INITIAL>";" {
	//printf("SEMI\n");
	return SEMI;
}
<INITIAL>"," {
	//printf("COMMA\n");
	return COMMA;
}
<INITIAL>"+" {
	//printf("PLUS\n");
	return PLUS;
}
<INITIAL>"-" {
	//printf("MINUS\n");
	return MINUS;
}
<INITIAL>"*" {
	//printf("STAR\n");
	return STAR;
}
<INITIAL>"/" {
	//printf("DIV\n");
	return DIV;
}
<INITIAL>"&&" {
	//printf("AND\n");
	return AND;
}
<INITIAL>"||" {
	//printf("OR\n");
	return OR;
}
<INITIAL>"." {
	//printf("DOT\n");
	return DOT;
}
<INITIAL>"!" {
	//printf("NOT\n");
	return NOT;
}
<INITIAL>"(" {
	//printf("LP\n");
	return LP;
}
<INITIAL>")" {
	//printf("RP\n");
	return RP;
}
<INITIAL>"[" {
	//printf("LB\n");
	return LB;
}
<INITIAL>"]" {
	//printf("RB\n");
	return RB;
}
<INITIAL>"{" {
	//printf("LC\n");
	return LC;
}
<INITIAL>"}" {
	//printf("RC\n");
	return RC;
}
<INITIAL>"struct" {
	//printf("STRUCT\n");
	return STRUCT;
}
<INITIAL>"return" {
	//printf("RETURN\n");
	return RETURN;
}
<INITIAL>"if" {
	//printf("IF\n");
	return IF;
}
<INITIAL>"else" {
	//printf("ELSE\n");
	return ELSE;
}
<INITIAL>"while" {
	//printf("WHILE\n");
	return WHILE;
}
<INITIAL>"int"|"float" {
	//yylval.strval = strdup(yytext);
	return TYPE;
}


 /*build-in types
 */
<INITIAL>{digit}*"."{digit}*[Ee][+-]?{digit}+ {
	//printf("float: %s\n",yytext);
	//yylval.strval = strdup(yytext);
	return FLOAT;
}
<INITIAL>{digit}+"."{digit}+ {
	//printf("float: %s\n",yytext);
	//yylval.strval = strdup(yytext);
	return FLOAT;
}
<INITIAL>{digit}{10}{digit}+ {
	printf("Error type A at Line %d : value of numebr is too large\'%s\'\n",
	yylineno,yytext);
}
<INITIAL>{digit_0}{digit}{0,9}|"0" {
	if(atoll(yytext)>0xffffffff){
		printf("Error type A at Line %d : value of numebr is too large\'%s\'\n",
		yylineno,yytext);
	}
	else{
		//printf("Int value: %d\n",atoi(yytext));
		//yylval.intval = atoi(yytext);
		return INT;
	}
}
<INITIAL>"0x"[0-9a-f]{1,8}|"0X"[0-9A-F]{1,8} {
	//printf("Hex: %s\n",yytext);
	//yylval.strval = strdup(yytext);
	return INT;
}
<INITIAL>"0"[0-7]{1,16} {
	//printf("Oct: %s\n",yytext);
	//yylval.strval = strdup(yytext);
	return INT;
}

<INITIAL>{alpha_}({alpha_}|{digit})* {
	//printf("identification : %s\n",yytext);
	return ID;
}
 /*
 *error
 */
<INITIAL>. {
	printf("Error type at Line %d : mysterious token: '%s'\n",
	yylineno,yytext);
}

%%
