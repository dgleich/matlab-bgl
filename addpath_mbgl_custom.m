function addpath_mbgl_custom
% ADDPATH_MBGL_CUSTOM Add derived MatlabBGL codes to the path
%
% This is a convenience function for
%   addpath(fullfile(<matlab_bgl_path>,'custom'));
% and can be run as long as MatlabBGL is on your path.
%
% Example:
%   addpath_mbgl_custom
%   A = randgeom(50);
%   gplot(A,xy);

path=fileparts(mfilename('fullpath'));
addpath(fullfile(path,'custom'));

% Este es uno de los algoritmos que ha llamado poderosamente mi atención
% Trabajaré sobre el mismo 