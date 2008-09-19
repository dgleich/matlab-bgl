%% New features in MatlabBGL version 3.0
% Matlab sparse matrices only support non-zero values. Because the
% structure of the sparse matrix is used to infer the edges of an
% underlying graph this limitation prevents MatlabBGL from trivially 
% addressing graphs with 0-weight edges.
%
% To fix this problem, codes that accept a weighted graph allow the user to
% specify a vector of edge weights for each edge in the graph using the
% optional 'weights' parameter.  Using the 'weights' parameter correctly
% can be difficult due to issues with how edges are indexed in MatlabBGL.

%% A first attempt
% A trivial example graph to illustrate the problem that occurs with 0
% weighted graphs occurs even with a simple cycle.  Suppose that the graph
% corresponding to adjacency matrix A is a symmetric cycle where all edges
% have weight 0 except for one edge between vertices (1,2).