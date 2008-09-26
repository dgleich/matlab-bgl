function [X,data] = kamada_kawai_spring_layout(A,varargin)
% KAMADA_KAWAI_SPRING_LAYOUT Use springs to compute a graph layout
% 
% See
% http://www.boost.org/doc/libs/1_36_0/libs/graph/doc/kamada_kawai_spring_layout.html
% for information about the layout function and the parameters
%
% [X,data] returns extra data, such as the spring matrix and 
%   data.spring_strength % returns the spring stremgth
%   data.distances % returns the distance between all points
%
%   options.tol: stopping tolerance of layout change [double | {0.001}]
%   options.spring_constant: energy of the system [double | {1}]
%   options.progressive: [{0} | position matrix X]
%   options.edge_length: default length of an edge [double | {1}]
%   options.edge_weight: a double array over the edges with an edge
%       weight for each edge, see EDGE_INDEX and EXAMPLES/REWEIGHTED_GRAPHS
%       for information on how to use this option correctly
%       [{'matrix'} | 'none' | length(nnz(A)) double vector]
%
% Example:
%   G = grid_graph(6,5);
%   X = kamada_kawai_spring_layout(G);
%   

% David F. Gleich
% Copyright, Stanford University, 2008

%% History
%  2008-09-25: Initial coding
%%

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A), A = sparse(A); end

n = num_vertices(A);
options = struct('tol',0.001,'spring_constant',1,'progressive',false,...
    'edge_length',1,'edge_weight','matrix');
options = merge_options(options,varargin{:});

% edge_weights is an indicator that is 1 if we are using edge_weights
% passed on the command line or 0 if we are using the matrix, or -1 to use
% nothing
edge_weights = 0;
edge_weight_opt = 'matrix';

if strcmp(options.edge_weight, 'matrix'), % do nothing to use the matrix weights
elseif strcmp(options.edge_weight,'none'), edge_weights = -1; % don't use weights
else edge_weights = 1; edge_weight_opt = options.edge_weight;
end

if check
    % make sure the matrix is symmetric
    if ~edge_weights==1
        check_matlab_bgl(A,struct('sym',1,'values',edge_weights==0,'noneg',1));
    else
        if nnz(A) ~= length(edge_weight_opt)
            error('matlab_bgl:invalidParameter', ...
             'the vector of edge weights must have length nnz(A)'); end
        if any(edge_weight_opt)<0, error('matlab_bgl:invalidParameter',...
                'the edge_weight array must be non-negative'); end
        [i j] = find(A);
        Av = sparse(i,j,edge_weight_opt,size(A,1),size(A,2));
        check_matlab_bgl(Av,struct('sym',1,'noneg',1));
    end
end

progessive_opt = [];
if ~isscalar(options.progressive), progressive_opt = options.progressive; end

[X,spring,distance]=kamada_kawai_spring_layout_mex(...
    A, options.tol, options.spring_constant, progessive_opt, ...
    options.edge_length, edge_weights, edge_weight_opt);

if nargout>1, data.spring_strength = spring; data.distances = distance; end
