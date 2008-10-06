function is_sldrawing = is_straight_line_drawing(A,X,varargin)
%
%   options.fix_negative: automatically adjust negative layouts

% David Gleich
% Copyright, Stanford University, 2008

%% History
%  2008-10-05: Initial version
%%


[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A), A = sparse(A); end

options = struct('fix_negative',1);
options = merge_options(options,varargin{:});
if check
    check_matlab_bgl(A,struct('sym',1)); 
    if ~isequal(floor(X),X), warning('matlab_bgl:roundWarning',...
        'after rounding, the values in X changed, possible incorrect output');
    end
end

if check || options.fix_negative
    minX = min(min(X));
    maxX = max(max(X));
    if check && maxX-minX>10*size(A,1) && maxX-minX>1e7,
        warning('matlab_bgl:memoryWarning',...
            'is_straight_line_drawing uses a bucket sort that may run out of memory');
    end
    if options.fix_negative && minX < 0
        X = X-minX;
    end
end

is_sldrawing = planar_test_mex(A,2,X);