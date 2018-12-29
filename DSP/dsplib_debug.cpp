#include "../stdafx.h"
#include "stdio.h"
#include "dsplib_debug.h"


BOOL WriteToFile(char *fname,double *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%f\n",pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(char *fname,CString *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%s\n",pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(CString path,CString *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,path,"at");
	else fopen_s(&fp,path,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%s\n",pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(CString fname,double *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%f\n",pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(CString fname,DWORD *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%d\n",(signed int)pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(CString fname,unsigned char *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%d\n",(unsigned int)pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(char *fname,float *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%f\n",pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(char *fname,int *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%d\n",pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(char *fname,short int *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%d\n",pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(char *fname,unsigned int *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%u\n",(unsigned int)pArray[i]);
	fclose(fp);
	return TRUE;
}

BOOL WriteToFile(char *fname,MKL_Complex16 *pArray,int size,BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<size;i++) fprintf_s(fp,"%lf%+lfi\n",pArray[i].real,pArray[i].imag);
	fclose(fp);
	return TRUE;
}

int ReadFromFile(char *fname,MKL_Complex16 *pArray)
{
	FILE *fp;
	int lineCnt=0,chCnt=0,i;
	char cbuff=0;
	char sbuff[70]="";
	BOOL sw=FALSE;

	memset(sbuff,'0',69);
	fopen_s(&fp,fname,"r");
	if (!fp) return -1;

	while(!feof(fp))
	{
		fscanf_s(fp,"%c",&cbuff);

		if (cbuff=='+')
		{
			if (!sw)
			{
				if (strlen(sbuff)>0)
				{
					sbuff[chCnt]='\0';
					pArray[lineCnt].real=atof(sbuff);
				}
				chCnt=0;
				memset(sbuff,'0',69);
			}
			else 
			{
				sbuff[chCnt]=cbuff;
				sw=FALSE;
				chCnt++;
			}
		}
		else if (cbuff=='-')
		{
			if (!sw)
			{
				if (strlen(sbuff)>0)
				{
					sbuff[chCnt]='\0';
					pArray[lineCnt].real=atof(sbuff);
				}
				chCnt=0;
				memset(sbuff,'0',69);
				sbuff[chCnt]=cbuff;
				sw=FALSE;
				chCnt++;
			}
			else 
			{
				sbuff[chCnt]=cbuff;
				sw=FALSE;
				chCnt++;
			}		
		}
		else if (cbuff=='i')
		{				
			if (strlen(sbuff)>0)
			{
				sbuff[chCnt]='\0';
				pArray[lineCnt].imag=atof(sbuff);
			}
			chCnt=0;
			lineCnt++;
			memset(sbuff,'0',69);
		}
		else
		{
			sbuff[chCnt]=cbuff;
			if (cbuff=='e') sw=TRUE;
			chCnt++;
		}
	}
	fclose(fp);
	return lineCnt;
}

int ReadFromFile(char *fname,double *pArray)
{
	FILE *fp;
	char cbuff=0;
	char sbuff[70]="";
	int lineCnt=0, chCnt=0;
	int sizeCounter=0;

	memset(sbuff,'0',69);

	fopen_s(&fp,fname,"r");
	if (!fp) return -1;

	while(!feof(fp)) 
	{
		fscanf_s(fp,"%c",&cbuff);

		if (cbuff != '\n')
		{
			sbuff[chCnt]=cbuff;
			chCnt++;
		}
		else
		{
			sbuff[chCnt]='\0';
			chCnt=0;
			pArray[lineCnt]=atof(sbuff);
			lineCnt++;
		}
		sizeCounter++;
	}
	fclose(fp);
	return lineCnt;
}

int ReadFromFileCount(char *fname,double *pArray,int len)
{
	FILE *fp;
	char cbuff=0;
	char sbuff[70]="";
	int lineCnt=0, chCnt=0;
	int sizeCounter=0;

	memset(sbuff,'0',69);

	fopen_s(&fp,fname,"r");
	if (!fp) return -1;

	while(!feof(fp)) 
	{
		if (sizeCounter>=len) break;
		fscanf_s(fp,"%c",&cbuff);

		if (cbuff != '\n')
		{
			sbuff[chCnt]=cbuff;
			chCnt++;
		}
		else
		{
			if (chCnt>0)
			{
				sbuff[chCnt]='\0';
				chCnt=0;
				pArray[lineCnt]=atof(sbuff);
				lineCnt++;
				sizeCounter++;
			}
		}
	}
	if (chCnt>0) 
	{
		pArray[lineCnt]=atof(sbuff);
		lineCnt++;
	}
	fclose(fp);
	return lineCnt;
}

BOOL WriteToFile(CString fname, double **pArray, int colsize, int rowsize, BOOL bAdd)
{
	FILE *fp;
	if(bAdd) fopen_s(&fp,fname,"at");
	else fopen_s(&fp,fname,"wt");
	if(!fp) return FALSE;
	for(int i=0;i<rowsize;i++) 
	{
		for (int j=0;j<colsize;j++)
		{
			fprintf_s(fp,"%f\t",pArray[i][j]);
		}
		fprintf_s(fp,"\n");
	}

	fclose(fp);
	return TRUE;
}