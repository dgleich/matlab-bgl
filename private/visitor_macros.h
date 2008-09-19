#ifndef VISITOR_MACROS_H
#define VISITOR_MACROS_H

/*
 * ==============================================================
 * visitor_macros.h A set of macros to define various commands 
 * to work with visitors and MatlabBGL
 *
 * David Gleich
 * 31 May 2006
 * =============================================================
 */

#define PROTOTYPE_VISITOR_VERTEX_FUNCTION(FUNC) \
int call_matlab_## FUNC (void *pdata, mbglIndex u);

#define PROTOTYPE_VISITOR_EDGE_FUNCTION(FUNC) \
int call_matlab_## FUNC (void *pdata, mbglIndex ei, mbglIndex u, mbglIndex v);

#define CHECK_AND_SET_VISITOR_FUNCTION(A,FUNC,VISSTR) \
    if (mxGetFieldNumber(A, #FUNC) == -1)  \
    { \
        VISSTR . FUNC = NULL; \
    } \
    else \
    { \
        mxArray* f = mxGetField(A,0, #FUNC ); \
        switch (mxGetClassID(f)) \
        { \
            case mxFUNCTION_CLASS: case 23: \
                VISSTR . FUNC  = call_matlab_## FUNC ;\
                break; \
            default: \
                mexWarnMsgTxt("Invalid visitor for " #FUNC "."); \
                VISSTR .FUNC  = NULL; \
                break; \
        } \
    } 

#define CALL_MATLAB_VERTEX_VISITOR_FUNCTION(NAME) \
int call_matlab_## NAME  (void *pdata, mbglIndex u) \
{ \
    mxArray* func; \
    mxArray* vis = pdata; \
     \
    mxArray* prhs[2]; \
    mxArray* plhs[1]; \
     \
    func = mxGetField(pdata,0,#NAME);  \
     \
    prhs[0] = func; \
    prhs[1] = mxCreateDoubleScalar((double)(u+1)); \
     \
    plhs[0] = NULL; \
     \
    mexCallMATLAB(0,plhs,2,prhs,"feval"); \
    if (plhs[0] != NULL) \
    { \
        if (mxGetNumberOfElements(plhs[0]) > 1  \
            || !(mxIsDouble(plhs[0]) || mxIsLogical(plhs[0]))) \
        { \
            mexWarnMsgTxt("Invalid return from " #NAME ".");\
            return (0); \
        } \
         \
        if (mxIsDouble(plhs[0])) \
        { \
            return ((int)mxGetScalar(plhs[0])); \
        } \
        else \
        { \
            return ((int)mxGetScalar(plhs[0])); \
        } \
    } \
     \
    return (1); \
}

#define CALL_MATLAB_EDGE_VISITOR_FUNCTION(NAME) \
int call_matlab_## NAME  (void *pdata, mbglIndex ei, mbglIndex u, mbglIndex v) \
{ \
    mxArray* func; \
    mxArray* vis = pdata; \
     \
    mxArray* prhs[4]; \
    mxArray* plhs[1]; \
     \
    func = mxGetField(pdata,0,#NAME);  \
     \
    prhs[0] = func; \
    prhs[1] = mxCreateDoubleScalar((double)(ei+1)); \
    prhs[2] = mxCreateDoubleScalar((double)(u+1)); \
    prhs[3] = mxCreateDoubleScalar((double)(v+1)); \
     \
    plhs[0] = NULL; \
     \
    mexCallMATLAB(0,plhs,4,prhs,"feval"); \
    if (plhs[0] != NULL) \
    { \
        if (mxGetNumberOfElements(plhs[0]) > 1  \
            || !(mxIsDouble(plhs[0]) || mxIsLogical(plhs[0]))) \
        { \
            mexWarnMsgTxt("Invalid return from " #NAME ".");\
            return (0); \
        } \
         \
        if (mxIsDouble(plhs[0])) \
        { \
            return ((int)mxGetScalar(plhs[0])); \
        } \
        else \
        { \
            return ((int)(mxGetScalar(plhs[0]))); \
        } \
    } \
     \
    return (1); \
}


#endif /* VISITOR_MACROS_H */


