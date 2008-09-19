function [D,P] = floyd_warshall_all_sp(A,varargin)
% FLOYD_WARSHALL_ALL_SP Compute the weighted all-pairs shortest path problem.
%
% The Floyd-Warshall algorithm for the all-pairs shortest path problem 
% works only on graphs without negative edge weights.  This method should
% be used over the Johnson algorithm for dense graphs.  
%
% This algorithm can return the predecessor matrix.
%
% This method works on weighted directed graphs.
% The runtime is O(V^3).
%
% See the shortest_paths function for calling information.  This function 
% just calls all_shortest_paths(...,struct('algname','floyd_warshall'));
%
% Example:
%    load graphs/clr-26-1.mat
%    floyd_warshall_all_sp(A)
%
% See also ALL_SHORTEST_PATHS, JOHNSON_ALL_SP.

%
% David Gleich
% 23 April 2006
%

% History
% 2008-04-02: Added documenation for predecessor matrix

algname = 'floyd_warshall';

if (nargin > 1)
    options = varargin{1};
    options.algname = algname;
else
    options = struct('algname',algname);
end;

if nargout > 1
    [D,P] = all_shortest_paths(A,options);
else
    D = all_shortest_paths(A,options);
end




