function [varargout]=kolmogorov_max_flow(A,u,v,options)
% KOLMOGOROV_MAX_FLOW Kolmogorov's max flow algorithm
%
% Kolmogorov's algorithm implements a variation on the augmenting path idea
% to compute a maximum flow or minimum cut in a network.  
%
% See the max_flow function for calling information and return parameters.
% This function just calls max_flow(...,struct('algname','kolmogorov'));
%
% Example:
%   load('graphs/max_flow_example.mat');
%   kolmogorov_max_flow(A,1,8)


%
% 7 July 2008
% Initial version
%

if (nargin > 1)
    options.algname = 'kolmogorov';
else
    options = struct('algname','kolmogorov');
end;

varargout = cell(1,max(nargout,1));

[varargout{:}] = max_flow(A,u,v,options);