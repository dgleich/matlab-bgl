function [p,q,r,s]=matching_dmperm(A)
% MATCHING_DMPERM Implement an equivalent dmperm function with matching
% 
% The Matlab DMPERM function (as of R2007a) uses a recursive implementation
% which can cause stack overflows for large matrices.  This function
% implements the DMPERM function in terms of the non-recursive MATCHING
% function in MatlabBGL.
%
% [p,q,r,s]=matching_dmperm(A) computes equivalent output to Matlab's
% DMPERM function.
%
% See also DMPERM, SPRANK

[nr nc]=size(A);
GA=spaugment(A,0);
rv=1:nr; cv=nr+1:nr+nc;
m=matching(GA);
cbar=find(m(cv)==0);
rbar=find(m(rv)==0);
d_cbar=setbfs(GA,cbar+nr);
r1=find(mod(d_cbar(rv),2)==1 & d_cbar(rv)>0);
c1=m(r1)-nr;
d_rbar=setbfs(GA,rbar);
c3=find(mod(d_rbar(cv),2)==1 & d_rbar(cv)>0);
r3=m(c3+nr);
rused=false(nr,1); rused(rbar)=1; rused(r1)=1; rused(r3)=1; 
r2=find(~rused);
cused=false(nc,1); cused(cbar)=1; cused(c1)=1; cused(c3)=1; 
c2=find(~cused);
B=A(r2,c2);
ccB=components(B);
[ignore ccp]=sort(ccB);
p=[r1; r2(ccp); r3; rbar];
q=[cbar; c1; c2(ccp); c3];
end

function d=setbfs(A,S)
% SETBFS Compute BFS from a set of vertices
n=size(A,1);
A(n+1,n+1)=0; % extend A
A(n+1,S)=1;
d=bfs(A,n+1);
d(end)=[];
d(d>0)=d(d>0)-1;
end