#include "stdafx.h"
#include "dsplib_demod_ssb_hop.h"

//1st fir coef=0.00011972
double __ssb_hop_demod_lpf_coef[507]={1,0.00018938,0.00030814,0.00044112,
0.00056656,0.0006544,0.00066916,0.00057498,0.00034201,-4.6675e-005,-0.00058904,
-0.00126,-0.0020108,-0.002772,-0.0034608,-0.0039918,-0.0042898,-0.004302,
-0.0040086,-0.0034288,-0.0026215,-0.0016784,-0.00071253,0.00015819,0.00082917,
0.0012256,0.001315,0.0011127,0.00067965,0.00011174,-0.00047629,-0.00097122,
-0.0012812,-0.0013518,-0.0011755,-0.00079217,-0.00028056,0.00025715,0.00071569,
0.0010073,0.0010789,0.00092233,0.00057566,0.00011484,-0.00036194,-0.00075454,
-0.00098161,-0.0009973,-0.00080068,-0.00043578,1.7934e-005,0.00046242,0.000802,
0.00096386,0.00091364,0.00066292,0.00026687,-0.00018762,-0.00060069,-0.00088122,
-0.00096686,-0.00083785,-0.00052171,-8.757e-005,0.00036879,0.00074579,0.00095861,
0.00095811,0.00074226,0.00035711,-0.00011286,-0.00056306,-0.00089196,-0.0010239,
-0.00092623,-0.00061771,-0.00016474,0.00033266,0.00076275,0.0010271,0.0010631,
0.00085854,0.00045542,-5.85e-005,-0.00056874,-0.00095943,-0.0011396,-0.0010641,
-0.00074502,-0.00024993,0.00031225,0.00081505,0.0011428,0.0012173,0.0010159,
0.00057856,-3.5907e-007,-0.00059215,-0.0010623,-0.0013008,-0.0012482,-0.00090969,
-0.00035585,0.00029203,0.00088838,0.001296,0.0014173,0.0012179,0.00073567,
7.3783e-005,-0.00062096,-0.0011907,-0.0015024,-0.0014784,-0.001116,-0.00048968,
0.0002641,0.00097624,0.0014829,0.0016633,0.0014677,0.00093158,0.00016869,
-0.00065226,-0.0013451,-0.001748,-0.0017613,-0.0013721,-0.00065878,0.00022387,
0.0010784,0.0017085,0.0019643,0.0017774,0.0011788,0.00029479,-0.00067995,
-0.0015247,-0.0020426,-0.0021065,-0.0016895,-0.00087418,0.00016336,0.0011915,
0.0019743,0.0023258,0.0021536,0.0014826,0.00045281,-0.00070967,-0.0017421,
-0.0024041,-0.0025336,-0.0020856,-0.0011465,8.2448e-005,0.0013277,0.0023042,
0.0027802,0.002632,0.0018755,0.00066631,-0.00073128,-0.0020021,-0.0028508,
-0.0030697,-0.0025897,-0.0014997,-3.1462e-005,0.0014894,0.002715,0.0033547,
0.003243,0.0023817,0.00094482,-0.00075579,-0.0023378,-0.0034335,-0.003775,
-0.0032587,-0.0019747,-0.00019228,0.0016953,0.0032574,0.0041231,0.0040688,
0.0030743,0.001335,-0.00077548,-0.0027847,-0.004226,-0.004746,-0.0041901,
-0.0026452,-0.00042913,0.0019736,0.0040162,0.0052131,0.0052537,0.0040806,
0.0019138,-0.00078881,-0.0034264,-0.0053867,-0.0061866,-0.0055884,-0.0036658,
-0.00080093,0.0023886,0.0051808,0.0069096,0.0071207,0.0056866,0.0028523,
-0.00080114,-0.004472,-0.0073099,-0.0086082,-0.0079737,-0.0054359,-0.0014648,
0.003109,0.0072621,0.0099996,0.010584,0.0087237,0.0046668,-0.00081212,-0.0065501,
-0.01123,-0.013667,-0.013087,-0.0093382,-0.0029822,0.0047668,0.012248,0.017678,
0.019533,0.016909,0.0097947,-0.0008167,-0.013009,-0.024211,-0.031623,-0.032716,
-0.025733,-0.010076,0.01348,0.042803,0.074668,0.10521,0.1305,0.1472,0.15303,0.1472,
0.1305,0.10521,0.074668,0.042803,0.01348,-0.010076,-0.025733,-0.032716,-0.031623,
-0.024211,-0.013009,-0.0008167,0.0097947,0.016909,0.019533,0.017678,0.012248,
0.0047668,-0.0029822,-0.0093382,-0.013087,-0.013667,-0.01123,-0.0065501,
-0.00081212,0.0046668,0.0087237,0.010584,0.0099996,0.0072621,0.003109,-0.0014648,
-0.0054359,-0.0079737,-0.0086082,-0.0073099,-0.004472,-0.00080114,0.0028523,
0.0056866,0.0071207,0.0069096,0.0051808,0.0023886,-0.00080093,-0.0036658,
-0.0055884,-0.0061866,-0.0053867,-0.0034264,-0.00078881,0.0019138,0.0040806,
0.0052537,0.0052131,0.0040162,0.0019736,-0.00042913,-0.0026452,-0.0041901,
-0.004746,-0.004226,-0.0027847,-0.00077548,0.001335,0.0030743,0.0040688,0.0041231,
0.0032574,0.0016953,-0.00019228,-0.0019747,-0.0032587,-0.003775,-0.0034335,
-0.0023378,-0.00075579,0.00094482,0.0023817,0.003243,0.0033547,0.002715,0.0014894,
-3.1462e-005,-0.0014997,-0.0025897,-0.0030697,-0.0028508,-0.0020021,-0.00073128,
0.00066631,0.0018755,0.002632,0.0027802,0.0023042,0.0013277,8.2448e-005,-0.0011465,
-0.0020856,-0.0025336,-0.0024041,-0.0017421,-0.00070967,0.00045281,0.0014826,
0.0021536,0.0023258,0.0019743,0.0011915,0.00016336,-0.00087418,-0.0016895,
-0.0021065,-0.0020426,-0.0015247,-0.00067995,0.00029479,0.0011788,0.0017774,
0.0019643,0.0017085,0.0010784,0.00022387,-0.00065878,-0.0013721,-0.0017613,
-0.001748,-0.0013451,-0.00065226,0.00016869,0.00093158,0.0014677,0.0016633,
0.0014829,0.00097624,0.0002641,-0.00048968,-0.001116,-0.0014784,-0.0015024,
-0.0011907,-0.00062096,7.3783e-005,0.00073567,0.0012179,0.0014173,0.001296,
0.00088838,0.00029203,-0.00035585,-0.00090969,-0.0012482,-0.0013008,-0.0010623,
-0.00059215,-3.5907e-007,0.00057856,0.0010159,0.0012173,0.0011428,0.00081505,
0.00031225,-0.00024993,-0.00074502,-0.0010641,-0.0011396,-0.00095943,-0.00056874,
-5.85e-005,0.00045542,0.00085854,0.0010631,0.0010271,0.00076275,0.00033266,
-0.00016474,-0.00061771,-0.00092623,-0.0010239,-0.00089196,-0.00056306,-0.00011286,
0.00035711,0.00074226,0.00095811,0.00095861,0.00074579,0.00036879,-8.757e-005,
-0.00052171,-0.00083785,-0.00096686,-0.00088122,-0.00060069,-0.00018762,0.00026687,
0.00066292,0.00091364,0.00096386,0.000802,0.00046242,1.7934e-005,-0.00043578,
-0.00080068,-0.0009973,-0.00098161,-0.00075454,-0.00036194,0.00011484,0.00057566,
0.00092233,0.0010789,0.0010073,0.00071569,0.00025715,-0.00028056,-0.00079217,
-0.0011755,-0.0013518,-0.0012812,-0.00097122,-0.00047629,0.00011174,0.00067965,
0.0011127,0.001315,0.0012256,0.00082917,0.00015819,-0.00071253,-0.0016784,
-0.0026215,-0.0034288,-0.0040086,-0.004302,-0.0042898,-0.0039918,-0.0034608,
-0.002772,-0.0020108,-0.00126,-0.00058904,-4.6675e-005,0.00034201,0.00057498,
0.00066916,0.0006544,0.00056656,0.00044112,0.00030814,0.00018938,0.00011972
};

