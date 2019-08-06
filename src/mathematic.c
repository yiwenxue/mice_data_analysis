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
        double *store,      //array to store the coefficiencies "order increases with position";
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

/*
 * A dfa program using gsl polynomial regression fit function.
 *
 * Author: Yiwen
 * Date: 15/4/2019
 * */

double 
detrend_flucuation(int order,                      //order of DFA
                   double *y,                      //Time series data 
                   int num,                        //total number of data samples 
                   double *fn,                     //fn of dfa function 
                   double *n,                      //n of dfa function 
                   int *ssize,                       //size of dfa function 
                   double *fit                     //final fit function
)
{
    if (num < 100 ){
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
    return coeff;
}

double 
fit_rms_window(double *x,
                      double *y,
                      int size,
                      double *store,
                      int order)
{
    double rms = 0.0;
    for(int i=0;i<size;i++){
        rms += pow(fit_diff(x[i],y[i],store,order),2.0);
    }
    return rms;
}

/* The except value - origin value */ 
double 
fit_diff(double x,
                double y,
                double *store,
                int order)
{
    double expect = 0; // double expect; WRONG!!! BC Value must be initializaed !!!!!!!!!!!!
    for(int l=0;l<order;l++){
        expect += store[l] * pow(x,(double)l);
    }
    expect -= y;
    return  expect;
}

double 
stats_std(double *data,
                 int size)
{
    return sqrt(stats_var(data,size));
}

double 
stats_var(double* data, 
                 int size)
{
    double mean = stats_mean(data,size);
    double sum =0;
    for(int i=0;i<size;i++){
        sum += pow((data[i] - mean),2.0);
    }
    sum /= size;
    return sum;
}

double 
stats_mean(double *data,
                  int size)
{
    double sum =0;
    for(int i=0;i<size;i++){
        sum += data[i];
    }
    sum /= size;
    return sum;
}

double  
cosinor(double *x,
            double *y,
            int size,
            double *store,
            int degree /*(2*N+1)*/)
{
    double tss = gsl_stats_tss(y,1,size);

    gsl_matrix *X, *cov;
    gsl_vector *Y, *c;

    X = gsl_matrix_alloc (size,degree);
    Y = gsl_vector_alloc (size);

    c = gsl_vector_alloc (degree);
    cov = gsl_matrix_alloc (degree,degree);

    double t = 0;
    double constant =0;
    for(int i=0;i<size;i++)
    {
        gsl_matrix_set (X, i, 0, 1.0);
        gsl_vector_set (Y, i, y[i]);
    }
    for(int j=1;j<degree;j+=2)
    {
        t = 24.0/(j+1)*2.0;
        constant = 2*M_PI/t;
        /* printf("t = %f\n",t); */

        for(int i=0;i<size;i++)
        {
            gsl_matrix_set (X, i, j, cos(constant * x[i]));
            gsl_matrix_set (X, i, j+1, sin(constant * x[i]));
            /* printf ("%d: %d\t%d\n", i, j, j+1); */
        }
    }

    double chisq = 0.0;
    gsl_multifit_linear_workspace *ws 
        = gsl_multifit_linear_alloc (size,degree);
    gsl_multifit_linear (X, Y, c, cov, &chisq, ws);

    for(int i=0;i<degree;i++){
        store[i] = gsl_vector_get (c,i);
    }
    double phi=0.0,amp = 0.0;
    for(int j=1;j<degree;j+=2){
        phi = atan(  -1 * store[j+1] / store[j] );
        amp = store[j] / cos(phi);
        store[j] = amp;
        store[j+1] = phi;
    }
    
    double rsq = 1.0 - chisq /tss;
    fprintf(stdout,"Cosinor fit -- R^2=%.3f\n",rsq);

    gsl_multifit_linear_free (ws);

    gsl_matrix_free (X);
    gsl_vector_free (Y);
    gsl_vector_free (c);
    gsl_matrix_free (cov);

    return rsq; 
}

double get_mid(double *data, int step, int num){
    double *out;
    num = num/step;
    out = (double *)malloc(sizeof(double)*num);
    for(int i=0;i<num;i++){
        out[i] = data[i*step];
    }
    double temp =0;
    for(int i=0;i<num;i++){
        for(int j=0;j<i;j++){
            if(out[j] > out[i]){
                temp = out[i]; 
                out[i] = out[j];
                out[j] = temp;
            }
        }
    }
    double mid;
    if(num%2==0){
        mid = out[num/2] + out[num/2-1];
        mid /= 2.0;
    }else{
        mid = out[(num-1)/2];
    }
    free(out);
    return mid;
}

double get_max(double *data, int step, int num){
    double *out;
    num = num/step;
    out = (double *)malloc(sizeof(double)*num);
    for(int i=0;i<num;i++){
        out[i] = data[i*step];
    }
    double temp =0;
    for(int i=0;i<num;i++){
        for(int j=0;j<i;j++){
            if(out[j] > out[i]){
                temp = out[i]; 
                out[i] = out[j];
                out[j] = temp;
            }
        }
    }
    free(out);
    return out[0];
}

double get_min(double *data, int step, int num){
    double *out;
    num = num/step;
    out = (double *)malloc(sizeof(double)*num);
    for(int i=0;i<num;i++){
        out[i] = data[i*step];
    }
    double temp =0;
    for(int i=0;i<num;i++){
        for(int j=0;j<i;j++){
            if(out[j] > out[i]){
                temp = out[i]; 
                out[i] = out[j];
                out[j] = temp;
            }
        }
    }
    free(out);
    return out[num-1];
}

int cross_corr(double *r,double *x,double *y,int num){
    double mx,my,sx,sy,sxy,denom;
    mx = 0;
    my = 0;   
    for (int i=0;i<num;i++) {
        mx += x[i];
        my += y[i];
    }
    mx /= num;
    my /= num;

    sx = 0;
    sy = 0;
    for (int i=0;i<num;i++) {
        sx += (x[i] - mx) * (x[i] - mx);
        sy += (y[i] - my) * (y[i] - my);
    }
    denom = sqrt(sx*sy);

    int maxdelay = num -1;
    int j=0;
    for(int delay = -maxdelay;delay < maxdelay; delay ++){
        sxy = 0.0;
        for(int i=0;i<num;i++){
            j=i+delay;
            if (j < 0 || j >= num)
                continue;
            else
                sxy += (x[i] - mx)*(y[j] -my);
        }
        r[delay + maxdelay] = sxy / denom;
    }
    return 1;
}

void hilbert_trans(double *in,fftw_complex *output,int num){
    int i=0;
    fftw_complex *out;
    out = fftw_alloc_complex(num);
    for(i=0;i<num;i++){
        out[i][RE] = in[i];
        out[i][IM] = 0.0;
    }
    fftw_plan plan;
    plan = fftw_plan_dft_1d(num,out,out,FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    int hN = num>> 1;
    int numRem = hN; 
    for(i=1;i<hN;i++){
        out[i][RE] *= 2;
        out[i][IM] *= 2;
    }

    if(N%2 == 0)
        numRem --;
    else if(num>1){
        out[hN][RE] *= 2;
        out[hN][IM] *= 2;
    }
    for(i=hN+1;i<num;i++){
        out[i][RE] = 0;
        out[i][IM] = 0;
    }

    plan = fftw_plan_dft_1d(num,out,out,FFTW_BACKWARD,FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    for(i=0;i<num;i++){
        output[i][IM] = out[i][IM]/num;
        output[i][RE] = out[i][RE]/num;
    }
    fftw_free(out);
}

int fft(fftw_complex *in, fftw_complex *out, int num){
    fftw_plan p; 
    p = fftw_plan_dft_1d(num,in,out,-1,FFTW_ESTIMATE); 
    fftw_execute(p);
    fftw_destroy_plan(p);
    return 1;
}

int ifft(fftw_complex *in, fftw_complex *out, int num){
    fftw_plan p; 
    p = fftw_plan_dft_1d(num,in,out,+1,FFTW_ESTIMATE); 
    fftw_execute(p);
    fftw_destroy_plan(p);
    for(int i=0;i<num;i++){
        out[i][RE] /= num;
        out[i][IM] /= num;
    }
    return 1;
}
