#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include "dsplib_general_func.h"

BOOL WriteToFile(char *fname,double *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(CString fname,double *pArray,int size,BOOL bAdd=FALSE);

BOOL WriteToFile(CString fname,DWORD *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(CString path,CString *pArray,int size,BOOL bAdd=FALSE);


BOOL WriteToFile(char *fname,float *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(char *fname,int *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(char *fname,short int *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(char *fname,MKL_Complex16 *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(char *fname,unsigned int *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(char *fname,CString *pArray,int size,BOOL bAdd=FALSE);
BOOL WriteToFile(CString fname,unsigned char *pArray,int size,BOOL bAdd=FALSE);

BOOL WriteToFile(CString fname, double **pArray, int colsize, int rowsize, BOOL bAdd=FALSE);


int ReadFromFile(char *fname,MKL_Complex16 *pArray);
int ReadFromFile(char *fname,double *pArray);
//int ReadFromFile(char *fname,double *pArray,int size=-1);
int ReadFromFileCount(char *fname,double *pArray,int len=0);
