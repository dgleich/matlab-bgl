function is_kgraph = is_kuratowski_graph(A,varargin)

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A), A = sparse(A); end

if check, check_matlab_bgl(A,struct('sym',1)); end

is_kgraph = planar_test_mex(A,1);