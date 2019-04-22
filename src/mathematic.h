/*************************************************************************
    > File Name: mathematic.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 18 Apr 2019 12:36:36 PM IDT
*************************************************************************/

#include<math.h>
#include<gsl/gsl_multifit.h>
#include<gsl/gsl_statistics_double.h>
#include<gsl/gsl_sf.h>
#include<stdbool.h>


bool                    //be of no use
polyfit(int size,       //the total number of the data 
        int degree,     //degree of the polynomial fit 
        double *dx,     //data of x 
        double *dy,     //data of y 
        double *store,  //array to store the coefficiencies 
        double *coeff   //double to store the err of this fit R square
    );


double detrend_flucuation(int order, double *y, int num, double *fn, double *n, int *ssize, double *fit);

double fit_rms_window(double *x,double *y,int size,double *store,int order);

double fit_diff(double x,double y,double *store,int order);

double stats_std(double *data,int size);

double stats_var(double* data, int size);

double stats_mean(double *data,int size); 

int fit_sin4(double *func,double *x,double *y,int size);
