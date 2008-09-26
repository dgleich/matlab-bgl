function [X,extra] = kamada_kawai_spring_layout(A,varargin)
% X = kamada_kawai_spring_layout
% [X,data] returns extra data, such as the spring matrix and 
%   data.spring_strength % returns the spring stremgth
%   data.distance % returns the distance between all points
%
%   options.tol: stopping tolerance of layout change [double | {0.001}]
%   options.spring_constant: energy of the system [double | {1}]
%   options.progressive: [{0} | position matrix X]
%   options.edge_length: default length of an edge [double | {1}]
%   options.edge_weight

% David F. Gleich
% Copyright, Stanford University, 2008

%% History
%  2008-09-25: Initial coding
%%

