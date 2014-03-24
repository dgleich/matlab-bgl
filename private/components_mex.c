/*
 * ==============================================================
 * components_mex.c The mex interface to the matlab bgl wrapper.
 *
 * David Gleich
 * 21 April 20020
 * =============================================================
 */

/*
 * 19 February 2007
 * Updated to use Matlab 2006b sparse matrix interface
 *
 * 25 February 2007
 * Updated to use expand macros
 */


#include "mex.h"

#if MX_API_VER < 0x07030000
typedef int mwIndex;
typedef int mwSize;
#endif /* MX_API_VER */

#include "matlab_bgl.h"
#include "expand_macros.h"
#include "common_functions.h"

#include <math.h>
#include <stdlib.h>

/*
 * The mex function runs a connected components problem.
 */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    mwIndex i;
    
    mwIndex mrows, ncols;
    
    mwIndex n,nz;
    
    /* sparse matrix */
    mwIndex *ia, *ja;
    
    /* output data */
    double *ci, *sizes;
    mwIndex* int_ci;
    
    mwIndex num_cc;

    /* load the matrix*/
    load_graph_arg(nrhs, prhs, 0, -1, -1, 0, &n, &nz, &ia, &ja, NULL);
    
    /* create the output */
    plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL);
    ci = mxGetPr(plhs[0]);
    
    strong_components(n, ja, ia,
        (mwIndex*)ci);
    
    /* count the number of components */
    int_ci = (mwIndex*)ci;    
    num_cc = 0;
    for (i=0;i<n;i++)
    {
        if (int_ci[i] > num_cc)
        {
            num_cc = int_ci[i];
        }
    }
    
    /* add one to the total because we computed the max index */
    num_cc++;
    
    #ifdef _DEBUG
    mexPrintf("num_cc = %i\n", num_cc);
    #endif 
    
    plhs[1] = mxCreateDoubleMatrix(num_cc,1,mxREAL);
    sizes = mxGetPr(plhs[1]);
    
    for (i=0;i<num_cc;i++)
    {
        sizes[i] = 0.0;
    }
   
        
    for (i=0;i<n;i++)
    {
        sizes[int_ci[i]] += 1.0;
    }
    
    expand_index_to_double((mwIndex*)ci,ci,n,1.0);
    
    #ifdef _DEBUG
    mexPrintf("return\n"); 
    #endif 
}



