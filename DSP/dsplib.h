/*
			*****************DSP Library*****************
			General Definition for all DSP Functions
			Author : Ahmad Shafaati
			Date:	87/4/1
*/
#ifndef _DSP_LIBRARY
	#define _DSP_LIBRARY
#endif

#define BULK_SIZE 8192

#include "func_interv.h"
#include "mkl_vml.h"
#include "mkl_types.h"
#include "mkl_cblas.h"
#include "mkl_dfti.h"
#include <Windows.h>
#define  _USE_MATH_DEFINES
#include <math.h>

#include "dsplib_general_func.h"
#include "dsplib_fir_design.h"
#include "dsplib_fir_ola.h"
#include "dsplib_hilbert_ola.h"
#include "dsplib_cic.h"
/*
#include "dsplib_amr_analog_hf.h"
#include "dsplib_amr_module.h"
#include "Cdsplib_amr_module_base.h"
#include "Cdsplib_amr_analog.h"

#include "dsplib_demod_fm.h"
#include "dsplib_demod_am.h"
#include "dsplib_demod_ssb_hop.h"
#include "dsplib_debug.h"
#include "dsplib_amr_digital.h"
#include "dsplib_demod_ssb_fixed.h"
*/
#include <math.h>
