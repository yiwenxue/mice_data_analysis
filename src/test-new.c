/*************************************************************************
    > File Name: main.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Wed 17 Apr 2019 07:35:07 PM IDT
 *************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "mathematic.h"
#include "plot.h"
#include "file.h"

int main(int argc,char** argv){

    int lines;
    double *x;
    double *y = read_mice_file(argv[1],&lines);
    x = (double*)malloc(sizeof(double)*lines);
    for(int i=0;i<lines;i++){
        x[i] = 10.0 * i;
    }
    int order = atoi(argv[2]);

    double mean = gsl_stats_mean(y,1,lines);

    /* y[0] -= mean; */
    for(int i=0;i<lines;i++){
        y[i] -= mean;
    }
    for(int i=0;i<lines;i++){
        y[i] += y[i-1];
    }

    int size = (int)(log10((double)lines/5.0)*100);

    double* fn = (double*)malloc(size*sizeof(double));
    double* n = (double*)malloc(size*sizeof(double));
    double fit[2];
    double hurst = detrend_flucuation(order,y,lines,fn,n,&size,fit);
    
    printf("Hurst exponent: %.5f\n",hurst);
    printf("y = %.3f*x + %.3f\n",fit[1],fit[0]);

    free(x);
    free(y);
    free(fn);
    free(n);
    return 0;
}

