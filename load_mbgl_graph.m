function varargout=load_mbgl_graph(graphname)
% LOAD_MBGL_GRAPH Loads a graph from the matlab-bgl library
%
% load_mbgl_graph is a helper function to load a graph provided with the
% library regardless of the current working directory.  
%
% If it's called without any output arguments, it functions just like a
% load command executed on the .mat file with the graph.  If it's called
% with an output arguemnt, it functions just like a load command with
% output arguments.  It's somewhat complicated to explain because this is
% just a convinence function to make the examples work for any path, and
% not just from the gaimc root directory.
%
% Example:
%   % same as load('graphs/airports.mat') run from the matlab-bgl directory
%   load_mbgl_graph('tapir') 
%   gplot(A,xy,'.-');
%   % same as P=load('graphs/kt-7-2.mat') run from the matlab-bgl directory
%   P=load_mbgl_graph('kt-7-2.mat') 
%   % so you don't have to put the path in for examples!

% David F. Gleich
% Copyright, Stanford University, 2008-2009
% Copyright, Sandia Corporation, 2011

% History
% 2009-04-27: Initial coding
% 2011-02-01: port to matlab-bgl from gaimc


path=fileparts(mfilename('fullpath'));
if nargout==0
    evalin('caller',['load(''' fullfile(path,'graphs',graphname) ''');']);
else
    P = load(fullfile(path,'graphs',graphname));
    varargout{1} = P;
end
    
