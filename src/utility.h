/*************************************************************************
    > File Name: usage.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Tue 07 May 2019 06:26:16 PM IDT
*************************************************************************/

#include "mathematic.h"
#include "plot.h"
#include "file.h"

void mice_dfa(int order, double *data,double *fit, int head);

void mice_dfas(int order, double *data,int line);

void mice_std(double *func,double *data,int line);

void mice_mean(double *func,double *data,int line);

int check_circadian(char *filename,int breakpoint);

int mean_dev(char *filename, int breakpoint, int order);
int dfa_mean_dev(char *filename, int breakpoint, int order);
