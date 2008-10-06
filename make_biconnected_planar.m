function varargout= make_biconnected(A,varargin)

% David Gleich
% Copyright, Stanford University, 2008

%% History
%  2008-10-05: Initial version
%%


[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A), A = sparse(A); end

options = struct('');
options = merge_options(options,varargin{:});
if check
    check_matlab_bgl(A,struct('sym',1)); 
end

[i j]= planar_edges_mex(A,1,1,0);

if nargout < 2
    if ~isempty(i), 
        [ai aj] = find(A);
        varargout{1} = sparse([ai; i; j], [aj; j; i], 1, size(A,1), size(A,2));
    else
        varargout{1}= A;
    end
else
    varargout{1}= i;
    varargout{2}= j;
end