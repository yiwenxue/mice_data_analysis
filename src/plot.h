/*************************************************************************
    > File Name: plot.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 18 Apr 2019 12:42:01 PM IDT
*************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum{
    GAVERAGE,
    GINDIVIDUAL,
    GDEVIATION,
    GMID,
    GMIN,
    GMAX,
    GPOWERSPEC, 
    GCHECK
}Gtype;

FILE* gnuplot_create_t(void);

void plot_cmd(FILE* gnuplotPipe, char* command);

bool plot_mean(double *func,char *filename,int width,int height);

bool plot_std(double *func,char *filename,int width,int height);

bool plot_dfas(char *filename,int order,int width,int height);

bool plot_dfa(char *filename,double *fit,int order,int width,int height);
