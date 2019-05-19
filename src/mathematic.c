/*************************************************************************
    > File Name: mathematic.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 18 Apr 2019 12:36:16 PM IDT
*************************************************************************/

#include<mathematic.h>

/*
 * This function can do polynomial fits 
 * But the accuracy of this function decrease 
 * sharply when the degree is larger than 6(And it does depend on the data).
 * Maybe I should add more code to improve it.
 *
 * Here, I remove the average of x and then do the polynomial fit. 
 * After these progresses, I move the function on x axis. This improves the accuracy for some data.
 *
 * Author: Yiwen 
 * Date: 26/3/2019
 * */
bool                        //be of no use
polyfit(int size,           //the total number of the data 
        int degree,         //degree of the polynomial fit 
        double *ddx,         //data of x 
        double *dy,         //data of y 
        double *store,      //array to store the coefficiencies 
        double *coeff       //double to store the err of this fit R square
        )
{

    gsl_multifit_linear_workspace *ws;
    gsl_matrix *cov, *X;
    gsl_vector *y, *c;
    double chisq;

    int i, j;

    X = gsl_matrix_alloc(size, degree);
    y = gsl_vector_alloc(size);
    c = gsl_vector_alloc(degree);
    cov = gsl_matrix_alloc(degree, degree);

    double meanx = gsl_stats_mean(ddx,1,size); 
    //printf("mean= %lf\n",meanx);

    double *dx;
    dx = (double*)malloc(sizeof(double)*size);
    for(int i=0;i<size;i++){
        dx[i]=ddx[i]-meanx;
        /* printf("%lf\t",ddx[i]); */
        /* printf("%lf\n",dy[i]); */
    }

    for(i=0; i < size; i++) {
        for(j=0; j < degree; j++) {
            gsl_matrix_set(X, i, j, pow(dx[i], j));
        }
        gsl_vector_set(y, i, dy[i]);
    }
    ws = gsl_multifit_linear_alloc(size, degree);
    gsl_multifit_linear(X, y, c, cov, &chisq, ws);
    *coeff = 1.0 - chisq/gsl_stats_tss(dy,1,size);
    /* store result ... */
    for(i=0; i < degree; i++)
    {
        store[i] = gsl_vector_get(c, i);
    }

    for(i=0;i<degree;i++){
        for(int j=i+1;j<degree;j++){
            int order = gsl_sf_fact(j)/gsl_sf_fact(i)/gsl_sf_fact(j-i);
            store[i]+= store[j]*order*pow(-1*meanx,j-i);
            //printf("store[%d] + %d*a[%d]*(-meanx)^%d\n",i,order,j,j-i);
        }
        //printf("\n");
    }

    gsl_multifit_linear_free(ws);
    gsl_matrix_free(X);
    gsl_matrix_free(cov);
    gsl_vector_free(y);
    gsl_vector_free(c);
    free(dx);
    return true;
}

typedef struct{
    double x;
    double y;
}fun2;

/*
 * A dfa program using gsl polynomial regression fit function.
 *
 * Author: Yiwen
 * Date: 15/4/2019
 * */

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
    if (num < 1000 ){
        printf ("The data samples is not enough.");
        return 0;
    }

    /* first, You should remove the mean, and make a integration */ 
    double mean = gsl_stats_mean(y,1,num);
    
    double *tempy = (double*)malloc(sizeof(double)*num);
    tempy[0] = y[0] - mean;
    for(int i=1;i<num;i++){
        tempy[i] = y[i] - mean + tempy[i-1];
    }


    int size = *ssize;
    double *x = (double *)malloc(sizeof(double)*num);
    for(int i=0;i<num;i++)
        x[i] = i;
    double max = log10 ((double) num/5.0);

    order ++;
    double *store;
    double coeff=0;

    store=(double *)malloc(sizeof(double)*order);
    double errorsq=0.0; 

    for(int i=0;i<size;i++){
        fn[i] = 0.0;
        n[i] = 0.0;
    }

    int seq=0; // index of f(n)~n
    int length =0, end = 0,tail =0;
    for (double i=log10 ((double)(order + 2)); i<max; i+=0.1){ 
        length = (int)pow(10,i);
        end = num - length; 
        for (int j=0; j < end; j+= length){
            polyfit (length,order,(x),(tempy+j),store,&coeff);
            fn[seq] += fit_rms_window(x,(tempy+j),length,store,order);
            
            tail = j + length -1;
        }
        n[seq] = length;
        fn[seq] = sqrt(*(fn+seq)/(double)num);
        seq ++;
    }

    double *log10fn ;
    double *log10n ;
    log10n = (double *)malloc(sizeof(double )*size);
    log10fn = (double *)malloc(sizeof(double )*size);
    for(int i=0;i<seq;i++){
        log10fn[i] = log10(fn[i]);
        log10n[i] = log10(n[i]);
    }

    polyfit(seq,2,log10n,log10fn,fit,&coeff);

    *ssize = seq; 
    free(x);
    free(store);
    free(tempy);
    free(log10fn);
    free(log10n);
    return fit[1];
}

double fit_rms_window(double *x,double *y,int size,double *store,int order){
    double rms = 0.0;
    for(int i=0;i<size;i++){
        rms += pow(fit_diff(x[i],y[i],store,order),2.0);
    }
    return rms;
}

/* The except value - origin value */ 
double fit_diff(double x,double y,double *store,int order){
    double expect = 0; // double expect; WRONG!!! BC Value must be initializaed !!!!!!!!!!!!
    for(int l=0;l<order;l++){
        expect += store[l] * pow(x,(double)l);
    }
    expect -= y;
    return  expect;
}

double stats_std(double *data,int size){
    return sqrt(stats_var(data,size));
}

double stats_var(double* data, int size){
    double mean = stats_mean(data,size);
    double sum =0;
    for(int i=0;i<size;i++){
        sum += pow((data[i] - mean),2.0);
    }
    sum /= size;
    return sum;
}

double stats_mean(double *data,int size){
    double sum =0;
    for(int i=0;i<size;i++){
        sum += data[i];
    }
    sum /= size;
    return sum;
}

int fit_sin4(double *func,double *x,double *y,int size){
    gsl_matrix *X, *cov;
    gsl_vector *Y,*c;

    X = gsl_matrix_alloc (size, 5);
    Y = gsl_vector_alloc (size);

    c = gsl_vector_alloc (5);
    cov = gsl_matrix_alloc (5, 5);

    for(int i=0;i<size;i++){
        gsl_matrix_set (X, i, 0, 1.0);
        gsl_matrix_set (X, i, 1, sin(x[i]));
        gsl_matrix_set (X, i, 2, cos(x[i]));
        gsl_matrix_set (X, i, 3, sin(2*x[i]));
        gsl_matrix_set (X, i, 4, cos(2*x[i]));

        gsl_vector_set (Y, i, y[i]);
    }

    double chisq =0;
    gsl_multifit_linear_workspace * work
        = gsl_multifit_linear_alloc (size, 5);
    gsl_multifit_linear (X, Y, c, cov,
            &chisq, work);
    for(int i=0;i<5;i++){
        func[i] = gsl_vector_get(c,i);
    }
    gsl_multifit_linear_free (work);

    gsl_matrix_free (X);
    gsl_vector_free (Y);
    gsl_vector_free (c);
    gsl_matrix_free (cov);
    return 1;
}