double __ssb_hop_demod_hilbert_coef[501]={
	2.4221e-014,-0.0039205,-9.9097e-015,-0.00039516,-9.9645e-016,-0.00041465,
	1.9346e-015,-0.0004346,5.131e-015,-0.00045512,9.4201e-015,-0.00047632,
	-5.5834e-015,-0.00049843,-3.5937e-015,-0.00052153,-1.2095e-015,-0.00054565,
	6.8298e-015,-0.00057062,-8.7251e-015,-0.00059611,1.6132e-014,-0.00062165,
	-1.026e-014,-0.00064677,1.1115e-014,-0.00067178,-1.8956e-014,-0.00069904,
	1.775e-014,-0.00073538,-3.578e-015,-0.00075942,3.9304e-015,-0.00079142,
	6.4331e-015,-0.0008229,-1.5235e-015,-0.00085511,1.9335e-015,-0.00088798,
	-1.0017e-015,-0.00092158,-3.2693e-015,-0.000956,9.8892e-015,-0.00099145,
	-5.4585e-015,-0.0010281,1.1726e-014,-0.0010659,-6.7339e-015,-0.0011048,
	1.423e-014,-0.0011441,-1.0227e-014,-0.0011838,6.2011e-015,-0.001225,1.669e-015,
	-0.0012694,-1.2087e-014,-0.0013116,1.6201e-014,-0.001357,-6.5739e-015,
	-0.0014031,1.0147e-014,-0.0014505,-5.3478e-015,-0.0014992,9.7403e-016,
	-0.0015491,2.3345e-015,-0.0016001,5.8258e-015,-0.0016524,-7.6997e-016,
	-0.001706,3.7291e-015,-0.0017612,1.9112e-015,-0.001818,2.609e-015,
	-0.001876,8.2001e-016,-0.0019351,1.0854e-014,-0.001996,-5.694e-015,
	-0.0020593,5.8426e-015,-0.0021227,-2.6062e-015,-0.0021887,4.1105e-015,
	-0.0022561,2.8788e-015,-0.0023253,2.3443e-015,-0.0023964,9.6159e-016,
	-0.0024695,5.5514e-015,-0.0025445,1.1072e-015,-0.0026215,-2.512e-015,
	-0.0027005,4.9437e-015,-0.0027818,1.4755e-015,-0.0028655,1.9847e-015,
	-0.0029513,2.9447e-015,-0.0030394,3.269e-015,-0.0031302,7.4208e-015,
	-0.0032241,-8.305e-015,-0.0033199,-1.6655e-015,-0.0034192,1.8777e-015,
	-0.0035211,1.7217e-015,-0.0036261,3.6701e-015,-0.0037343,-2.0695e-015,
	-0.0038459,6.823e-015,-0.003961,-4.0107e-015,-0.0040798,3.5122e-015,
	-0.0042023,-6.7078e-016,-0.0043289,3.186e-015,-0.0044599,2.0525e-015,
	-0.0045953,3.4261e-015,-0.0047351,1.7761e-015,-0.0048801,-3.4913e-015,
	-0.0050305,3.3652e-015,-0.0051861,1.7931e-016,-0.0053479,-2.1597e-015,
	-0.0055157,5.4948e-015,-0.0056902,3.9043e-015,-0.0058717,3.4479e-015,
	-0.0060608,-8.245e-015,-0.0062578,3.4877e-015,-0.0064634,3.4756e-015,
	-0.0066781,2.1448e-015,-0.0069027,1.7351e-015,-0.007138,2.4306e-015,
	-0.0073846,-1.9773e-015,-0.0076435,4.0449e-015,-0.0079157,2.0432e-015,
	-0.0082026,-9.0604e-016,-0.0085049,2.0181e-015,-0.0088247,-2.4885e-016,
	-0.0091631,3.8483e-015,-0.0095221,-1.1169e-015,-0.0099038,2.235e-015,
	-0.010311,9.3145e-016,-0.010745,-1.1928e-015,-0.01121,1.7722e-015,-0.01171,
	1.4438e-015,-0.012249,5.3859e-015,-0.012831,-4.1968e-015,-0.013462,1.4694e-015,
	-0.014149,8.4229e-016,-0.014901,7.9522e-016,-0.015727,6.0974e-017,-0.016639,
	1.8803e-015,-0.017652,-4.1019e-016,-0.018785,2.1195e-015,-0.02006,-8.6976e-016,
	-0.021506,2.5492e-015,-0.023163,-7.3041e-016,-0.025079,-9.6694e-016,-0.027324,
	2.3228e-015,-0.029991,2.1199e-016,-0.033213,5.7376e-016,-0.037185,-1.1388e-015,
	-0.042209,3.5237e-016,-0.048769,5.6529e-016,-0.057704,4.5063e-016,-0.070596,
	2.0717e-016,-0.090837,2.0538e-016,-0.12725,-2.0959e-016,-0.21216,-8.353e-017,
	-0.6366,0,0.6366,8.353e-017,0.21216,2.0959e-016,0.12725,-2.0538e-016,0.090837,
	-2.0717e-016,0.070596,-4.5063e-016,0.057704,-5.6529e-016,0.048769,-3.5237e-016,
	0.042209,1.1388e-015,0.037185,-5.7376e-016,0.033213,-2.1199e-016,0.029991,
	-2.3228e-015,0.027324,9.6694e-016,0.025079,7.3041e-016,0.023163,-2.5492e-015,
	0.021506,8.6976e-016,0.02006,-2.1195e-015,0.018785,4.1019e-016,0.017652,
	-1.8803e-015,0.016639,-6.0974e-017,0.015727,-7.9522e-016,0.014901,-8.4229e-016,
	0.014149,-1.4694e-015,0.013462,4.1968e-015,0.012831,-5.3859e-015,0.012249,
	-1.4438e-015,0.01171,-1.7722e-015,0.01121,1.1928e-015,0.010745,-9.3145e-016,
	0.010311,-2.235e-015,0.0099038,1.1169e-015,0.0095221,-3.8483e-015,0.0091631,
	2.4885e-016,0.0088247,-2.0181e-015,0.0085049,9.0604e-016,0.0082026,
	-2.0432e-015,0.0079157,-4.0449e-015,0.0076435,1.9773e-015,0.0073846,
	-2.4306e-015,0.007138,-1.7351e-015,0.0069027,-2.1448e-015,0.0066781,
	-3.4756e-015,0.0064634,-3.4877e-015,0.0062578,8.245e-015,0.0060608,
	-3.4479e-015,0.0058717,-3.9043e-015,0.0056902,-5.4948e-015,0.0055157,
	2.1597e-015,0.0053479,-1.7931e-016,0.0051861,-3.3652e-015,0.0050305,
	3.4913e-015,0.0048801,-1.7761e-015,0.0047351,-3.4261e-015,0.0045953,
	-2.0525e-015,0.0044599,-3.186e-015,0.0043289,6.7078e-016,0.0042023,
	-3.5122e-015,0.0040798,4.0107e-015,0.003961,-6.823e-015,0.0038459,2.0695e-015,
	0.0037343,-3.6701e-015,0.0036261,-1.7217e-015,0.0035211,-1.8777e-015,
	0.0034192,1.6655e-015,0.0033199,8.305e-015,0.0032241,-7.4208e-015,0.0031302,
	-3.269e-015,0.0030394,-2.9447e-015,0.0029513,-1.9847e-015,0.0028655,
	-1.4755e-015,0.0027818,-4.9437e-015,0.0027005,2.512e-015,0.0026215,
	-1.1072e-015,0.0025445,-5.5514e-015,0.0024695,-9.6159e-016,0.0023964,
	-2.3443e-015,0.0023253,-2.8788e-015,0.0022561,-4.1105e-015,0.0021887,
	2.6062e-015,0.0021227,-5.8426e-015,0.0020593,5.694e-015,0.001996,
	-1.0854e-014,0.0019351,-8.2001e-016,0.001876,-2.609e-015,0.001818,
	-1.9112e-015,0.0017612,-3.7291e-015,0.001706,7.6997e-016,0.0016524,
	-5.8258e-015,0.0016001,-2.3345e-015,0.0015491,-9.7403e-016,0.0014992,
	5.3478e-015,0.0014505,-1.0147e-014,0.0014031,6.5739e-015,0.001357,
	-1.6201e-014,0.0013116,1.2087e-014,0.0012694,-1.669e-015,0.001225,
	-6.2011e-015,0.0011838,1.0227e-014,0.0011441,-1.423e-014,0.0011048,
	6.7339e-015,0.0010659,-1.1726e-014,0.0010281,5.4585e-015,0.00099145,
	-9.8892e-015,0.000956,3.2693e-015,0.00092158,1.0017e-015,0.00088798,
	-1.9335e-015,0.00085511,1.5235e-015,0.0008229,-6.4331e-015,0.00079142,
	-3.9304e-015,0.00075942,3.578e-015,0.00073538,-1.775e-014,0.00069904,
	1.8956e-014,0.00067178,-1.1115e-014,0.00064677,1.026e-014,0.00062165,
	-1.6132e-014,0.00059611,8.7251e-015,0.00057062,-6.8298e-015,0.00054565,
	1.2095e-015,0.00052153,3.5937e-015,0.00049843,5.5834e-015,0.00047632,
	-9.4201e-015,0.00045512,-5.131e-015,0.0004346,-1.9346e-015,0.00041465,
	9.9645e-016,0.00039516,9.9097e-015,0.0039205,-2.4221e-014
};

