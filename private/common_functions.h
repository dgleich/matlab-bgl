#ifndef MATLAB_BGL_COMMON_FUNCTIONS_H
#define MATLAB_BGL_COMMON_FUNCTIONS_H

/** @file common_functions.h
 * @copyright Stanford University, 2008
 * @author David F. Gleich
 * Implement a few common functions to be included in many mex files.
 */

/** History
 *  2007-07-08: Initial version
 *  2008-09-26: Added isscalar and isscalardoube
 *  2008-09-27: Added intmin
 */

int intmin(int a, int b) { if (a<b) { return a; } else { return b; } }

int isscalar(const mxArray* a) {
	if (mxGetM(a) == 1 && mxGetN(a) == 1 && mxGetNumberOfDimensions(a) == 2) {
		return 1;
	} else {
		return 0;
	}
}

int isscalardouble(const mxArray* a) {
	if (isscalar(a) && mxIsDouble(a)) {
		return 1;
	} else {
		return 0;
	}
}

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

