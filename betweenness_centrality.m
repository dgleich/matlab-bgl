function [bc,E] = betweenness_centrality(A,varargin)
% BETWEENNESS_CENTRALITY Compute the betweenness centrality for vertices.
%
% bc = betweenness_centrality(A) returns the betweenness centrality for
% all vertices in A.  
%
% [bc,E] = betweenness_centrality(A) returns the betweenness centrality for
% all vertices in A along with a sparse matrix with the centrality for each
% edge.  
%
% This method works on weighted or weighted directed graphs.
% For unweighted graphs (options.unweighted=1), the runtime is O(VE).
% For weighted graphs, the runtime is O(VE + V(V+E)log(V)).
%
% ... = betweenness_centrality(A,options) sets optional parameters (see 
% set_matlab_bgl_options) for the standard options.
%   options.unweighted: use the slightly more efficient unweighted
%     algorithm in the case where all edge-weights are equal [{0} | 1]  
%   options.ec_list: do not form the sparse matrix with edge [{0} | 1]
%   options.edge_weight: a double array over the edges with an edge
%       weight for each node, see EDGE_INDEX and EXAMPLES/REWEIGHTED_GRAPHS
%       for information on how to use this option correctly
%       [{'matrix'} | length(nnz(A)) double vector]
%
% Note: the edge centrality can also be returned as an edge list using the
% options.ec_list options.  This option can eliminate some ambiguity in the
% output matrix E when the edge centrality of an edge is 0 and Matlab drops
% the edge from the sparse matrix.  
%
% Note: if the edge centrality matrix E is not requested, then it is not
% computed and not returned.  This yields a slight savings in computation
% time.  
%
% Example:
%    load graphs/padgett-florentine.mat
%    betweenness_centrality(A)

%
% David Gleich
% 19 April 2006
%
% 2006-05-31: Added full2sparse check
%
% 1 March 2007
% Added edge centrality output
%
% 20 April 2007
% Added edge weight option
%
% 9 July 2007
% Restricted input to positive edge weights
%
% 12 July 2007
% Fixed edge_weight documentation.
%

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if (full2sparse && ~issparse(A)) 
    A = sparse(A); 
end

options = struct('unweighted', 0, 'ec_list', 0, 'edge_weight', 'matrix');
if (~isempty(varargin))
    options = merge_structs(varargin{1}, options);
end;

% edge_weights is an indicator that is 1 if we are using edge_weights
% passed on the command line or 0 if we are using the matrix.
edge_weights = 0;
edge_weight_opt = 'matrix';

if strcmp(options.edge_weight, 'matrix')
    % do nothing if we are using the matrix weights
else
    edge_weights = 1;
    edge_weight_opt = options.edge_weight;
end

if (check)
    % check the values
    if options.unweighted ~= 1 && edge_weights ~= 1
        check_matlab_bgl(A,struct('values',1,'noneg',1));
    else
        check_matlab_bgl(A,struct());
    end
    if edge_weights && any(edge_weights < 0)
        error('matlab_bgl:invalidParameter', ...
                'the edge_weight array must be non-negative');
    end
end

if trans
    A = A';
end

weight_arg = options.unweighted;
if ~weight_arg
    weight_arg = edge_weight_opt;
else
    weight_arg = 0;
end
if nargout > 1
    [bc,ec] = betweenness_centrality_mex(A,weight_arg);
    
    [i j] = find(A);
    if ~trans
        temp = i;
        i = j;
        j = temp;
    end
    
    if options.ec_list
        E = [j i ec];
    else
        E = sparse(j,i,ec,size(A,1),size(A,1));
    end
    
else
    bc = betweenness_centrality_mex(A,weight_arg);
end