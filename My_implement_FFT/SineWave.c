#include "mex.h"
#include <math.h>
#include <string.h>

#define LENGTH ((double)2000)
#define Fs ((double)1000)
#ifndef PI
#define PI 3.14159265358979323846
#endif

void MATLABMain(int nlhs,mxArray* plhs[],int nrhs,const mxArray* prhs[])
{
	int i=0;
	char temppath[100];
	mxArray* temp[3];
	mxArray* lefttemp[3];
	mxArray* mydata[4];//Definitions
	
	memset(temp,0,sizeof(temp));
	memset(lefttemp,0,sizeof(lefttemp));
	memset(temppath,0,sizeof(temppath));
	memset(mydata,0,sizeof(mydata));//Initializations
	
	mexCallMATLAB(1,lefttemp,0,NULL,"cd");
	mxGetString(lefttemp[0],temppath,100);
	mxDestroyArray(lefttemp[0]);
	
	mydata[0]=mxCreateDoubleMatrix(1,LENGTH,mxREAL);
	mydata[1]=mxCreateDoubleMatrix(1,LENGTH,mxREAL);
	mydata[2]=mxCreateDoubleMatrix(1,LENGTH,mxREAL);
	mydata[3]=mxCreateDoubleMatrix(1,LENGTH,mxREAL);
	for(i=0;i<LENGTH;i++)
	{
		mxGetPr(mydata[0])[i]=1/Fs*i;//T=1/f
		mxGetPr(mydata[1])[i]=5.3*sin(2*PI*6*1/Fs*i)+6.7*sin(2*PI*10*1/Fs*i)+4.3*sin(2*PI*25*1/Fs*i)+0.7*sin(2*PI*50*1/Fs*i)+2.83*sin(2*PI*70*1/Fs*i)+7*sin(2*PI*100*1/Fs*i)+10;
		//Generate a discrete signal
	}
    
    mexCallMATLAB(1,&mydata[3],1,&mydata[1],"FFT_MATLAB_Call");
	for(i=0;i<LENGTH;i++)
	{
		//mxGetPr(mydata[3])[i]=abs(mxGetPr(mydata[3])[i]);
		mxGetPr(mydata[2])[i]=i/LENGTH*Fs;
		
		if(i>=(LENGTH/2)){
		mxGetPr(mydata[3])[i]=0;
		}
		//The fft result will be mirrored at half of Nyquist frequency.
		//Another half of frequency map will not be showed.
	}
    
	temp[0]=mxCreateDoubleScalar(2);
	temp[1]=mxCreateDoubleScalar(1);
	temp[2]=mxCreateDoubleScalar(1);
	mexCallMATLAB(0,NULL,3,temp,"subplot");
	mxDestroyArray(temp[2]);
	mxDestroyArray(temp[1]);
	mxDestroyArray(temp[0]);
	
	temp[0]=mydata[0];
	temp[1]=mydata[1];
	mexCallMATLAB(0,NULL,2,temp,"plot");
	temp[0]=NULL;
	temp[1]=NULL;

	temp[0]=mxCreateDoubleScalar(2);
	temp[1]=mxCreateDoubleScalar(1);
	temp[2]=mxCreateDoubleScalar(2);
	mexCallMATLAB(0,NULL,3,temp,"subplot");
	mxDestroyArray(temp[2]);
	mxDestroyArray(temp[1]);
	mxDestroyArray(temp[0]);

	temp[0]=mydata[2];
	temp[1]=mydata[3];
	mexCallMATLAB(0,NULL,2,temp,"plot");
	temp[0]=NULL;
	temp[1]=NULL;

	mxDestroyArray(mydata[0]);
	mxDestroyArray(mydata[1]);
	mxDestroyArray(mydata[2]);
	mxDestroyArray(mydata[3]);
	//Add your code here.

	/*CD to previous folder before exit*/
	temp[0]=mxCreateString(temppath);
	mexCallMATLAB(0,NULL,1,temp,"cd");
	mxDestroyArray(temp[0]);
}

void mexFunction(int nlhs,mxArray* plhs[],int nrhs,const mxArray* prhs[])
{
	return MATLABMain(nlhs,plhs,nrhs,prhs);
}
