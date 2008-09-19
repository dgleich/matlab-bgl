function [varargout]=edmonds_maximum_cardinality_matching(A,options)
% EDMONDS_MAXIMUM_CARDINALITY_MATCHING Compute a maximum cardinality matching
%
% Edmonds' maximum cardinality matching algorithm begins with an extra
% greedy maximal matching and extends it by finding augmenting paths.  The
% output is always verified and Matlab displays a warning if the matching
% is not maximum cardinality.  
%
% This method works on undirected graphs (symmetric matrices) and ignores
% edge weights.  
% The runtime is O(mn*alpha(m,n)).  The alpha function is the inverse
% Ackermann function and is <= 4 for all valid Matlab inputs.
%
% See the MATCHING function for calling information.  This function 
% just calls matching(...,struct('initial_match','extra_greedy',...
% 'augmenting_path','edmonds','verify',1));
%
% See also MATCHING, MAXIMAL_MATCHING, TEST_MATCHING
%
% Example:
%   load('graphs/matching_example.mat');
%   m=edmonds_maximum_cardinality_matching(A) 
%   sum(m>0)/2                % matching cardinality, should be 8

if (nargin > 1)
    options.augmenting_path = 'edmonds';
    options.initial_match = 'extra_greedy';
    options.verify = 1;
else
    options = struct('initial_match', 'extra_greedy', ...
        'augmenting_path', 'edmonds', ...
        'verify', 1);
end;

varargout = cell(1,max(nargout,1));

[varargout{:}] = matching(A,options);