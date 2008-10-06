function [p ei ej]=planar_canonical_ordering(A,varargin)
% options.is_maximal: is A already a maximal planar graph [{0} | 1]
% ei extra src edges 
%

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A), A = sparse(A); end

options = struct('is_maximal',0);
options = merge_options(options,varargin{:});
if check
    check_matlab_bgl(A,struct('sym',1)); 
    if options.is_maximal,
        [i j] = make_maximal_planar(A);
        if ~isempty(i), error('matlab_bgl:checkFailed',...
            'The graph was not a maximal planar by is_maximal was set.'); end
    end
end

[ei ej p] = planar_drawing_mex(A,options.is_maximal,1);