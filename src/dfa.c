/*************************************************************************
    > File Name: dfa.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Tue 07 May 2019 11:29:33 AM IDT
*************************************************************************/

#include<stdio.h>
double 
detrend_flucuation(
        int order,                      //order of DFA
        double *y,                      //Time series data 
        int num,                        //total number of data samples 
        double *fn,                     //fn of dfa function 
        double *n,                      //n of dfa function 
        int *ssize,                       //size of dfa function 
        double *fit                     //final fit function
)
{
     /* Check the size of this data, if it is too small, I will not do the dfa */
    if (num < 1000 ){
        printf ("The data samples is not enough.");
        return 0;
    }

    int size = *ssize;
    double *x = (double *)malloc(sizeof(double)*num);
    for(int i=0;i<num;i++)
        x[i] = i;
    double max = log10 ((double) num/5.0);

    order ++;
    double *store;
    double coeff;

    store = (double *)malloc(order * sizeof(double)) ;
    double errorsq = 0.0;
    for (int i=0;i<size;i++){
        fn[i] = 0.0 ;
        n[i] = 0.0; 
    }

    int seq = 0; // index of fn and n;

    int length, end;
    for(double i = log10( (double)(order+2) ); i < max ; i+= 0.1 ){
        length = (int)pow(10.0,i);
        end = num - length;

        for(int j = 0; j < end; j += length){
            polyfit();
        }
    }


}
