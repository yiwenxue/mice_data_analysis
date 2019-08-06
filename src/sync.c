/*************************************************************************
    > File Name: sync.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Mon 29 Jul 2019 11:18:07 AM CST
*************************************************************************/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<fftw3.h>

#define N 1000
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

int convolution(fftw_complex *data1, int n1, fftw_complex *data2, int n2, fftw_complex *out){
    int num = n1 + n2 - 1;
    fftw_complex *fft1, *fft2, *fftw_temp;
    fft1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(n1+n2-1));
    fft2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(n1+n2-1));
    fftw_temp = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(n1+n2-1));

    /* padding array1 with data and zero */
    for(int i=0;i<n1;i++){
        fft1[i][RE] = data1[i][RE];
        fft1[i][IM] = data1[i][IM];
    }
    for(int i=n1;i<num;i++){
        fft1[i][RE] = 0.0;
        fft1[i][IM] = 0.0;
    }
    fft(fft1,fftw_temp,num);
    /* Copy the result into fft1 */
    for(int i=0;i<num;i++){
        fft1[i][RE] = fftw_temp[i][RE];
        fft1[i][IM] = fftw_temp[i][IM];
    }

    /* Padding array2 with data and zero */ 
    for(int i=0;i<n2;i++){
        fft2[i][RE] = data2[i][RE];
        fft2[i][IM] = data2[i][IM];
    }
    for(int i=n2;i<num;i++){
        fft1[i][RE] = 0.0;
        fft1[i][IM] = 0.0;
    }
    fft(fft2,fftw_temp,num);

    /* Copy the data back to fft2 */ 
    for(int i=0;i<num;i++){
        fft2[i][RE] = fftw_temp[i][RE];
        fft2[i][IM] = fftw_temp[i][IM];
    }
    /* Multiple fft1 with fft2 */ 
    for(int i=0;i<num;i++){
        fftw_temp[i][RE] = fft1[i][RE] * fft2[i][RE] - fft1[i][IM] * fft2[i][IM];
        fftw_temp[i][IM] = fft1[i][RE] * fft2[i][IM] + fft1[i][IM] * fft2[i][RE];
    }
    /* Then IFFT to get the final result */
    ifft(fftw_temp,out,num);
    fftw_free(fft1);fftw_free(fft2);fftw_free(fftw_temp);
    return 1;
}

int convol(double *a, double *c, double *b,int  n,int  m){
    for(int j=0;j<n+m-1;j++)
    {
        for(int k=0;k<=j;k++)
        {
            if((m>(m+k-j-1)>=0)&&(k<n))
            {
                c[j]+=a[k]*b[m+k-j-1];
            }
        }
    }
    return 0;
}

int hilbert_trans(double *data, double *out,int num, double time){
    double IN_PI = 1/M_PI;
    IN_PI /= time;
    fftw_complex *in_1,*in_2,*out_1;
    in_1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*num);
    in_2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*num);
    out_1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*num*2);

    for(int i=0;i<num;i++){
        in_1[i][RE] = data[i];
        in_1[i][IM] = 0.0;
        in_2[i][RE] = IN_PI / (double)i;
        in_2[i][IM] = 0.0;
        out_1[i][RE] = 0.0;
        out_1[i][IM] = 0.0;
    }
    convolution(in_1,num,in_2,num,out_1);
    for(int i=0;i<num;i++){
        out[i] = out_1[i][RE];
    }
    fftw_free(in_1);fftw_free(in_2);fftw_free(out_1);
    return 1;
}

int main(int argc, char **argv){
    int i=0;
    double omega = M_PI/100.0;
    double data[N],filt[N],output[N];

    fftw_complex * data1, *data2, *out, *dat1,*dat2;
    data1 = fftw_alloc_complex(N);
    data2 = fftw_alloc_complex(N);
    dat1 = fftw_alloc_complex(N);
    dat2 = fftw_alloc_complex(N);
    out = fftw_alloc_complex(N+N-1);
    for(i=0;i<N;i++){
        data1[i][RE] = cos(2*M_PI*i/16);
        data[i] = data1[i][RE];
        data1[i][IM] = 0.0;
    }
    for(i=0;i<N;i++){
        data2[i][RE] = 1.0;
        filt[i] = data2[i][RE];
        data2[i][IM] = 0.0;
        output[i] = 0.0;
    }

    printf("start\n");
    convol(data,output,filt,N,N);
    printf("end\n");
    for(i=0;i<N;i++){
        printf("%d\t%f\t%f\t%f\n",i,data[i],filt[i],output[i]);
    }

    for(i=0;i<N;i++){
        printf("%f,",data[i]);
    }
    printf("\n");
    for(i=0;i<N;i++){
        printf("%f,",filt[i]);
    }
    printf("\n");

    /* data2[0][RE] = 1.0; */

    /* for(int i=0;i<N;i++){ */
    /*     printf("%f,",data1[i][RE]); */
    /* } */
    /*     printf("\n\n\n"); */
    /* for(int i=0;i<N;i++){ */
    /*     printf("%f,",data2[i][RE]); */
    /* } */


    /* convolution(data1,N,data2,N,out); */

    /* FILE *stream; */
    /* stream = fopen("sync","w"); */
    /* for(i=0;i<2*N-1;i++){ */
        /* fprintf(stream,"%d\t%f\t%f\n",i,out[i][RE],out[i][IM]); */
    /* } */
    /* fclose(stream); */
    fftw_free(data1);fftw_free(data2);
    fftw_free(dat1);fftw_free(dat2);
    fftw_free(out);
    return 0;
}
