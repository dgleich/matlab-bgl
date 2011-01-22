function [S,xy] = randgeom(nverts,randfunc)
% RANDGEOM Produce a random geometric graph in the plane
%
% 
if ~exist('randfunc','var') || isempty(randfunc), randfunc='rand'; end
nxy = nverts;
xy = feval(randfunc, nxy, 2);
tri = delaunay(xy(:,1),xy(:,2));
S = sparse(tri(:,[1 1 2 2 3 3]),tri(:,[2 3 1 3 1 2]),1,nxy, nxy);
