/*************************************************************************
    > File Name: convol.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Tue 30 Jul 2019 05:13:39 PM CST
*************************************************************************/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<fftw3.h>


#define N 864
#define RE 0
#define IM 1

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

void convol(double* input1, double* input2, double * output, int mm, int nn)
{
    int max, min;
    if(mm > nn){
        max = mm;min = nn;
    }else{
        max = nn;min = mm;
    }
    double* xx = (double*)malloc(sizeof(double) * (mm + nn - 1));
    for (int i = 0; i < mm + nn - 1; i++)
    {
        xx[i] = 0;
        for (int j = 0; j < min ; j++) {
            if (mm <= nn) {
                if (i - j >= 0 && i - j < max ){
                    xx[i] += input1[j] * input2[i - j];
                }
            }
            else {
                if (i - j >= 0 && i - j < max){
                    xx[i] += input2[j] * input1[i - j];
                }
            }
        }
    }
    for (int i = 0; i < mm+nn-1; i++) {
        output[i] = 1.0 * xx[i];
    }
    free(xx);
}

void hilbert_trans(double *in,double *output,int num){
    int i=0;
    fftw_complex *out;
    out = fftw_alloc_complex(num);
    for(i=0;i<num;i++){
        out[i][RE] = in[i];
        out[i][IM] = 0.0;
    }
    /* fftw_plan plan; */
    /* plan = fftw_plan_dft_1d(num,out,out,FFTW_FORWARD,FFTW_ESTIMATE); */
    /* fftw_execute(plan); */
    /* fftw_destroy_plan(plan); */
    fft(out,out,num);
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

    /* plan = fftw_plan_dft_1d(num,out,out,FFTW_BACKWARD,FFTW_ESTIMATE); */
    /* fftw_execute(plan); */
    /* fftw_destroy_plan(plan); */
    ifft(out,out,num);

    for(i=0;i<num;i++){
        output[i] = out[i][IM];///num;
    }
    fftw_free(out);
}

int main(int argc, char **argv){
    double xx[N],yy[N],xx2[N],yy2[N],amp[N],phase[N];
    int i=0;
    double mpi = 1/M_PI;
    for(i=0;i<N;i++){
        xx[i] = sin(2*M_PI*i/N);
        xx2[i] = xx[i] + 5;
        yy[i] = 0.0;
        yy2[i] = 0.0;
    }

    hilbert_trans(xx,yy,N);
    hilbert_trans(xx2,yy2,N);



    for(i=0;i<N;i++){
        amp[i] = sqrt(xx[i]*xx[i] + yy[i]*yy[i] );
        phase[i] = sqrt(xx2[i]*xx2[i] + yy2[i]*yy2[i] );
        /* phase[i] = atan(yy[i]/xx[i]); */
    }

    for(i =0;i<N;i++){
        printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",i,xx[i],yy[i],amp[i]*cos(phase[i]),amp[i]*sin(phase[i]),amp[i],phase[i],yy2[i]-yy[i]);
    }
    return 0;
}
