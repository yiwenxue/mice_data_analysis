/*************************************************************************
    > File Name: cosinor.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 23 May 2019 12:30:54 PM IDT
*************************************************************************/

#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include<gsl/gsl_multifit.h>
#include<gsl/gsl_sf.h>
#include<gsl/gsl_statistics_double.h>

#include "mathematic.h"


/* int cosinor(double *x, */
/*             double *y, */
/*             int size, */
/*             double *store, */
/*             int degree */
/* { */
/*     gsl_matrix *X, *cov; */
/*     gsl_vector *Y, *c; */

/*     X = gsl_matrix_alloc (size,degree); */
/*     Y = gsl_vector_alloc (size); */

/*     c = gsl_vector_alloc (degree); */
/*     cov = gsl_matrix_alloc (degree,degree); */

/*     double t = 0; */
/*     double constant =0; */
/*     for(int i=0;i<size;i++) */
/*     { */
/*         gsl_matrix_set (X, i, 0, 1.0); */
/*         gsl_vector_set (Y, i, y[i]); */
/*     } */
/*     for(int j=1;j<degree;j+=2) */
/*     { */
/*         t = 24.0/(j+1)*2.0; */
/*         constant = 2*M_PI/t; */
/*         /1* printf("t = %f\n",t); *1/ */

/*         for(int i=0;i<size;i++) */
/*         { */
/*             gsl_matrix_set (X, i, j, cos(constant * x[i])); */
/*             gsl_matrix_set (X, i, j+1, sin(constant * x[i])); */
/*             /1* printf ("%d: %d\t%d\n", i, j, j+1); *1/ */
/*         } */
/*     } */

/*     double chisq = 0.0; */
/*     gsl_multifit_linear_workspace *ws */ 
/*         = gsl_multifit_linear_alloc (size,degree); */
/*     gsl_multifit_linear (X, Y, c, cov, &chisq, ws); */

/*     for(int i=0;i<degree;i++){ */
/*         store[i] = gsl_vector_get (c,i); */
/*     } */
/*     double phi=0.0,amp = 0.0; */
/*     for(int j=1;j<degree;j+=2){ */
/*         amp = sqrt( (store[j]*store[j]) + (store[j+1]*store[j+1]) ); */
/*         phi = atan(  -1 * store[j+1] / store[j] ); */
/*         store[j] = amp; */
/*         store[j+1] = phi; */
/*     } */

/*     gsl_multifit_linear_free (ws); */

/*     gsl_matrix_free (X); */
/*     gsl_vector_free (Y); */
/*     gsl_vector_free (c); */
/*     gsl_matrix_free (cov); */

/*     return 1; */ 
/* } */

int main(){
 
    double x[100],y[100];
    double t = (M_PI/12.0);
    for(int i=0;i<100;i++){
        x[i] = i;
        y[i] = 100 - 20*cos(t*x[i]-M_PI/4.0) + 10*cos(2*t*x[i]+M_PI/6.0);
    }

    double *store;
    printf("sizeof(x) = %lu\n",sizeof(x));
    int degree = 1+2*2;
    store = (double*)malloc(degree*sizeof(double));


    cosinor(x,y,sizeof(x)/sizeof(double), store, degree);
    for(int i=0;i <degree;i++){
        printf("%f\t",store[i]);
    }
    printf("\n");
    printf(" y = %.3f + %.3f * cos(%.3f * x + %.3f )+%.3f * cos(%.3f * x+ %.3f )\n",
            store[0],store[1],t,store[2],store[3],2*t,store[4]);
    free(store);

    return 0;
}
