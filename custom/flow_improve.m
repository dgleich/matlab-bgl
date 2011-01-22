function [S,q] = flow_improve(G,A,p,maxiter)
% FLOW_IMPROVE Improve an existing clustering using max-flow
%   G must be a symmetric graph
%   A is an indicator over the vertices of G
%   p a non-negative integer vertex weight vector (default ones(n,1))
%   maxiter the maximum number of times to run (default 5)

%% History
%  2008-10-20: Added partition list input.

n = size(G,1);

if ~exist('maxiter','var') || isempty(maxiter), maxiter=5; end
if ~exist('p','var'), p='cond'; end

if ~islogical(A), lA = false(n,1); lA(A)=1; A=lA; end % convert A to logical

% compute p
if isempty(p), p = ones(n,1); end
if ischar(p) && strfind(p,'cond'), p = full(sum(G,2)); end

[ei ej ev] = find(G);
L = diag(sum(G,2))-G; % compute the laplacian
from_s = find(A); pA = p(A);
to_t = find(~A); pB = p(~A);

infval = 2^29;

piA = sum(pA);
piB = sum(pB);

iter= 1;
S= A;
% compute alpha = Q(A)
v= zeros(n,1);
v(S) = 1;
v(~S) = -1;
cv = v'*(L*v)/4;
bal = min(piA,piB);
alpha = cv/min(piA,piB);  
q = alpha;
fprintf('Initial cut : cut= %7i bal= %7i quot= %8.5f\n', ...
    cv, bal, q);
q_old = q; S_old = S;
while iter<=maxiter
    % compute G_A(alpha)
    scale = ceil(1/alpha);
    from_s_v = alpha*pA*scale;
    to_t_v = alpha*pB*piA/piB*scale;
    GA = sparse([ei; (n+1)*ones(length(from_s),1); to_t], ...
                [ej; from_s; (n+2)*ones(length(to_t),1)], ...
                [ev*scale; ceil(from_s_v); ceil(to_t_v)], n+2, n+2);
   
    [flowval,cut] = push_relabel_max_flow(GA,n+1,n+2);
    S = cut(1:n)==1;
    DAofS = sum(p(S&A)) - sum(p(S&~A))*piA/piB;
    if DAofS > 0, 
        v= zeros(n,1);
        v(S) = 1;
        v(~S) = -1;
        cv = v'*(L*v)/4;
        QAofS = cv/DAofS;
    else
        QAofS = infval;
    end
    bal = min(sum(p(S)), sum(p(~S)));
    q = cv/bal;
    fprintf('   iter %3i : cut= %7i bal= %7i q= %8.5f quot(A)= %8.5f DA= %7f\n', ...
        iter, cv, bal, q, QAofS, DAofS);
    if QAofS>=alpha, S = S_old; q = q_old; break; end
    alpha = QAofS;
    q_old = q; S_old = S;
end

