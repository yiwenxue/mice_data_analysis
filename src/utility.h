/*************************************************************************
    > File Name: utility.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Tue 07 May 2019 06:26:16 PM IDT
*************************************************************************/

#include <string.h>
#include "mathematic.h"
#include "plot.h"

int mice_name(char *inname,char *path, char *name, char *type);

int mice_dfa(char *name, char *type, double *data, int size, int order, int duration, Gtype graphtype, int outputf, char *outname);

int mice_mean(char *name, char *type, double *data, int size, int duration, Gtype graphtype, int outputf, char *outname);

int mice_std(char *name, char *type, double *data, int size, int duration, Gtype graphtype, int outputf, char *outname);

int check_circadian(char *filename,int breakpoint);

