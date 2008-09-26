function options = merge_options(default_options,varargin)
% MERGE_OPTIONS Merge a set of default options with options from varargin
% The set of options in varargin can be a list of key,value pairs, or a
% struct with the same information.

% David F. Gleich
% Copyright, Stanford University, 2008

%% History
%  2008-09-25: Initial coding
%%

if len(varargin)>0 && mod(len(varargin),2) == 0
    options = merge_structs(struct(varargin{:}),default_options);
elseif len(varargin)==1
    options = merge_structs(varargin{1},default_options);
elseif len(varargin) > 0
    error('matlag_bgl:optionsParsing',...
        'There were an odd number of key-value pairs of options specified');
else
    options = default_options;
end