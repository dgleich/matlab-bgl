function [trans check full2sparse] = get_matlab_bgl_options(options)
% 
% Internal private function.
%
% Example:
%    Don't use this function!
%

doptions = set_matlab_bgl_default();
if (nargin>0)
    options = merge_structs(options, doptions);
else
    options = doptions;
end;

trans = ~options.istrans;
check = ~options.nocheck;
full2sparse = options.full2sparse;

