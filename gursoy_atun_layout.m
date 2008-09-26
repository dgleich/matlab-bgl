function X = gursoy_atun_layout(G,varargin)
% GURSOY_ATUN_LAYOUT Layout a graph by uniformly distributing vertices
% 
% Instead of trying to optimize an objective function of forces or springs
% in the layout, the Gursoy-Atun layout distributes vertices uniformly
% within a topology subject to keeping vertices nearby their neighbors.
%
%   options.topology: the topology spaced used for the layout
%     [{'square'} | 'heart' | 'sphere' | 'ballN'* | 'cubeN'*]
%     * for the ball and cube topolgy, N can be replaced by any number, so 
%       ball3 is the same as the sphere, cube2 is the same as the square,
%       but cube3 is the true cube and cube4 is a hypercube.
%   options.iterations: the number of iterations [num_vertices(G) | integer]
%   options.diameter_range: The inital and final diameters when updating
%     [ {[sqrt(num_vertices(G)),1.0]} | [diameter_initial,diameter_final] ]
%     where all values are double.
%   options.learning_constant_range: The inital and final learning constants 
%     [ {[0.8,0.2]} | [learning_constant_initial,learning_constant_final] ]
%     where all values are double.
%   

n = num_vertices(G);
options = struct('topology','square','iterations',n,...
    'diameter_range',[sqrt(n) 1.0],'learning_constant_range',[0.8 0.2]);
options = merge_options(options,varargin{:});