double __ssb_hop_demod_hop_lpf_audio[115] = {
	-0.0065474,
	-0.0016722,
	-0.0015609,
	-0.0011995,
	-0.0005765,
	0.00028955,
	0.0013557,
	0.0025516,
	0.0037847,
	0.0049451,
	0.0059193,
	0.0065952,
	0.006879,
	0.0067032,
	0.00604,
	0.0049034,
	0.0033567,
	0.001507,
	-0.00049719,
	-0.002481,
	-0.0042536,
	-0.0056301,
	-0.0064462,
	-0.0065815,
	-0.0059712,
	-0.0046224,
	-0.0026142,
	-9.9509e-005,
	0.0027064,
	0.0055363,
	0.0080957,
	0.010088,
	0.011245,
	0.01136,
	0.010309,
	0.0080624,
	0.0047065,
	0.00046877,
	-0.004343,
	-0.0093086,
	-0.013942,
	-0.017735,
	-0.020182,
	-0.020834,
	-0.019331,
	-0.015451,
	-0.0091161,
	-0.00043234,
	0.010324,
	0.02271,
	0.036143,
	0.049932,
	0.063329,
	0.075573,
	0.085953,
	0.093851,
	0.098793,
	0.10047,
	0.098793,
	0.093851,
	0.085953,
	0.075573,
	0.063329,
	0.049932,
	0.036143,
	0.02271,
	0.010324,
	-0.00043234,
	-0.0091161,
	-0.015451,
	-0.019331,
	-0.020834,
	-0.020182,
	-0.017735,
	-0.013942,
	-0.0093086,
	-0.004343,
	0.00046877,
	0.0047065,
	0.0080624,
	0.010309,
	0.01136,
	0.011245,
	0.010088,
	0.0080957,
	0.0055363,
	0.0027064,
	-9.9509e-005,
	-0.0026142,
	-0.0046224,
	-0.0059712,
	-0.0065815,
	-0.0064462,
	-0.0056301,
	-0.0042536,
	-0.002481,
	-0.00049719,
	0.001507,
	0.0033567,
	0.0049034,
	0.00604,
	0.0067032,
	0.006879,
	0.0065952,
	0.0059193,
	0.0049451,
	0.0037847,
	0.0025516,
	0.0013557,
	0.00028955,
	-0.0005765,
	-0.0011995,
	-0.0015609,
	-0.0016722,
	-0.0065474
};
///////////////////////////////////////////////////////////
namespace NS_DSP
{

CSSBHopvmld::CSSBHopvmld()
{
	m_nModType = DSPLIB_LSB_DEMOD;
	m_pDescFFT = NULL;
	m_pDescFFTHann = NULL;
	m_pDescIFFTHann = NULL;
	m_nSizeFFT = 0;
	m_fPeakFinderBuffer = NULL;
	m_cPeakFinderOut = NULL;
	m_fFs = 40e3;
	m_fMultSin = NULL;
	m_fMultCos = NULL;
	m_timeDDS = 0;
	m_iHopVectorStart=0;
	m_iHopVectorStartPos=0;
	m_iHopVectorEnd=0;
	m_iHopVectorEndPos=0;
	m_fPeakFinderFc=0;
	m_iHopNumberSamples=0;
	m_fZeroMemory = NULL;
	m_iDistance=0;
	hanning(_DSPLIB_DELTA_FREQ_L1,m_fHannWindow);//hanning window
	memset(m_fMultHann,0,_DSPLIB_DELTA_FREQ_L2*sizeof(double));
	memset(m_cMultHannLn,0,_DSPLIB_DELTA_FREQ_L2*sizeof(MKL_Complex16));
	m_timeDemodDDS = 1;//initial phase
	m_iHopSamplesCntr=0;
	m_bF0=TRUE;
	m_iThreshold=8;

}

CSSBHopvmld::~CSSBHopvmld()
{
	Release();
}

void CSSBHopvmld::Reset()
{
	m_firLPF_I.Reset();
	m_firLPF_Q.Reset();
	m_fir_Demod.Reset();
//	m_hilbert.Reset();
	m_fir_hilbert.Reset();
	m_timeDDS = 0;
	m_iHopVectorStart=0;
	m_iHopVectorStartPos=0;
	m_iHopVectorEnd=0;
	m_iHopVectorEndPos=0;
	m_fPeakFinderFc=0;
	m_iHopNumberSamples=0;
	m_iDistance=0;
	m_timeDemodDDS = 1;//initial phase
	m_iHopSamplesCntr=0;
	m_fFineTune=0;
}

void CSSBHopvmld::Release()
{
	if(m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
	m_pDescFFT = NULL;
	if(m_pDescFFTHann )DftiFreeDescriptor(&m_pDescFFTHann);
	m_pDescFFTHann =NULL;
	if(m_pDescIFFTHann )DftiFreeDescriptor(&m_pDescIFFTHann);
	m_pDescIFFTHann =NULL;

	if(m_fPeakFinderBuffer) delete[] m_fPeakFinderBuffer;
	m_fPeakFinderBuffer = NULL;
	if(m_cPeakFinderOut) delete[] m_cPeakFinderOut;
	m_cPeakFinderOut = NULL;
	if(m_fMultSin ) delete[] m_fMultSin ;
	m_fMultSin = NULL;
	if(m_fMultCos ) delete[] m_fMultCos ;
	m_fMultCos = NULL;
	if(m_fZeroMemory) delete[] m_fZeroMemory;
	m_fZeroMemory = NULL;

	for(size_t i=0;i<m_vSamplesVector.size();i++)
	{//delete vector
		delete m_vSamplesVector[i];
	}
	m_vSamplesVector.clear();

	m_firLPF_I.Release();
	m_firLPF_Q.Release();
//	m_hilbert.Release();
	m_fir_Demod.Release();
	m_fir_hilbert.Release();

#ifdef _DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC
	fclose(fp_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC);
#endif
#ifdef _DSPLIB_DEMOD_SSB_SAVE_DEVIATION
	fclose(fp_DSPLIB_DEMOD_SSB_SAVE_DEVIATION);
#endif
}

BOOL CSSBHopvmld::Create(int nSizeFFT,int modType,double fs,int iThreshold,BOOL bF0)
{
	BOOL ret;
	m_bF0=bF0;
	m_iThreshold=iThreshold;
	m_nSizeFFT = nSizeFFT;
	m_nModType = modType;
	m_fFs = fs;
	//Init FFT Struct----------------------
	long Status = DftiCreateDescriptor( &m_pDescFFT, DFTI_DOUBLE,DFTI_REAL, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}
	//   Set placement of result DFTI_NOT_INPLACE
	Status = DftiSetValue( m_pDescFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}
	double Scale = 1.0/(double)nSizeFFT;

	Status = DftiSetValue(m_pDescFFT, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}

	//   Commit Dfti descriptor
	Status = DftiCommitDescriptor( m_pDescFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}

	//Init FFT Hann Struct----------------------
	Status = DftiCreateDescriptor( &m_pDescFFTHann, 
		DFTI_DOUBLE,DFTI_REAL, 1, _DSPLIB_DELTA_FREQ_L2 );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}
	//   Set placement of result DFTI_NOT_INPLACE
	Status = DftiSetValue( m_pDescFFTHann, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}
	Scale = 1.0/(double)_DSPLIB_DELTA_FREQ_L2;

	Status = DftiSetValue(m_pDescFFTHann, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}

	//   Commit Dfti descriptor
	Status = DftiCommitDescriptor( m_pDescFFTHann );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}

