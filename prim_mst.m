function [varargout] = prim_mst(A,options)
% PRIM_MST Compute a minimum spanning with Prims's algorithm.
%
% Prim's MST algorithm computes a minimum spanning tree for a graph.
%
% This method works on weighted symmetric graphs without negative edge
% weights.
% The runtime is O(E log (V)).
%
% See MST for calling information.  This function just calls
% mst(...,struct('algname','prim'));
%   options.root: specify the root vertex for prim's algorithm
%       [{'none'} | any vertex number]
%
% Example:
%    load graphs/clr-24-1.mat
%    prim_mst(A)
%    T = prim_mst(A,struct('root',5));
%
% See also MST, KRUSKAL_MST.

%
% David Gleich
% 23 April 2006
%

%
% 23 April 2007
% Fixed bug with output without parameters
%
% 8 July 2007
% Fixed documentation typo
%

if (nargin > 1)
    options.algname = 'prim';
else
    options = struct('algname','prim');
end;

varargout = cell(1,max(nargout,1));

[varargout{:}] = mst(A,options);

