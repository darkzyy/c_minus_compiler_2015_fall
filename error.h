#ifndef __ERROR_H__
#define __ERROR_H__

typedef struct lex_error_msg{
	char* error_token;
	char* error_type;
}lex_error_msg;

enum{lexical_error = 1,
	syntax_error_known,
	syntax_error_unknown};

typedef struct error_list{
	int error_type;
	int lineno;
	struct lex_error_msg *error_node;//for lexical error	
	char* error_msg; //for known syntax error
	struct error_list* next;
}error_list;

#endif
