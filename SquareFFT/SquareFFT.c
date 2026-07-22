#include "mex.h"
#include <math.h>
#include <string.h>

#define LENGTH ((double)2000)
#define Fs ((double)1000)
#define SQUARE_FREQUENCY ((double)5)

void MATLABMain(int nlhs,mxArray* plhs[],int nrhs,const mxArray* prhs[])
{
	int i=0;
	int j=0;
	int k=0;
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

	j=0;
	k=pow(SQUARE_FREQUENCY,-1)/pow(Fs,-1)/2;
	for(i=0;i<LENGTH;i++)
	{
		mxGetPr(mydata[0])[i]=1/Fs*i;
		if(j<pow(SQUARE_FREQUENCY,-1)/pow(Fs,-1)/2)
		{
			mxGetPr(mydata[1])[i]=2;
			j++;
			if(j==pow(SQUARE_FREQUENCY,-1)/pow(Fs,-1)/2)goto LOW_VOLT;
		}

		if(k<pow(SQUARE_FREQUENCY,-1)/pow(Fs,-1)/2)
		{
			mxGetPr(mydata[1])[i]=-2;
			k++;
			if(k==pow(SQUARE_FREQUENCY,-1)/pow(Fs,-1)/2)goto HIGH_VOLT;
		}
		continue;
HIGH_VOLT:j=0;
		  k=pow(SQUARE_FREQUENCY,-1)/pow(Fs,-1)/2;
		  continue;
LOW_VOLT:k=0;
		 j=pow(SQUARE_FREQUENCY,-1)/pow(Fs,-1)/2;
		 continue;
	}//for
    
    mexCallMATLAB(1,&mydata[3],1,&mydata[1],"fft");
	for(i=0;i<LENGTH;i++)
	{
		mxGetPr(mydata[2])[i]=i/LENGTH*Fs;
		
		if(i>=(LENGTH/2))
		{
		mxGetPr(mydata[3])[i]=0;
		}
		//The fft result will be mirrored at half of Nyquist frequency.
		//Another half of frequency map will not be showed.
	}//for
    
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
