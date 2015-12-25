#ifndef __API_H__
#define __API_H__

char* data_seg = ".data\n_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n";
char* global_seg = ".globl main\n";
char* text_seg_start = ".text\n";
char* func_read = "read:\n  li $v0, 4\n  la $a0, _prompt\n  syscall\n  li $v0, 5\n  syscall\n  jr $ra\n";
char* func_write = "write:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n";

#endif
