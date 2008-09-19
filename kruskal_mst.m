function [varargout] = kruskal_mst(A,options)
% KRUSKAL_MST Compute a minimum spanning with Kruskal's algorithm.
%
% The Kruskal MST algorithm computes a minimum spanning tree for a graph.
%
% This method works on weighted symmetric graphs.
% The runtime is O(E log (E)).
%
% See the mst function for calling information.  This function just calls
% mst(...,struct('algname','kruskal'));
%
% Example:
%    load graphs/clr-24-1.mat
%    kruskal_mst(A)
%
% See also MST, PRIM_MST.

%
% David Gleich
% 23 April 2006
%

if (nargin > 1)
    options.algname = 'kruskal';
else
    options = struct('algname','kruskal');
end;

varargout = cell(1,max(nargout,1));

[varargout{:}] = mst(A,options);

