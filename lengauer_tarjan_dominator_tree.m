function pred=lengauer_tarjan_dominator_tree(A,u,varargin)
% LENGAUER_TARJAN_DOMINATOR_TREE Compute a dominator tree for a graph.
%
% A dominator tree encodes a dominates relations.  A vertex u dominates a
% vertex v if all paths to v must go through u.  In an undirected graph,
% this means that if u dominates v, then v will be disconnected from the
% graph if we remove u and start the search at the root.  A dominator tree
% is rooted at a particular vertex and the entire graph must be reachable
% from that vertex.  (Historically, these were called flowgraphs.)
%
% p = lengauer_tarjan_dominator_tree(A,u) returns the predecessor array for
% the dominator tree rooted at u.  
%
% The runtime is O((V+E) log (V+E)) and the algorithm works on unweighted,
% directed graphs.
%
% ... = lengauer_tarjan_dominator_tree(A,options) sets optional parameters 
% (see set_matlab_bgl_options) for the standard options.
%   There are no additional options for this function.
%
% Note: this function does not depend upon the non-zero values of A, but
% only uses the non-zero structure of A.
%
% Example:
%   load('graphs/dominator_tree_example.mat');
%   p=lengauer_tarjan_dominator_tree(A);
%   

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A), A = sparse(A); end
if check, check_matlab_bgl(A,struct()); end
if trans, A = A'; end

pred = dominator_tree_mex(A,u);
