flex lexical.l
bison -d -t syntax.y
gcc main.c syntax.tab.c multi_tree.c multi_tree.h -lfl -ly -o parser -Wall -Wno-unused-function
