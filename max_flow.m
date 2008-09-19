function [flowval cut R F] = max_flow(A,u,v,varargin)
% MAX_FLOW Compute the max flow on A from u to v.
%
% flowval=max_flow(A,u,v) computes the maximum flow on the network defined by
% the adjacency structure A, with source u and sink v.
%
% [flowval cut R F] = max_flow(A,u,v) returns the maximum flow in the 
% network A with source u and sink v as well as additional information.  
% For each vertex on the source side of the mincut, mincut(i) = 1, 
% for each vertex on the sink side, mincut(i) = -1.  
% R is the residual graph.  R(i,j) is the amount of unused capacity 
% on edge (i,j).  F is the flow graph, F(i,j) is the amount of used 
% capacity on edge (i,j).  F, A, and R satisfy the relationship A = F + R.
%
% The optional parameter algname specifies the algorithm used to compute
% the maximum flow.  For reference, the push relabel method is likely the 
% best general purpose algorithm.  The Edmunds-Karp algorithm 
%
% ... = max_flow(A,optionsu) sets optional parameters (see 
% set_matlab_bgl_options) for the standard options.
%   options.algname: the max flow/min cut algorithm
%   [{'push_relabel'} | 'edmunds_karp' | 'kolmogorov']
%
% Note: the values on A are interpreted as integers, please round them
% yourself to get the best interpretation.  The code uses the floor of 
% the values in A.
%
% Example:
%    load graphs/max_flow_example.mat
%    max_flow(A,1,8)

%
% David Gleich
% Copyright, Stanford University, 2006-2007
%

%
% 16 April 2006
% Initial version
%
% 31 May 2006
% Added full2sparse check
%
% 8 July 2007
% Added additional algname
% Fixed transpose option to implement the pretranspose
% Fixed documentation bug
%
% 9 July 2007
% Added non-negative edge capacities check
%

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A)
    A = sparse(A); 
end

options = struct('algname', 'push_relabel');
if ~isempty(varargin)
    options = merge_structs(varargin{1}, options);
end;

if check
    % the matrix cannot have nonnegative capacities
    check_matlab_bgl(A,struct('noneg',1));
end


% max_flow will transpose the data inside
if ~trans
    % but that means they are passing the transposed data, so we need to
    % pre-transpose... 
    A = A';
end

if (check)
    % remove any non-zero diagonals
    A = A - diag(diag(A));
end

n = size(A,1);


if nargout == 2
    [flowval cut] = max_flow_mex(A,u,v,lower(options.algname));
elseif nargout >= 3
    [flowval cut ri rj rv] = max_flow_mex(A,u,v,lower(options.algname));
    
    % If anyone needs this operation to be more efficient, send me email, 
    % and I can make max_flow_mex return this more efficiently.
    R = sparse(ri,rj,rv,n,n);
    if ~trans
        R = R';
    end
else
    flowval = max_flow_mex(A,u,v,lower(options.algname));
end

if nargout >= 4
    F = A - R;
end


