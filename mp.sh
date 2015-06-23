flex lexical.l
bison -d syntax.y
gcc main.c syntax.tab.c multi_tree.c multi_tree.h -lfl -ly -o parser
