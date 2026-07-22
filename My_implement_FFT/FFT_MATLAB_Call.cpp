//Usage:MATLAB M file call FFT_MATLAB_Call(series),series is 1*N double array.
//Bluestein Algorithm for FFT
#include "mex.h"

//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


#define uint16_t    int
#define uint8_t     int
#define TRUE        1
#define FALSE       0
#define BOOL        short

#define ForTra      0x00//Forward transform
#define InvTra      0x01//Inverse transform
#ifndef PI
#define PI          3.14159265358979323846
#endif

struct complex_t
{
double Re;
double Im;
};

void nop()
{
	return;
}

double GetMag(complex_t data,uint16_t DataSize)
{
return 2*sqrt(data.Re*data.Re+data.Im*data.Im)/DataSize;
}

double GetPha(complex_t data)
{
return atan2(data.Im,data.Re);
}

void fftDataReverse(complex_t *fftArry,uint16_t DataSize,uint16_t *len)
{
    //uint16_t RevArry[DataSize];
	uint16_t* RevArry=(uint16_t*)malloc(DataSize*sizeof(uint16_t));
    complex_t Temp;
    uint16_t i=0;
    *len = 0;
    uint16_t temp_size = DataSize;
    while (temp_size >>= 1) (*len)++;
    
    RevArry[0] = 0;
    for (i = 1; i < DataSize; i++) {
        RevArry[i] = (RevArry[i>>1] >> 1) | ((i & 1) << (*len - 1));
    }
    
    for (i = 0; i < DataSize; i++) {
        if (i < RevArry[i]) {
            Temp = fftArry[i];
            fftArry[i] = fftArry[RevArry[i]];
            fftArry[RevArry[i]] = Temp;
        }
    }

free(RevArry);
}

complex_t MultiComplex(complex_t a,complex_t b)
{
complex_t temp;
temp.Re=a.Re*b.Re-a.Im*b.Im;
temp.Im=a.Re*b.Im+a.Im*b.Re;
return temp;
}

complex_t AddComplex(complex_t a,complex_t b)
{
a.Re+=b.Re;
a.Im+=b.Im;
return a;
}

complex_t SubComplex(complex_t a,complex_t b)
{
a.Re-=b.Re;
a.Im-=b.Im;
return a;
}

complex_t ConjComplex(complex_t a)
{
a.Im=-a.Im;
return a;
}

void fft(complex_t *fftArry,uint16_t DataSize,uint8_t TanMode)
{
    uint16_t len=0;
    fftDataReverse(fftArry,DataSize,&len);
    uint16_t i,j,k;
    for (i = 1; i <= len; i++)
	{
        uint16_t m = 1 << i;
        complex_t wn;
        wn.Re = cos(2*PI/m);
        wn.Im = (TanMode == ForTra) ? -sin(2*PI/m) : +sin(2*PI/m);
        
        for (j = 0; j < DataSize; j += m) {
            complex_t w = {1.0f, 0.0f};
            for (k = 0; k < m/2; k++) {
                complex_t Temp1 = MultiComplex(w, fftArry[j+k+m/2]);
                complex_t Temp2 = fftArry[j+k];
                
                fftArry[j+k] = AddComplex(Temp2, Temp1);
                fftArry[j+k+m/2] = SubComplex(Temp2, Temp1);
                
                w = MultiComplex(w, wn);
            }
        }
    }
    
    if (TanMode == InvTra) {
        for (i = 0; i < DataSize; i++) {
            fftArry[i].Re /= DataSize;
            fftArry[i].Im /= DataSize;
        }
    }
}

int IsPowerOfTwo(int N)
{
double t1=log(N)/log(2);
int t2=log(N)/log(2);
if(t1!=(double)t2)return FALSE;
return TRUE;
}

int NextPow2(int N)
{
double t1=log(N)/log(2);
int t2=log(N)/log(2);
if(t1!=(double)t2)return pow(2,t2+1);
return pow(2,t2);
}

void bluestein_fft(complex_t *x,int N,uint8_t mode)
{
int i=0;
int M=NextPow2(2*N-1);
double angle=0;
double cs=0;
double sn=0;

complex_t *a=(complex_t*)calloc(M,sizeof(complex_t));
complex_t *b=(complex_t*)calloc(M,sizeof(complex_t));
//complex_t *c=(complex_t*)calloc(M,sizeof(complex_t));

if(mode==InvTra)
{
for(i=0;i<N;i++)
x[i]=ConjComplex(x[i]);
}

for(i=0;i<N;i++)
{
angle=PI*i*i/N;

cs=cos(angle);
sn=sin(angle);

a[i].Re=x[i].Re*cs+x[i].Im*sn;
a[i].Im=x[i].Im*cs-x[i].Re*sn;
}//for

for(i=0;i<N;i++)
{
angle=PI*i*i/N;
b[i].Re=cos(angle);
b[i].Im=sin(angle);

if(i!=0)
b[M-i]=b[i];
}

fft(a,M,ForTra);
fft(b,M,ForTra);

for(i=0;i<M;i++)
{
a[i]=MultiComplex(a[i],b[i]);
}

fft(a,M,InvTra);

for(i=0;i<N;i++)
{
angle=PI*i*i/N;

cs=cos(angle);
sn=sin(angle);

x[i].Re=a[i].Re*cs+a[i].Im*sn;
x[i].Im=a[i].Im*cs-a[i].Re*sn;
}

if(mode==InvTra)
{
for(i=0;i<N;i++)
{
x[i]=ConjComplex(x[i]);
x[i].Re/=N;
x[i].Im/=N;
}
}

free(a);
free(b);
//free(c);
}

BOOL fft_any(complex_t *x,int N,uint8_t mode)
{
if(IsPowerOfTwo(N))
{
fft(x,N,mode);
return TRUE;
}else
{
bluestein_fft(x,N,mode);
return FALSE;
}
}

void MATLABMain(int nlhs,mxArray* plhs[],int nrhs,const mxArray* prhs[])
{
	mxArray* LeftParameter[3];
	mxArray* RightParameter[3];//Definitions
	int N=0;
	int i=0;
	complex_t* xdata=NULL;
	memset(LeftParameter,0,sizeof(LeftParameter));
	memset(RightParameter,0,sizeof(RightParameter));//Initializations

	if(nrhs!=1)
	{
	mexPrintf("Usage:FFT_MATLAB_Call(x)\nError!!\n");
	plhs[0]=mxCreateDoubleScalar(-100);
	nlhs=1;
	return;
	}

	N=mxGetNumberOfElements(prhs[0]);
	mexPrintf("data amount is %d.\n",N);
	mexPrintf("If need more angle information please check testFFT folder.\n");
	xdata=(complex_t*)malloc(N*sizeof(complex_t));
	
	for(i=0;i<N;i++)
	{
        xdata[i].Re=mxGetPr(prhs[0])[i];
        xdata[i].Im=0;
    }

	fft_any(xdata,N,ForTra);

	nlhs=1;
	plhs[0]=mxCreateDoubleMatrix(1,N,mxREAL);
	for(i=0;i<N;i++)
	{
		if(i==0)
		{
			mxGetPr(plhs[0])[i]=GetMag(xdata[i],N)/2;
			continue;
		}
		mxGetPr(plhs[0])[i]=GetMag(xdata[i],N);
	}
	free(xdata);
}

void mexFunction(int nlhs,mxArray* plhs[],int nrhs,const mxArray* prhs[])
{
	return MATLABMain(nlhs,plhs,nrhs,prhs);
}
