/*************************************************************************
    > File Name: plot.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 18 Apr 2019 12:42:01 PM IDT
*************************************************************************/

#include <stdio.h>

FILE* gnuplot_create_t(void);

void plot_cmd(FILE* gnuplotPipe, char* command);

bool plot_std(int width,int height);

bool plot_dfa(int order,int width,int height);

bool plot_dfas(int order,int width,int height);
