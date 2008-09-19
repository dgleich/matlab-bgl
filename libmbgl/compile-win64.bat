@echo off

rem % SETUP command line options
rem % the first call gets us all the vc8.00 tools including lib.exe

rem % This call gives the cross-compiler for 32-bit windows to 64-bit windows
call D:\vstudio8\VC\bin\x86_amd64\vcvarsx86_amd64.bat

rem % This call gives the 64-bit compiler on 64-bit windows
rem call d:\vstudio8\VC\bin\amd64\vcvarsamd64.bat


set LIBNAME=libmbgl-pcwin64-large.lib
set OUTDIR=Release

set YASMICDIR=.\
set BOOSTDIR=e:\dev\lib\boost_1_33_1

set CFLAGS=-c -nologo -I"%YASMICDIR%" -I"%BOOSTDIR%" /Fo"%OUTDIR%\" /EHsc /D "NDEBUG MATLAB_BGL_LARGE_ARRAYS" /O2 /ML
rem set CFLAGS=-c -nologo -Ie:\dev\yasmic -Ie:\dev\lib\boost_1_33_1 /Fo"%OUTDIR%\\" /EHsc /ML /Od /D "_DEBUG" /Fd"%OUTDIR%\vc70.pdb" /Zi
set LIBFLAGS=-nologo /out:"%OUTDIR%\\%LIBNAME%"

@echo on

cl %CFLAGS% components.cc
cl %CFLAGS% max_flow.cc
cl %CFLAGS% orderings.cc
cl %CFLAGS% searches.cc
cl %CFLAGS% shortest_path.cc
cl %CFLAGS% spanning_trees.cc
cl %CFLAGS% statistics.cc
lib %LIBFLAGS% %OUTDIR%\components.obj 	%OUTDIR%\max_flow.obj %OUTDIR%\orderings.obj %OUTDIR%\searches.obj %OUTDIR%\shortest_path.obj %OUTDIR%\spanning_trees.obj %OUTDIR%\statistics.obj


