#ifndef MATLAB_BGL_COMMON_FUNCTIONS_H
#define MATLAB_BGL_COMMON_FUNCTIONS_H

/*
 * David Gleich
 * Copyright, Stanford University, 2007
 */

/**
 * @file common_functions.h
 * Implement a few common functions to be included in many mex files.
 */

/*
 * 8 July 2007
 * Initial version
 */

char* load_string_arg(const mxArray* a, int k)
{
    mwSize buflen;
    char *s;
    int status;
    
    if (mxIsChar(a) != 1) {
        mexErrMsgIdAndTxt("matlab_bgl:invalidMexArgument",
            "argument %i must be a string", k+1);
    }
    
    /* Input must be a row vector. */
    if (mxGetM(a) != 1) {
        mexErrMsgIdAndTxt("matlab_bgl:invalidMexArgument",
            "argument %i must be a string (and a row vector)", k+1);
    }
    
    /* Get the length of the input string. */
    buflen = (mxGetM(a) * mxGetN(a)) + 1;

    /* Allocate memory for input and output strings. */
    s = mxCalloc(buflen, sizeof(char));

    status = mxGetString(a, s, buflen);
    if (status != 0) {
        mexErrMsgIdAndTxt("matlab_bgl:sizeError",
            "insufficient space to copy argument %i to a string", k+1);
    }
    
    return s;
}

double load_scalar_arg(const mxArray* a, int k)
{
    if (mxGetNumberOfElements(a) > 1 || !mxIsDouble(a)) {
        mexErrMsgIdAndTxt("matlab_bgl:invalidMexArgument",
            "argument %i should be a scalar", k+1);
    }
    return mxGetScalar(a);
}

#endif /* MATLAB_BGL_COMMON_FUNCTIONS_H */

