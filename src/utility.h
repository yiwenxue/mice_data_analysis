/*************************************************************************
    > File Name: utility.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Tue 07 May 2019 06:26:16 PM IDT
*************************************************************************/

#include <string.h>
#include "mathematic.h"
#include "plot.h"

char name[255], type[255], path[255];

int mice_name(char *inname,char *path, char *name, char *type);

int mice38_dfa(double *data, int size, int order, int duration, Gtype graphtype, int outputf);

int mice38_mean(double *data, int size, int duration, Gtype graphtype, int outputf);

int mice38_std(double *data, int size, int duration, Gtype graphtype, int outputf);

int check_circadian(char *filename,int breakpoint);

int mice_dfa(char *name,char *path,char *type,double *data, int size,int order, int duration);

int mice_dfap(char *name,char *path,char *type,double *data, int size, int point,int order, int duration);

int mice_mean(char *name,char *path,char *type,double *data, int size, int duration);

int mice_std(char *name,char *path,char *type,double *data, int size, int duration);