	//Init IFFT Hann Struct----------------------complex
	Status = DftiCreateDescriptor( &m_pDescIFFTHann, 
		DFTI_DOUBLE,DFTI_COMPLEX, 1, _DSPLIB_DELTA_FREQ_L2 );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}
	//   Set placement of result DFTI_NOT_INPLACE
	Status = DftiSetValue( m_pDescIFFTHann, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}
	Scale = 1.0/(double)_DSPLIB_DELTA_FREQ_L2;

	Status = DftiSetValue(m_pDescIFFTHann, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}

	//   Commit Dfti descriptor
	Status = DftiCommitDescriptor( m_pDescIFFTHann );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		return DSP_RET_FFT_INIT_ERROR;
	}

	//-------------------------------------
	//----------LPFs------------------------
	ret = m_firLPF_I.Create(8192,1,__ssb_hop_demod_lpf_coef);
	if(ret != DSP_RET_SUCCESS) return ret;
	ret = m_firLPF_Q.Create(8192,1,__ssb_hop_demod_lpf_coef);
	if(ret != DSP_RET_SUCCESS) return ret;
	__ssb_hop_demod_lpf_coef[0] = 0.00011972;
	ret = m_fir_Demod.Create(8192,115,__ssb_hop_demod_hop_lpf_audio);//__ssb_hop_demod_lpf_coef);
	if(ret != DSP_RET_SUCCESS) return ret;
	//--------Hilberts----------------------
	ret = m_fir_hilbert.Create(8192,501,__ssb_hop_demod_hilbert_coef);
	if(ret != DSP_RET_SUCCESS) return ret;
