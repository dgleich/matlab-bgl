function [is_planar ksubgraph EI]=boyer_myrvold_planarity_test(A,varargin)

[trans check full2sparse] = get_matlab_bgl_options(varargin{:});
if full2sparse && ~issparse(A), A = sparse(A); end

if check, check_matlab_bgl(A,struct('sym',1)); end

if nargout <= 1
    is_planar = planar_test_mex(A,0);
else 
    if nargout <= 2
        [is_planar ki kj] = planar_test_mex(A,0);
    else
        [is_planar ki kj eip eie] = planar_test_mex(A,0);
        EI = struct('vp',eip,'edge_order',eie);
    end
    if ~isempty(ki)
        ksubgraph = sparse(ki,kj,1,size(A,1),size(A,2));
    else
        ksubgraph = sparse([]);
    end
    ksubgraph = ksubgraph|ksubgraph'; % placed here to get the type right    
end