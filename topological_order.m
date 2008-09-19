function perm=topological_order(A,varargin)
% TOPOLOGICAL_ORDER Returns the topological ordering of vertices in a dag
%
% perm=topological_order(A) generates a permutation of the vertices that
% indices a topological order.  A topological order satisfies the property
% that edge (i,j) in A implies i comes before j in the order.  A
% topological order only exists for directed acyclic graphs.  If A is not a
% directed acyclic graph, then the returned perm is empty.
%
% This method works on directed graphs.
% The runtime is O(V+E).
%
% ... = topological_order(A,options) sets optional parameters (see 
% set_matlab_bgl_options) for the standard options.
%   There are no additional options for this function.
%
% Note: this function does not depend upon the non-zero values of A, but
% only uses the non-zero structure of A.
%
% Example:
%    load graphs/bfs_example.mat
%    d = bfs(A,1)
%
% See also TEST_DAG

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if (full2sparse && ~issparse(A)) 
    A = sparse(A); 
end
if (check) 
    check_matlab_bgl(A,struct()); 
end
if (trans) 
    A = A'; 
end

[perm dag] = topological_order_mex(A);

% in this case it wasn't a dag
if dag == 0
    perm = [];
end