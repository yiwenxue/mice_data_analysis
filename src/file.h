/*************************************************************************
    > File Name: file.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 11 Apr 2019 11:35:48 AM IDT
*************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

#define BUFFER_SIZE 1024

//bool wrist_csv(char* );
double* read_mice_file(char* ifname, int *line);
