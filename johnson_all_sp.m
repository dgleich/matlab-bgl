function D = johnson_all_sp(A,varargin)
% JOHNSON_ALL_SP Compute the weighted all-pairs shortest path problem.
%
% Johnson's algorithm for the all-pairs shortest path problem 
% works only on graphs without negative edge weights.  This method should
% be used over the Floyd-Warshall algorithm for sparse graphs.  
%
% This method works on weighted directed graphs.
% The runtime is O(VE log(V)).
%
% See the shortest_paths function for calling information.  This function 
% just calls all_shortest_paths(...,struct('algname','johnson'));
%
% Example:
%    load graphs/clr-26-1.mat
%    johnson_all_sp(A)
%
% See also ALL_SHORTEST_PATHS, FLOYD_WARSHALL_ALL_SP.

%
% David Gleich
% 23 April 2006
%

algname = 'johnson';

if (nargin > 1)
    options = varargin{1};
    options.algname = algname;
else
    options = struct('algname',algname);
end;

D = all_shortest_paths(A,options);