//	ret = m_hilbert.Create(4000);
//	if(ret != DSP_RET_SUCCESS) return ret;
	//---------------------------------------

	m_fPeakFinderBuffer = new double[m_nSizeFFT];
	if(!m_fPeakFinderBuffer) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cPeakFinderOut = new MKL_Complex16[m_nSizeFFT];
	if(!m_cPeakFinderOut) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fMultSin = new double[nSizeFFT ];
	if(!m_fMultSin) {
		Release();
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	m_fMultCos = new double[nSizeFFT ];
	if(!m_fMultCos) {
		Release();
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	m_fZeroMemory= new double[nSizeFFT ];
	if(!m_fZeroMemory) {
		Release();
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	memset(m_fZeroMemory,0,nSizeFFT*sizeof(double));//filled with 0


#ifdef _DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC
	fp_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC = fopen(_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC_FILE,"wt");
	if(!fp_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC){ 
		printf("cannot open file : %s\n",_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC_FILE);
	}
#endif
#ifdef _DSPLIB_DEMOD_SSB_SAVE_DEVIATION
	fp_DSPLIB_DEMOD_SSB_SAVE_DEVIATION =fopen(_DSPLIB_DEMOD_SSB_SAVE_DEVIATION_FILE,"wt");
	if(!fp_DSPLIB_DEMOD_SSB_SAVE_DEVIATION){ 
		printf("cannot open file : %s\n",_DSPLIB_DEMOD_SSB_SAVE_DEVIATION_FILE);
	}
#endif
	Reset();
	return DSP_RET_SUCCESS;
}

//peak finder : find  max index in fft(m_fSample,8192)
//m_fSamples = from vector[m_iVectorStart][m_iVectorStartPos] to vector[m_iVectorEnd][m_iVectorEndPos]
BOOL CSSBHopvmld::PeakFinder(int m_iVectorStart,
							 int m_iVectorStartPos,
							 int m_iVectorEnd,
							 int m_iVectorEndPos,
							 double &m_fFout
							 )
{
	int i, m_iPeakBufferIndex=0 , nLen;
	double *pPointer;
	m_fFout = 0;
	//peek first m_nSizeFFT samples
	memset(m_fPeakFinderBuffer,0,m_nSizeFFT *sizeof(double));
	//create a contiguous buffer from vectors
	for (i=m_iVectorStart;i<=m_iVectorEnd;i++)
	{
		pPointer = (m_vSamplesVector[i]->m_fSamples);//read pointer
		if(i == m_iVectorStart)
		{//first vector index
			if(i== m_iVectorEnd)
			{//if it is also last index
				nLen = (m_iVectorEndPos - m_iVectorStartPos + 1);
				nLen = min(nLen , m_nSizeFFT - m_iPeakBufferIndex);
				memcpy(m_fPeakFinderBuffer+m_iPeakBufferIndex ,pPointer+m_iVectorStartPos,
					nLen *sizeof(double));  
			}else{//if it is not last index
				nLen = (m_vSamplesVector[i]->m_nSize - m_iVectorStartPos);
				nLen = min(nLen , m_nSizeFFT - m_iPeakBufferIndex);
				memcpy(m_fPeakFinderBuffer + m_iPeakBufferIndex ,pPointer+m_iVectorStartPos,
					nLen *sizeof(double));
			}
		}else if(i==m_iVectorEnd)
		{//last vector index
			nLen = ( m_iVectorEndPos + 1);
			nLen = min(nLen , m_nSizeFFT - m_iPeakBufferIndex);
			memcpy(m_fPeakFinderBuffer + m_iPeakBufferIndex ,pPointer,
				nLen *sizeof(double));
		}else{//not first nor last vector index
			nLen = (m_vSamplesVector[i]->m_nSize);
			nLen = min(nLen , m_nSizeFFT - m_iPeakBufferIndex);
			memcpy(m_fPeakFinderBuffer + m_iPeakBufferIndex ,pPointer,
				nLen *sizeof(double));
		}//if
		m_iPeakBufferIndex += nLen;
	}//for

#ifdef _DSPLIB_DEMOD_SSB_SAVE_FFT_PEAK_IN
	FILE *fpp = fopen(_DSPLIB_DEMOD_SSB_SAVE_FFT_PEAK_IN_FILE,"wt");
	if(!fpp){ 
		printf("cannot open file : %s\n",_DSPLIB_DEMOD_SSB_SAVE_FFT_PEAK_IN_FILE);
	}else{
		int u;
		for(u=0;u<m_nSizeFFT/2;u++)
			fprintf(fpp,"%f\n",m_fPeakFinderBuffer[u]);
		fclose(fpp);
	}
#endif
	//m_fPeakFinderBuffer = abs(fft(m_fSamples,m_nSizeFFT))
	long Status = DftiComputeForward( m_pDescFFT, m_fPeakFinderBuffer, m_cPeakFinderOut);
	vzAbs(m_nSizeFFT/2,m_cPeakFinderOut,m_fPeakFinderBuffer);
	int idx = cblas_idamax(m_nSizeFFT/2,m_fPeakFinderBuffer,1);
	m_fFout = (idx+1) * m_fFs / (double)m_nSizeFFT;
	return DSP_RET_SUCCESS;
}//func




BOOL CSSBHopvmld::Demodulate(double *m_fInputData,int *m_bInputHopsPosition,
							 int nInputSize,std::vector<__SSB_DOUBLE_VECTOR *> *m_vOutput,
				int &nOutputSize)
{
	if (nInputSize < 1) return DSP_RET_SUCCESS;
	int ret;
	double m_fDeltaFreq;
	double 	*m_fFcCorrection1 = NULL;
	double 	*m_fFcCorrection2 = NULL;
	int m_nSizeFcCorrection=0;
	double *m_fOutputCopy=NULL;
	nOutputSize=0;
	int nDDCSizeOut;
	__SSB_DOUBLE_VECTOR *m_sOutVector;
	m_vOutput->clear();

	__SSB_DOUBLE_VECTOR *pVector = new __SSB_DOUBLE_VECTOR;
	if(!pVector) {
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	pVector->m_fSamples = new double[nInputSize];
	pVector->m_iHops = new int[nInputSize];
	if(!pVector->m_fSamples || !pVector->m_iHops)
	{
		delete[] pVector;
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	memcpy(pVector->m_iHops,m_bInputHopsPosition,nInputSize * sizeof(int));
	memcpy(pVector->m_fSamples,m_fInputData,nInputSize * sizeof(double));
	pVector->m_nSize = nInputSize;
	m_vSamplesVector.push_back(pVector);
	int nextHopIdx;
	int nHopIdx=0;
	nextHopIdx = GetNextHopPosition(pVector->m_iHops,nInputSize,nHopIdx);
	if(nextHopIdx == -1)
	{//if hop position not found
		m_iHopVectorEnd++;
	}else{//if at least one hop position has been found
		while (nextHopIdx != -1)
		{//a hop has been found
			m_iHopVectorEndPos = nextHopIdx;
			//check distance
			if(m_iDistance <= 1000) {
				//[4]-Go on search for hop position
				nextHopIdx = GetNextHopPosition(pVector->m_iHops,nInputSize,nextHopIdx+1);		
				continue;
			}
			//printf("D=%d\n",m_iDistance);
			m_iDistance=0;

			//do DDC & Fc correction on 
			//[1]-{vector[0][m_nLastHopBufferIndex] to vector[current][nHopIdx]}
			ret = PeakFinder(m_iHopVectorStart,m_iHopVectorStartPos,m_iHopVectorEnd,
				m_iHopVectorEndPos,m_fPeakFinderFc);
			if(ret != DSP_RET_SUCCESS) return ret;

#ifdef _DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC
	if(fp_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC)
		fprintf(fp_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC,"%f\n",m_fPeakFinderFc);
#endif

		//[2]-correcting Fc
			ret = DDC(m_iHopVectorStart,m_iHopVectorStartPos,m_iHopVectorEnd,
				m_iHopVectorEndPos,m_fPeakFinderFc,
				&m_fFcCorrection1,&m_fFcCorrection2,m_nSizeFcCorrection
				);
			if(ret != DSP_RET_SUCCESS) return ret;		
			if(m_nModType == DSPLIB_LSB_DEMOD )
				ret = Delta_Freq_Estimator(m_fFcCorrection1,m_nSizeFcCorrection,m_fDeltaFreq);
			else
				ret = Delta_Freq_Estimator(m_fFcCorrection2,m_nSizeFcCorrection,m_fDeltaFreq);
			if(ret != DSP_RET_SUCCESS) return ret;
			if(m_nModType == DSPLIB_USB_DEMOD ) m_fDeltaFreq=-m_fDeltaFreq;
#ifdef _DSPLIB_DEMOD_SSB_SAVE_DEVIATION
			fprintf(fp_DSPLIB_DEMOD_SSB_SAVE_DEVIATION,"%f\n", m_fDeltaFreq);
#endif

			//[3]-SSB demodulation by corrected Fc
			m_sOutVector= new __SSB_DOUBLE_VECTOR;
			if(!m_sOutVector)
			{
				nOutputSize = 0;
				return DSP_RET_MEMORY_ALLOCATE_ERROR;
			}
			m_sOutVector->m_fSamples = new double[m_nSizeFcCorrection+2e4];
			if(!m_sOutVector->m_fSamples )
			{
				nOutputSize = 0;
				delete m_sOutVector;
				return DSP_RET_MEMORY_ALLOCATE_ERROR;
			}

			ret = DemodDDC(m_iHopVectorStart,m_iHopVectorStartPos,m_iHopVectorEnd,
				m_iHopVectorEndPos,m_fPeakFinderFc+m_fDeltaFreq+m_fFineTune,
				m_sOutVector->m_fSamples,nDDCSizeOut);
			if(ret != DSP_RET_SUCCESS)
			{
				delete m_sOutVector ;  // free original block
				nOutputSize = 0;
				return DSP_RET_MEMORY_ALLOCATE_ERROR;
			}
			m_sOutVector->m_nSize = nDDCSizeOut;
			m_vOutput->push_back(m_sOutVector);
			nOutputSize += nDDCSizeOut;

			//[4]-Go on search for hop position
			nextHopIdx = GetNextHopPosition(pVector->m_iHops,nInputSize,nextHopIdx+1);

			//[5]-delete used vectors and shift pointer
			FreeUsedVectors();
			if(m_nSizeFcCorrection>0){
				delete[] m_fFcCorrection1;
				delete[] m_fFcCorrection2;
			}
		}//while
		m_iHopVectorEnd++;
	}//if
	return DSP_RET_SUCCESS;
}

//frees used vectors and shift their pointers
void CSSBHopvmld::FreeUsedVectors()
{
	int i;
	for(i=m_iHopVectorStart;i<m_iHopVectorEnd;i++)
	{//delete vector
		delete m_vSamplesVector[i];
	}
	m_vSamplesVector.erase(m_vSamplesVector.begin()+m_iHopVectorStart,
		m_vSamplesVector.begin()+m_iHopVectorEnd);//delete vector[start] to vector[end-1]

	if (m_iHopVectorEndPos < m_vSamplesVector[0]->m_nSize - 1)
	{//if last element of vector has samples that must be processed
		m_iHopVectorStartPos = m_iHopVectorEndPos;//shift pointers
		m_iHopVectorEnd=0;
		m_iHopVectorEndPos=0;
	}else{//if last element of vector has not samples that must be processed
		//in this case last vector can be deleted
		delete m_vSamplesVector[0];
		m_vSamplesVector.erase(m_vSamplesVector.begin()+0);
		m_iHopVectorEnd=-1;
		m_iHopVectorEndPos=0;
	}
}//func

//-1 mean no position found
int CSSBHopvmld::GetNextHopPosition(int *m_iArray,int nSize,int nFirstIndex)
{
	int i;
	for(i=nFirstIndex;i<nSize;i++){
		m_iDistance++;
		m_iHopSamplesCntr++;
		if(m_iHopSamplesCntr > _DSPLIB_HOP_MAX_SAMPLES)
		{
			m_iHopSamplesCntr=0; return i;
		}
		if(m_iArray[i]!=0) {
			m_iHopSamplesCntr=0;
			return i;
		}
	}
	return -1;
}


//number samples : distance btwn two hops
//m_fSamples = from vector[m_iVectorStart][m_iVectorStartPos] to vector[m_iVectorEnd][m_iVectorEndPos]
BOOL CSSBHopvmld::DDC(int m_iVectorStart,
					  int m_iVectorStartPos,
					  int m_iVectorEnd,
					  int m_iVectorEndPos,
					  double m_fFc,
					  double **m_fFcCorrection1,
					  double **m_fFcCorrection2,
					  int &m_nOutSize
					  )
{
	int ret,i,iVector;
	double *pPointer;
	int nLen;
	m_timeDDS = 1;//initial phase
	int nFilterSizeOut;
	int outFirIndexI=0,outFirIndexQ=0;
	double *m_fOutFirI=NULL, *m_fOutFirICopy=NULL;
	double *m_fOutFirQ=NULL, *m_fOutFirQCopy=NULL;

	int outHilbertIndex=0;
	double *m_fOutHilbert=NULL ;

	m_firLPF_I.Reset();
	m_firLPF_Q.Reset();
	//fill dummy samples before actual data for the sake of filter delay
	//m_firLPF_I.Filter(m_fMultCosLast,m_firLPF_I.m_nTapSize,NULL,nFilterSizeOut);
	//m_firLPF_Q.Filter(m_fMultSinLast,m_firLPF_I.m_nTapSize,NULL,nFilterSizeOut);
	
	//m_hilbert.Reset();
	m_fir_hilbert.Reset();

	m_iHopNumberSamples = 0;
	m_nOutSize=0;
	//FILE *fpt = fopen("c:\\samples\\fir_in.txt","wt");

	for (iVector=m_iVectorStart;iVector<=m_iVectorEnd+1;iVector++)
	{
		if(iVector == m_iVectorEnd + 1)
		{//dump data for filling out fir samples
			nLen = m_nSizeFFT;
			pPointer = m_fZeroMemory;
		}else if(iVector == m_iVectorStart)
		{//first vector index
			nLen = (m_vSamplesVector[iVector]->m_nSize - m_iVectorStartPos);
			pPointer = (m_vSamplesVector[iVector]->m_fSamples)+m_iVectorStartPos;//read pointer
		}else if(iVector==m_iVectorEnd)
		{//last vector index
			pPointer = (m_vSamplesVector[iVector]->m_fSamples);//read pointer
			nLen = ( m_iVectorEndPos + 1);
		}else{//not first nor last vector index
			nLen = (m_vSamplesVector[iVector]->m_nSize);
			pPointer = (m_vSamplesVector[iVector]->m_fSamples);//read pointer	
		}//if
		if(iVector != m_iVectorEnd+1)		m_iHopNumberSamples += nLen;

		//Do DDC on pPointer(0:nLen-1)---------------------------
		//[a]-mult by DDS
		for(i=0;i<nLen;i++)
		{
			m_fMultSin[i] = sin(m_timeDDS*2*M_PI*m_fFc/m_fFs) * pPointer[i];
			m_fMultCos[i] = cos(m_timeDDS*2*M_PI*m_fFc/m_fFs) * pPointer[i];
			m_timeDDS++;
			//fprintf(fpt,"%f\n",m_fMultCos[i]);
		}

		//[b]-LPF
		//---------------I Channel------------------------------
		m_fOutFirICopy = m_fOutFirI;
		if( (m_fOutFirI =(double *) realloc( m_fOutFirI, 
			((outFirIndexI+nLen+1e4) * sizeof( double )) )) ==  NULL )
		{
			free( m_fOutFirICopy );  // free original block
			free(m_fOutFirQ);
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}

		if((ret = m_firLPF_I.Filter(m_fMultCos,nLen,m_fOutFirI+outFirIndexI
			,nFilterSizeOut))!=DSP_RET_SUCCESS)
		{
			free(m_fOutFirI);
			free(m_fOutFirQ);
			return ret;
		}
		outFirIndexI += nFilterSizeOut;
		//---------------Q Channel------------------------------
		m_fOutFirQCopy = m_fOutFirQ;
		if( (m_fOutFirQ =(double *) realloc( m_fOutFirQ, 
			((outFirIndexQ+nLen+1e4) * sizeof( double )) )) ==  NULL )
		{
			free( m_fOutFirQCopy );  // free original block
			free(m_fOutFirI);
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}

		if((ret = m_firLPF_Q.Filter(m_fMultSin,nLen,m_fOutFirQ+outFirIndexQ
			,nFilterSizeOut))!=DSP_RET_SUCCESS)
		{
			free(m_fOutFirQ);
			free(m_fOutFirI);
			return ret;
		}
		outFirIndexQ += nFilterSizeOut;
	}//for iVector

#ifdef _DSPLIB_DEMOD_SSB_SAVE_FIR_I
	FILE *fp = fopen(_DSPLIB_DEMOD_SSB_SAVE_FIR_I_FILE,"wt");
	if(!fp){ 
		printf("cannot open file : %s\n",_DSPLIB_DEMOD_SSB_SAVE_FIR_I_FILE);
	}else{
		int u;
		for(u=0;u<m_iHopNumberSamples;u++)
			fprintf(fp,"%f\n",m_fOutFirI[u]);//+m_firLPF_I.m_nTapSize]);
		fclose(fp);
	}
#endif

	//--------------apply Hilbert transform---------------------------
	//[1] m_cOutHilbert = imag(hilbert(m_fOutFirQ))
	m_fOutHilbert = new double[m_iHopNumberSamples+m_fir_hilbert.m_nFFT*4];
	if(!m_fOutHilbert)
	{
		free(m_fOutFirI);
		free(m_fOutFirQ);
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}

	//hilbert Using CHilbertTransform
	//ret = m_hilbert.Filter(m_fOutFirQ ,m_iHopNumberSamples,m_cOutHilbert
	//	,nFilterSizeOut);
	ret = m_fir_hilbert.Filter(m_fOutFirQ ,m_iHopNumberSamples,m_fOutHilbert
		,nFilterSizeOut);
	if(ret != DSP_RET_SUCCESS)
	{
		free(m_fOutFirI);
		free(m_fOutFirQ);
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	outHilbertIndex += nFilterSizeOut;
	//ret = m_hilbert.Filter(m_fZeroMemory ,m_hilbert.m_nFFT
	//	,m_cOutHilbert + outHilbertIndex,nFilterSizeOut);
	ret = m_fir_hilbert.Filter(m_fZeroMemory ,m_fir_hilbert.m_nFFT
		,m_fOutHilbert + outHilbertIndex,nFilterSizeOut);
	if(ret != DSP_RET_SUCCESS)
	{
		free(m_fOutFirI);
		free(m_fOutFirQ);
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	outHilbertIndex += nFilterSizeOut;

	//hilbert using fir
	//m_fir_hilbert

	//[1]  m_fOutFirI + imag(hilbert(m_fOutFirQ))
	//[2]  m_fOutFirI - imag(hilbert(m_fOutFirQ))
	double *m_fHilbertImag = m_fOutHilbert;//new double[m_iHopNumberSamples];
	//if(!m_fHilbertImag)
	//{
	//	free(m_fOutFirI);
	//	free(m_fOutFirQ);
	///	delete[] m_fOutHilbert;
	//	return DSP_RET_MEMORY_ALLOCATE_ERROR;
	//}
	*m_fFcCorrection1 = new double[m_iHopNumberSamples];
	if(!(*m_fFcCorrection1)){
		free(m_fOutFirI);
		free(m_fOutFirQ);
		delete[] m_fOutHilbert;
		//delete[] m_fHilbertImag;
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	*m_fFcCorrection2 = new double[m_iHopNumberSamples];
	if(!(*m_fFcCorrection2)){
		free(m_fOutFirI);
		free(m_fOutFirQ);
		delete[] m_fOutHilbert;
		//delete[] m_fHilbertImag;
		delete[] (*m_fFcCorrection1);
		return DSP_RET_MEMORY_ALLOCATE_ERROR;
	}
	int offset = m_fir_hilbert.m_nTapSize/2;
	vdAdd(offset,m_fZeroMemory,m_fHilbertImag,*m_fFcCorrection1);
	vdSub(offset,m_fZeroMemory,m_fHilbertImag,*m_fFcCorrection2);

	vdAdd(m_iHopNumberSamples-offset,m_fOutFirI,m_fHilbertImag+offset,(*m_fFcCorrection1)+offset);
	vdSub(m_iHopNumberSamples-offset,m_fOutFirI,m_fHilbertImag+offset,(*m_fFcCorrection2)+offset);
	m_nOutSize = m_iHopNumberSamples;

#ifdef _DSPLIB_DEMOD_SSB_SAVE_HILBERT
	FILE *fph = fopen(_DSPLIB_DEMOD_SSB_SAVE_HILBERT_FILE,"wt");
	if(!fph){ 
		printf("cannot open file : %s\n",_DSPLIB_DEMOD_SSB_SAVE_HILBERT_FILE);
	}else{
		int u;
		for(u=0;u<m_iHopNumberSamples;u++)
			fprintf(fph,"%f\n",(*m_fFcCorrection1)[u]);
		fclose(fph);
	}
#endif
	//fclose(fpt);
	free(m_fOutFirI);
	free(m_fOutFirQ);
	delete[] m_fOutHilbert;
	//delete[] m_fHilbertImag;

	return DSP_RET_SUCCESS;
}

BOOL CSSBHopvmld::Delta_Freq_Estimator(double *m_fSamples,int m_nInputSize,double &m_fDeltaFreq)
{
	double m_F0=0, m_fThreshold;
	int blk, blkNum = m_nInputSize/_DSPLIB_DELTA_FREQ_L1;
	int nFirstIdx=0,rez;
	long status;
	int pos;
	m_fDeltaFreq =0;
	memset(m_fMultHannFFTSum,0,_DSPLIB_DELTA_FREQ_L2/2 * sizeof(double));
	//compute priodogram of m_fSamples * Hanning window
	//WriteToFile("c:\\samples\\hop_in.txt",m_fSamples,m_nInputSize);
	for(blk=0;blk<=blkNum;blk++)
	{
		nFirstIdx=blk * _DSPLIB_DELTA_FREQ_L1;
		if((m_nInputSize - nFirstIdx) < _DSPLIB_DELTA_FREQ_L1)
		{
			rez = m_nInputSize - nFirstIdx;
			//y=[x zeros(L1-length(x)] * hann(L1)
			vdMul(rez,m_fSamples+nFirstIdx,	m_fHannWindow,m_fMultHann);
			memset(m_fMultHann+rez,0,(_DSPLIB_DELTA_FREQ_L1-rez)*sizeof(double));

		}else{
			vdMul(_DSPLIB_DELTA_FREQ_L1,m_fSamples+nFirstIdx,m_fHannWindow,m_fMultHann);
		}//if
		//WriteToFile("c:\\samples\\parts_in.txt",m_fMultHann,_DSPLIB_DELTA_FREQ_L2,TRUE);
		//WriteToFile("c:\\samples\\fft_in.txt",m_fMultHann,_DSPLIB_DELTA_FREQ_L1);
		status = DftiComputeForward(m_pDescFFTHann,m_fMultHann,m_cMultHannFFT);
		vzAbs(_DSPLIB_DELTA_FREQ_L2/2,m_cMultHannFFT,m_fMultHannFFTAbs);
		//WriteToFile("c:\\samples\\fft_out.txt",m_fMultHannFFTAbs,_DSPLIB_DELTA_FREQ_L2/2);
		vdAdd(_DSPLIB_DELTA_FREQ_L2/2,m_fMultHannFFTSum,m_fMultHannFFTAbs,m_fMultHannFFTSum);
	}//for blk
	//A=log(HannZs);
	//A(L2/2+1:L2)=0;
	//WriteToFile("c:\\samples\\sum_abs_out.txt",m_fMultHannFFTSum,_DSPLIB_DELTA_FREQ_L2/2);
	
	vdLn(_DSPLIB_DELTA_FREQ_L2/2,m_fMultHannFFTSum,m_fMultHannLn);
	for(blk=0;blk<_DSPLIB_DELTA_FREQ_L2/2;blk++)
	{
		m_cMultHannLn[blk].real=m_fMultHannLn[blk];
	}
#ifdef _DSPLIB_DEMOD_SSB_SAVE_CEPTRUM_IN
	FILE *fpci = fopen(_DSPLIB_DEMOD_SSB_SAVE_CEPTRUM_IN_FILE,"wt");
	if(!fpci){ 
		printf("cannot open file : %s\n",_DSPLIB_DEMOD_SSB_SAVE_CEPTRUM_IN_FILE);
	}else{
		for(int u=0;u<_DSPLIB_DELTA_FREQ_L2;u++)
			fprintf(fpci,"%f\n",m_cMultHannLn[u].real);
		fclose(fpci);
	}
#endif
	status = DftiComputeBackward(m_pDescIFFTHann,m_cMultHannLn,m_cIFFTHannLn);
	//cep=abs(ifft(A));
	//cblas_dcopy(_DSPLIB_DELTA_FREQ_L2,(double *)m_cIFFTHannLn,2,
	//	m_fCeptrum,1);
	//cep = abs(ifft(A));
	vzAbs(_DSPLIB_DELTA_FREQ_L2,m_cIFFTHannLn,m_fCeptrum);
	//cep(1:10)=0
	memset(m_fCeptrum,0,10*sizeof(double));
	//[max_ceps index_ceps]=max(abs(cep(1:end/2)));
	//Fo=Fs/index_ceps;

	//test iift in
//	FILE *fpif = fopen("c:\\samples\\ssb_cep_out.txt","wt");
//	if(!fpif){ 
//		printf("cannot open file : ssb_cep_out.txt\n");
//	}else{
//		for(int u=0;u<m_nInputSize;u++)
//			fprintf(fpif,"%f\n",m_fSamples[u]);
//		fclose(fpif);
//	}
		
	int idx,cept_idx=250;
	for(pos=100-70;pos <500-70 ;pos++)
	{
		idx = cblas_idamax(141,m_fCeptrum+pos,1);
		if(idx == 70)
		{
			cept_idx=pos+70;
		}
	}//for pos
	m_F0 = m_fFs / (double)(cept_idx+1);
	//WriteToFile("c:\\samples\\fo.txt",&m_F0,1,TRUE);

	//Thresh=max(HannZs)/8;
	idx = cblas_idamax(_DSPLIB_DELTA_FREQ_L2/2,m_fMultHannFFTSum,1);
	m_fThreshold = m_fMultHannFFTSum[idx]/m_iThreshold;
	m_fDeltaFreq = 0;
	
	//find max index by a 5 samples window
	for(pos=1500*_DSPLIB_DELTA_FREQ_L2/m_fFs +12;pos >=5 ;pos--)
	{
		idx = cblas_idamax(11,m_fMultHannFFTSum+pos,1);
		if(idx == 5)
		{
			//if HannZs(max_index(i))> Thresh && max_index(i)*Fs/L2<3000
			if(m_fMultHannFFTSum[pos+5] > m_fThreshold )
			{
				//Delta_Freq=max_index(i)*Fs/L2+Fo;
				if(m_bF0)
					m_fDeltaFreq = ((pos+5)*m_fFs/_DSPLIB_DELTA_FREQ_L2)+m_F0;
				else
					m_fDeltaFreq = ((pos+5)*m_fFs/_DSPLIB_DELTA_FREQ_L2);
				break;
			}
		}
	}//for pos
	return DSP_RET_SUCCESS;
}//func

BOOL CSSBHopvmld::DemodDDC(int m_iVectorStart,
			  int m_iVectorStartPos,
			  int m_iVectorEnd,
			  int m_iVectorEndPos,
			  double m_fFc,
			  double *m_fDemodOut,//must be created outside
			  int &m_nOutSize
			  )
{
	int ret,i,iVector;
	double *pPointer;
	int nLen;
	int nFilterSizeOut;
	int outFirIndex=0;

	m_iHopNumberSamples = 0;
	m_nOutSize=0;

	m_fir_Demod.Reset();
	for (iVector=m_iVectorStart;iVector<=m_iVectorEnd+1;iVector++)
	{
		if(iVector == m_iVectorEnd + 1)
		{//dump data for filling out fir samples
			nLen = m_nSizeFFT;
			pPointer = m_fZeroMemory;
		}else if(iVector == m_iVectorStart)
		{//first vector index
			nLen = (m_vSamplesVector[iVector]->m_nSize - m_iVectorStartPos);
			pPointer = (m_vSamplesVector[iVector]->m_fSamples)+m_iVectorStartPos;//read pointer
		}else if(iVector==m_iVectorEnd)
		{//last vector index
			pPointer = (m_vSamplesVector[iVector]->m_fSamples);//read pointer
			nLen = ( m_iVectorEndPos + 1);
		}else{//not first nor last vector index
			nLen = (m_vSamplesVector[iVector]->m_nSize);
			pPointer = (m_vSamplesVector[iVector]->m_fSamples);//read pointer	
		}//if
		if(iVector != m_iVectorEnd+1)		m_iHopNumberSamples += nLen;

		//Do DDC on pPointer(0:nLen-1)---------------------------
		//[a]-mult by DDS
		for(i=0;i<nLen;i++)
		{
			m_fMultCos[i] = cos(m_timeDemodDDS*2*M_PI*m_fFc/m_fFs) * pPointer[i];
			m_timeDemodDDS++;
		}

		//[b]-LPF

		if((ret = m_fir_Demod.Filter(m_fMultCos,nLen,m_fDemodOut+outFirIndex
			,nFilterSizeOut))!=DSP_RET_SUCCESS)
		{
			return ret;
		}
		outFirIndex += nFilterSizeOut;
		//m_nOutSize = outFirIndex;
	}//for iVector
	m_nOutSize = m_iHopNumberSamples;

	return DSP_RET_SUCCESS;
}//func

}//namespce