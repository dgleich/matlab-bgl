function [varargout]=edmunds_karp_max_flow(A,u,v,options)
% EDMUNDS_KARP_MAX_FLOW Edmunds-Karp max flow algorithm
%
% The Edmunds-Karp algorithm implements the augmenting path idea
% to compute a maximum flow or minimum cut in a network.  
%
% See the max_flow function for calling information and return parameters.
% This function just calls max_flow(...,struct('algname','edmunds_karp'));
% The complexity is O(VE^2) or O(VEU) where U is the maximum capacity.
%
% Example:
%   load('graphs/max_flow_example.mat');
%   edmunds_karp_max_flow(A,1,8)

%
% 7 July 2008
% Initial version
%

if (nargin > 1)
    options.algname = 'edmunds_karp';
else
    options = struct('algname','edmunds_karp');
end;

varargout = cell(1,max(nargout,1));

[varargout{:}] = max_flow(A,u,v,options);