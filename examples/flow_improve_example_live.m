%% A Flow Refinement Idea 
% Reid Andersen and Kevin Lang propose an algorithm to take a graph
% partition and improve it using a sequence of max-flow problems in the
% paper "An Algorithm for Improving Graph Partitions."  Proceedings of the 
% 19th annual ACM-SIAM Symposium on Discrete Algorithms (SODA2008), 2008, 
% pp. 651-660.  We can quickly implement and test their idea using the
% max-flow routine in MatlabBGL.  

%% Adjust the figures to be slightly nicer
set(0,'defaultlinelinewidth',.5);
rand('state',0); % ensure we are looking at the same output!

%% Flow improve is in the custom subdirectory of derived algorithms
addpath_mbgl_custom

%% The code
% The implementation follows the notation of |Improve| algorithm in the 
% paper closely.  
edit flow_improve.m

%% The US Road Network
% We'll examine these algorithms with the continential US road network.
% This example will model a real world "bigger" graph.

%% 
% Get the data from Tim Davis's matrix repository

addpath('~/data/uf'); % add the link to the local uf sparse matrix repo
mat = UFget('Gleich/usroads-48');
G = mat.A; % extract the graph, and coordinates
xy = mat.aux.coord;
fprintf('verts = %7i\n', size(G,1));
fprintf('edges = %7i\n', nnz(G));

%%
% A 
ms = 3; % set the marker size to 3 pt
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);axis tight; axis off;


%%
% Bisect the US vertically
A = xy(:,1)>-90; 
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);  axis tight; axis off; 
hold on; plot(xy(A,1), xy(A,2),'r.','MarkerSize',ms); hold off;

%%
% Improve the partition
S = flow_improve(G,A,[]);
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);  axis tight; axis off; 
hold on; plot(xy(S,1), xy(S,2),'r.','MarkerSize',ms); hold off;
