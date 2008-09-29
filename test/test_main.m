function test_main
%% Implement a test suite for matlab_bgl
% Call this function to test the MatlabBGL package.

%% Setup some parameters

msgid = 'matlab_bgl:test_main';

%% Test functions on empty or trivial input
% The goal of these tests is to make sure we get error messages or 
% reasonable output (i.e. it doesn't crash) on somewhat malformed or
% trivial input.

% test functions on empty matrices
try
    d = bfs(sparse([]),0);
    error(msgid, 'bfs did not report error');
catch 
end

try
    d = dfs(sparse([]),0);
    error(msgid, 'dfs did not report error');
catch 
end

try
    d = astar_search(sparse([]),0,@(x) x);
    error(msgid, 'astar_search did not report error');
catch 
end

try
    d = shortest_paths(sparse([]), 0);
    error(msgid, 'shortest_paths did not report error');
catch 
end

try
    d = bellman_ford_sp(sparse([]), 0);
    error(msgid, 'bellman_ford_sp did not report error');
catch 
end

try
    d = dag_sp(sparse([]), 0);
    error(msgid, 'dag_sp did not report error');
catch 
end

try
    f = max_flow(sparse([]),0,0);
    error(msgid, 'max_flow did not report error');
catch 
end

try
    p = dominator_tree(sparse([]),0);
    error(msgid, 'lengauer_tarjan_dominator_tree did not report error');
catch 
end

D = johnson_all_sp(sparse([]));
D = all_shortest_paths(sparse([]));
D = floyd_warshall_all_sp(sparse([]));
T = mst(sparse([]));
T = kruskal_mst(sparse([]));
T = prim_mst(sparse([]));
cc = components(sparse([]));
bcs = biconnected_components(sparse([]));
c = betweenness_centrality(sparse([]));
c = clustering_coefficients(sparse([]));
ei = edge_weight_index(sparse([]));
m = matching(sparse([]));
m = core_numbers(sparse([]));
v = edge_weight_vector(sparse([]),sparse([]));

X = circle_graph_layout(sparse([]));
X = random_graph_layout(sparse([]));
X = kamada_kawai_spring_layout(sparse([]));
X = fruchterman_reingold_force_directed_layout(sparse([]));
X = gursoy_atun_layout(sparse([]));

%% Code examples

% all_shortest_paths
load('../graphs/clr-26-1.mat');
all_shortest_paths(A);
all_shortest_paths(A,struct('algname','johnson'));

[D P]=all_shortest_paths(A,struct('algname','floyd_warshall'));
i=4; j=1; p=[]; while j~=0, p(end+1)=j; j=P(i,j); end; p=fliplr(p);

% astar_search
load('../graphs/bgl_cities.mat');
goal = 11; % Binghamton
start = 9; % Buffalo
% Use the euclidean distance to the goal as the heuristic
h = @(u) norm(xy(u,:) - xy(goal,:));
% Setup a routine to stop when we find the goal
ev = @(u) (u ~= goal);
[d pred f] = astar_search(A, start, h, ...
    struct('visitor', struct('examine_vertex', ev)));

% bellman_ford_sp
load('../graphs/kt-6-23.mat');
d = bellman_ford_sp(A,1);

% betweenness_centrality
load('../graphs/padgett-florentine.mat');
betweenness_centrality(A);

% bfs
load('../graphs/bfs_example.mat');
d = bfs(A,1);

% biconnected_components
load('../graphs/tarjan-biconn.mat');
biconnected_components(A);

% breadth_first_search
% see (dist_uv_bfs below)    
load('../graphs/bfs_example.mat');
d2 = dist_uv_bfs(A,1,3);

% circle_graph_layout
G = cycle_graph(6);
X = circle_graph_layout(G);
gplot(G,X);

% clustering_coefficients
load('../graphs/clique-10.mat');
clustering_coefficients(A);

% combine_visitors
vis1 = struct();
vis1.examine_vertex = @(u) fprintf('vis1: examine_vertex(%i)\n', u);
vis2 = struct();
vis2.examine_vertex = @(u) fprintf('vis2: examine_vertex(%i)\n', u);
combined_vis = combine_visitors(vis1, vis2);
load('../graphs/bfs_example.mat');
breadth_first_search(A,1,combined_vis);

% components
load('../graphs/dfs_example.mat');
components(A);
     
% core_numbers
load('../graphs/cores_example.mat');
cn = core_numbers(A);

% cycle_graph
[A xy] = cycle_graph(10);
gplot(A,xy);

% dag_sp
load('../graphs/kt-3-7.mat');
dag_sp(A,1);

% depth_first_search
load('../graphs/dfs_example.mat');
dist_uv_dfs(A,1,3);

% dfs
load('../graphs/dfs_example.mat');
d = dfs(A,1);

% dijkstra_sp
load('../graphs/clr-25-2.mat')
[d pred] = dijkstra_sp(A,1);

% edge_weight_index
load('../graphs/bfs_example.mat');
[eil Ei] = edge_weight_index(A,struct('undirected',1));
edge_rand = rand(num_edges(A)/2,1);
[iu ju] = find(triu(A,0));
Av = sparse(iu,ju,edge_rand,size(A,1),size(A,1)); Av = Av + Av';
ee = @(ei,u,v) fprintf('examine_edge %2i, %1i, %1i, %4f, %4f, %4f\n', ...
            ei, u, v, edge_rand(eil(ei)), Av(u,v), edge_rand(Ei(u,v)));
breadth_first_search(A,1,struct('examine_edge', ee));

% edge weight vector
n = 8; u = 1; v = 2;
E = [1:n 2:n 1; 2:n 1 1:n]';
w = [1 zeros(1,n-1) 1 zeros(1,n-1)]';
A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
[d pred] = shortest_paths(As,u,struct('edge_weight',edge_weight_vector(As,A)));

% edmonds_maximum_cardinality_matching
load('../graphs/matching_example.mat');
m=edmonds_maximum_cardinality_matching(A) ;
sum(m>0)/2;                % matching cardinality, should be 8

% edmunds_karp_max_flow
load('../graphs/max_flow_example.mat');
f=edmunds_karp_max_flow(A,1,8);
    
% erdos_reyni
A = erdos_reyni(100,0.05);

% floyd_warshall_all_sp
load('../graphs/clr-26-1.mat');
D=floyd_warshall_all_sp(A);

% fruchterman_reingold_force_directed_layout
G = grid_graph(6,5);
X = fruchterman_reingold_force_directed_layout(G);
gplot(G,X);

% grid_graph
[A xy] = grid_graph(5,10);
gplot(A,xy);
A = grid_graph(2*ones(1,10)); % compute 10d hypercube

% gursoy_atun_layout
G1 = cycle_graph(5000,struct('directed',0));
X1 = gursoy_atun_layout(G1,'topology','heart');
G2 = grid_graph(50,50);
X2 = gursoy_atun_layout(G2,'topology','square');
G3 = grid_graph(50,50);
X3 = gursoy_atun_layout(G3,'topology','circle');
subplot(1,3,1); gplot(G1,X1,'k'); subplot(1,3,2); gplot(G2,X2,'k');
subplot(1,3,3); gplot(G3,X3,'k');
     
% indexed_sparse

% johnson_all_sp
load('../graphs/clr-26-1.mat');
D=johnson_all_sp(A);

% kamada_kawai_spring_layout
G = grid_graph(6,5);
X = kamada_kawai_spring_layout(G);
gplot(G,X);

% kolmogorov_max_flow
load('../graphs/max_flow_example.mat');
kolmogorov_max_flow(A,1,8);

% kruskal_mst
load('../graphs/clr-24-1.mat');
T=kruskal_mst(A);
     
% lengauer_tarjan_dominator_tree
load('../graphs/dominator_tree_example.mat');
p = lengauer_tarjan_dominator_tree(A,1);

% matching
load('../graphs/matching_example.mat');
[m,v] = matching(A);
[m,v] = matching(A,struct('augmenting_path','none'));

% matching
load('../graphs/matching_example.mat');
m = matching(A); 
sum(m>0)/2;                % matching cardinality, should be 8
[m,v] = matching(A,struct('augmenting_path','none')); % not maximum matching

% max_flow
load('../graphs/max_flow_example.mat');
f=max_flow(A,1,8);

% maximal_matching
load('../graphs/matching_example.mat');
m=maximal_matching(A);
sum(m>0)/2;                % maximal matching cardinality, should be < 8
mmax=matching(A);
sum(mmax>0)/2;             % maximum matching cardinality, should be 8 

% mst
load('../graphs/clr-24-1.mat');
T=mst(A);

% num_edges
load('../graphs/dfs_example.mat');
n = num_edges(A);

% num_vertices
A = sparse(ones(5));
n = num_vertices(A);

% path_from_pred
load('../graphs/bfs_example.mat');
[d dt pred] = bfs(A,1,struct('target', 3));
path = path_from_pred(pred,3); % sequence of vertices to vertex 3

% prim_mst
load('../graphs/clr-24-1.mat');
prim_mst(A);
T = prim_mst(A,struct('root',5)); % root the tree at vertex e

% push_relabel_max_flow
load('../graphs/max_flow_example.mat');
f=push_relabel_max_flow(A,1,8);

% random_graph_layout
G = cycle_graph(1500);
X = random_graph_layout(G);
gplot(G,X); hold on; plot(X(:,1),X(:,2),'r.'); hold off;
% Layout on the grid
X = random_graph_layout(G,int32([0 0 5 5])); % random grid layout
gplot(G,X); grid on; hold on; plot(X(:,1),X(:,2),'r.'); hold off;


% shoretst_paths
load('../graphs/clr-25-2.mat');
shortest_paths(A,1);
shortest_paths(A,1,struct('algname','bellman_ford'));

% star_graph
[A xy] = star_graph(10);
gplot(A,xy);

% test_dag
n = 10; A = sparse(1:n-1, 2:n, 1, n, n); % construct a simple dag
test_dag(A);
A(10,1) = 1; % complete the cycle
test_dag(A);

% test_matching
load('../graphs/matching_example.mat');
[m_not_max,v] = matching(A,struct('augmenting_path','none'));
test_matching(A,m_not_max);

% toplogical_order
load('../graphs/bfs_example.mat');
d = bfs(A,1);

% tree_from_pred
load('../graphs/dominator_tree_example.mat');
p = lengauer_tarjan_dominator_tree(A,1);
T = tree_from_pred(p);

% wheel graph
[A xy] = wheel_graph(10);
gplot(A,xy);
n = 10;
A = cycle_graph(n);
[d dt ft pred] = dfs(A,1,struct('target',3));

%% all_shortest_paths
Dtrue = [0 1 -3 2 -4; 3 0 -4 1 -1; 7 4 0 5 3; 2 -1 -5 0 -2; 8 5 1 6 0];
load('../graphs/clr-26-1.mat');
D = all_shortest_paths(A);
if any(any(D ~= Dtrue))
    error(msgid, 'all_shortest_paths returned an incorrect distance matrix');
end
D = all_shortest_paths(A,struct('algname','johnson'));
if any(any(D ~= Dtrue))
    error(msgid, 'all_shortest_paths(johnson) returned an incorrect distance matrix');
end
D = all_shortest_paths(A,struct('algname','floyd_warshall'));
if any(any(D ~= Dtrue))
    error(msgid, 'all_shortest_paths(floyd_warshall) returned an incorrect distance matrix');
end
At = A';
D = all_shortest_paths(At,struct('istrans',1));
if any(any(D' ~= Dtrue))
    error(msgid, 'all_shortest_paths(istrans=1) returned an incorrect distance matrix');
end
% test non-reachable vertex
A = sparse([1 1; 0 1]);
Dtrue = [0 1; 5 0];
D = all_shortest_paths(A,struct('inf',5,'algname','johnson'));
if any(any(D ~= Dtrue))
    error(msgid, 'all_shortest_paths(johnson,inf=5) returned an incorrect distance matrix');
end
D = all_shortest_paths(A,struct('inf',5,'algname','floyd_warshall'));
if any(any(D ~= Dtrue))
    error(msgid, 'all_shortest_paths(floyd_warshall,inf=5) returned an incorrect distance matrix');
end
% test edge weighted graph
Dtrue = [0 1 -3 2 -4; 3 0 -4 1 -1; 7 4 0 5 3; 2 -1 -5 0 -2; 8 5 1 6 0];
load('../graphs/clr-26-1.mat');
D = all_shortest_paths(A,struct('edge_weight','matrix'));
if any(any(D ~= Dtrue))
    error(msgid, 'all_shortest_paths(weight=matrix) returned an incorrect distance matrix');
end
v=nonzeros(A');
D = all_shortest_paths(spones(A),struct('edge_weight',v));
if any(any(D ~= Dtrue))
    error(msgid, 'all_shortest_paths(weight=matrix) returned an incorrect distance matrix');
end
try
    bc = all_shortest_paths(A,struct('edge_weight',rand(2,1)));
    error(msgid, 'all_shortest_paths(weight=rand(2,1)) did not report an error');    
catch
end

% test predecessor matrix
[D P] = all_shortest_paths(A,struct('algname','floyd_warshall'));
for i=1:size(A,1)
    [d p] = shortest_paths(A,i);
    if any(D(i,:)~=d'), error(msgid,'all_shortest_paths(floyd_warshall) returned incorrect distance'); end
    if any(P(i,:)~=p), error(msgid,'all_shortest_paths(floyd_warshall) returned incorrect predecessor'); end    
    % the following command should always work.
    for j=1:size(A,1)
        p=[]; while j~=0, p(end+1)=j; j=P(i,j); end; p=fliplr(p);
    end
end

%% astar_search

%% betweenness_centrality

n = 10;
A = cycle_graph(n);

% the centrality index of all vertices in a cycle is the same
bc = betweenness_centrality(A,struct('unweighted',1));
if any(bc-bc(1))
    error(msgid, 'betweenness_centrality returned incorrect values for a cycle');
end

% make sure we toss an error when the graph has logical weights
try 
    bc = betweenness_centrality(A);
    error(msgid, 'betweenness_centrality did not report an error');
catch
end

% make sure the edge centrality graphs are the same in a few cases
A = sparse(1:n-1, 2:n, 1, n, n);
[bc,Ec] = betweenness_centrality(A);
if any(any(spones(A) - spones(Ec)))
    error(msgid, 'different non-zero structure in edge centrality matrix');
end

[bc,Ec] = betweenness_centrality(A,struct('istrans',1));
if any(any(spones(A) - spones(Ec)))
    error(msgid, 'different non-zero structure in edge centrality matrix');
end

% make sure betweenness centrality can use an optional edge weight matrix
bc = betweenness_centrality(A,struct('edge_weight',rand(nnz(A),1)));
bc = betweenness_centrality(A);
bc2 = betweenness_centrality(A,struct('edge_weight','matrix'));
if any(bc-bc2)
    error(msgid, 'edge_weight option error');
end
try
    bc = betweenness_centrality(A,struct('edge_weight',rand(2,1)));
    error(msgid, 'betweenness_centrality(weight=rand(2,1)) did not report an error');    
catch
end

%% biconnected_components
load('../graphs/tarjan-biconn.mat');
[a C] = biconnected_components(A);

%% bfs

%% breadth_first_search

%% circle_graph_layout
X = circle_graph_layout(sparse(4,4));
Y = [1 0; 0 1; -1 0; 0 -1];
if norm(X-Y)>5e-6, error(msgid, 'circle_graph_layout(4) bad coords'); 
end

%% clustering_coefficients

% Create a clique, where all the clustering coefficients are equal
A = sparse(ones(5));
ccfs = clustering_coefficients(A);
if any(ccfs ~= ccfs(1))
    error(msgid, 'clustering_coefficients failed');
end

%% core_numbers
load('../graphs/kt-7-2.mat');
A = spones(A);
cn = core_numbers(A);
load('../graphs/cores_example.mat');
cn = core_numbers(A);
cn2 = core_numbers(A,struct('unweighted',0));
if any(cn-cn2)
    error(msgid, 'core_numbers failed equivalence test');
end

A = [0 -1 -2; -1 0 -2; -2 -2 0];
cn = core_numbers(sparse(A),struct('unweighted',0));
if any(cn-[-1; -1; -4])
    error(msgid, 'core_numbers failed negative test');
end

%% cycle_graph
G = cycle_graph(10,struct('directed',0));
G = cycle_graph(10,'directed',0);
[A,xy] = cycle_graph(0); assert(all(size(A)==[0 0]));
[A,xy] = cycle_graph(1); assert(all(size(A)==[1 1])); assert(nnz(A)==1);
for i=0:10, [A,xy] = cycle_graph(i); end
for i=50:50:250, [A,xy] = cycle_graph(i); end

%% dfs

%% dominator_tree
load('../graphs/dominator_tree_example.mat');
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= [ 0, 1,  2,  2,  4,   5,  5,  2])
    error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end

% graphs from boost example's

A=sparse(13,13);
A(1,2)=1;A(1,3)=1;A(1,4)=1;A(2,5)=1;A(3,2)=1;A(3,5)=1;A(3,6)=1;A(4,7)=1;
A(4,8)=1;A(5,13)=1;A(6,9)=1;A(7,10)=1;A(8,10)=1;A(8,11)=1;A(9,6)=1;
A(9,12)=1;A(10,12)=1;A(11,10)=1;A(12,1)=1;A(12,10)=1;A(13,9)=1;
pred=[0 1 1 1 1 1 4 4 1 1 8 1 5 ];
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= pred)
   error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end

A=sparse(7,7);
A(1,2)=1;A(2,3)=1;A(2,4)=1;A(3,5)=1;A(3,6)=1;A(5,7)=1;A(6,7)=1;A(7,2)=1;
pred=[0 1 2 2 3 3 3 ];
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= pred)
   error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end

A=sparse(13,13);
A(1,2)=1;A(1,3)=1;A(2,4)=1;A(2,7)=1;A(3,5)=1;A(3,8)=1;A(4,6)=1;A(4,7)=1;
A(5,8)=1;A(5,3)=1;A(6,9)=1;A(6,11)=1;A(7,10)=1;A(8,13)=1;A(9,12)=1;
A(10,9)=1;A(11,12)=1;A(12,2)=1;A(12,13)=1;
pred=[0 1 1 2 3 4 2 3 2 7 6 2 1 ];
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= pred)
   error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end

A=sparse(8,8);
A(1,2)=1;A(2,3)=1;A(2,4)=1;A(3,8)=1;A(4,5)=1;A(5,6)=1;A(5,7)=1;A(6,8)=1;
A(7,5)=1;
pred=[0 1 2 2 4 5 5 2 ];
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= pred)
   error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end

A=sparse(8,8);
A(1,2)=1;A(2,3)=1;A(3,4)=1;A(3,5)=1;A(4,3)=1;A(5,6)=1;A(5,7)=1;A(6,8)=1;
A(7,8)=1;
pred=[0 1 2 3 3 5 5 5 ];
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= pred)
   error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end

A=sparse(8,8);
A(1,2)=1;A(1,3)=1;A(2,7)=1;A(3,4)=1;A(3,5)=1;A(4,8)=1;A(6,8)=1;A(7,8)=1;
pred=[0 1 1 3 3 0 2 1 ];
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= pred)
   error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end

A=sparse(14,14);
A(1,2)=1;A(1,14)=1;A(2,3)=1;A(3,4)=1;A(3,8)=1;A(4,5)=1;A(4,6)=1;A(5,7)=1;
A(6,7)=1;A(7,9)=1;A(8,9)=1;A(9,10)=1;A(10,11)=1;A(10,12)=1;A(11,12)=1;
A(12,10)=1;A(12,13)=1;A(13,3)=1;A(13,14)=1;
pred=[0 1 2 3 4 4 4 3 3 9 10 10 12 1 ];
p = lengauer_tarjan_dominator_tree(A,1);
if any(p ~= pred)
   error(msgid, 'lengauer_tarjan_dominator_tree failed test');
end



%% edmonds_maximum_cardinality_matching
load('../graphs/matching_example.mat');
[m,v] = edmonds_maximum_cardinality_matching(A);
if nnz(m)/2 ~= 8
    error(msgid, 'edmonds_maximum_cardinality_matching failed');
end

%% edge_weight_vector
n = 8; u = 1; v = 2;
E = [1:n 2:n 1; 2:n 1 1:n]';
w = [1 zeros(1,n-1) 1 zeros(1,n-1)]';
A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
[d pred] = shortest_paths(As,u,struct('edge_weight',edge_weight_vector(As,A)));
if d(v) ~= 0
    error(msgid, 'edge_weight_vector failed');
end

% remove the edge between node 1 and edge 8 to test non-symmetric in As
A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
As(1,8) = 0;
[d pred] = shortest_paths(As,u,struct('edge_weight',edge_weight_vector(As,A)));
if d(v) ~= 1 || any(d>1) 
    error(msgid, 'edge_weight_vector failed');
end

% make the weights non-symmetric to test non-symmetry in A
A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
A(1,2) = 2; 
A(1,8) = 3;
A(2,3) = 4;
[d pred] = shortest_paths(As,u,struct('edge_weight',edge_weight_vector(As,A)));
if d(v) ~= 2 || any(d(d>2)~=3)
    error(msgid, 'edge_weight_vector failed');
end

% test non-symmetric A and non-symmetric As
A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
As(1,8) = 0;
A(1,2) = 2; 
A(2,3) = 4;
[d pred] = shortest_paths(As,u,struct('edge_weight',edge_weight_vector(As,A)));
if d(v) ~= 2 || any(d(d>2)~=6)
    error(msgid, 'edge_weight_vector failed');
end

% make sure it works with pre-transposed matrices, repeat all the test
% cases

A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
[d pred] = shortest_paths(As',u,...
    struct('edge_weight',edge_weight_vector(As',A',struct('istrans',1)),...
        'istrans',1));
if d(v) ~= 0
    error(msgid, 'edge_weight_vector failed');
end

A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
As(1,8) = 0;
[d pred] = shortest_paths(As',u,...
    struct('edge_weight',edge_weight_vector(As',A',struct('istrans',1)),...
        'istrans',1));
if d(v) ~= 1
    error(msgid, 'edge_weight_vector failed');
end

A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
A(1,2) = 2; 
A(1,8) = 3;
A(2,3) = 4;
[d pred] = shortest_paths(As',u,...
    struct('edge_weight',edge_weight_vector(As',A',struct('istrans',1)),...
        'istrans',1));
if d(v) ~= 2 || any(d(d>2)~=3)
    error(msgid, 'edge_weight_vector failed');
end

A = sparse(E(:,1), E(:,2), w, n, n); % create weighted sparse matrix
As = sparse(E(:,1), E(:,2), true, n, n); % create structural sparse matrix
As(1,8) = 0;
A(1,2) = 2; 
A(2,3) = 4;
[d pred] = shortest_paths(As',u,...
    struct('edge_weight',edge_weight_vector(As',A',struct('istrans',1)),...
        'istrans',1));
if d(v) ~= 2 || any(d(d>2)~=6)
    error(msgid, 'edge_weight_vector failed');
end

%% fruchterman_reingold_force_directed_layout
G = cycle_graph(10,struct('directed',0));
X = fruchterman_reingold_force_directed_layout(G);
G = cycle_graph(10);
try
  G = fruchterman_reingold_force_directed_layout(G);
  error(msgid, 'fruchterman_reingold failed to throw on a directed graph');
catch end
G = grid_graph(5,6);
X = fruchterman_reingold_force_directed_layout(G);
for i=0:10, X = fruchterman_reingold_force_directed_layout(sparse(i,i)); end
G = cycle_graph(10,struct('directed',0));
X1 = fruchterman_reingold_force_directed_layout(G,...
  'progressive',circle_graph_layout(G));
X2 = fruchterman_reingold_force_directed_layout(G,...
  struct('progressive',circle_graph_layout(G)));
if norm(X1-X2,'fro')>1e-10, error(msgid, 'fruchterman_reingold options failed'); end

%% gursoy_atun_layout
check_layout = @(X) all(all(isfinite(X)));
for i=0:10, X=gursoy_atun_layout(sparse(i,i)); assert(check_layout(X)); end
for i=0:10, X=gursoy_atun_layout(grid_graph(i,i)); assert(check_layout(X)); end
for i=0:10, for j=2:10,
        X=gursoy_atun_layout(wheel_graph(i),'topology',sprintf('cube%i',j));
        assert(check_layout(X)); 
        X=gursoy_atun_layout(wheel_graph(i),'topology',sprintf('ball%i',j));
        assert(check_layout(X));         
end, end

try
    X=gursoy_atun_layout(sparse(10,10),'topology','ball');
    error(msgid,'gursoy_atun_layout did not throw on invalid topology(ball)');
catch end

try
    X=gursoy_atun_layout(sparse(10,10),'topology','cube');
    error(msgid,'gursoy_atun_layout did not throw on invalid topology(cube)');
catch end

%% kamada_kawai_spring_layout
% TODO: Expand these test cases
try
  X = kamada_kawai_spring_layout(sparse(2,2));
  error(msgid,'kamada_kawai_spring_layout didn''t fail on empty graph');
catch
end
X = kamada_kawai_spring_layout(grid_graph(1,2));
for i=0:10, X=kamada_kawai_spring_layout(grid_graph(i,i)); assert(check_layout(X)); end

%% matching
load('../graphs/dfs_example.mat');
try
    [m,v] = matching(A);
    error(msgid,'matching failed');
catch
end
load('../graphs/matching_example.mat');
[m,v] = matching(A);

%% max_flow

%% mst

% make sure mst('kruskal','root') fails
load('../graphs/clr-24-1.mat');

try
    T = mst(A,struct('algname','kruskal','root',5));
    error(msgid,'mst(kruskal,root) did not report an error');
catch
end

%% pred_from_path

% Create a line graph
n = 10;
A = sparse(1:n-1,2:n,1,n,n);
A = A+A';

% Compute BFS and test pred_from_path
[d dt pred] = bfs(A,1);
path = path_from_pred(pred,n);
if any(path ~= 1:n)
    error(msgid, 'path_from_pred failed');
end

%% prim_mst
load('../graphs/clr-24-1.mat');
% change the graph to make the result unique
A(2,3) = 9; A(3,2) = 9;
T = prim_mst(A); % root the tree at vertex 
Ttrueijv = [
     2     8     1     4     6     9     3     5     4     3     7     6     8     1     7     3
     1     1     2     3     3     3     4     4     5     6     6     7     7     8     8     9
     4     8     4     7     4     2     7     9     9     4     2     2     1     8     1     2 ];
Ttrue = sparse(Ttrueijv(1,:),  Ttrueijv(2,:), Ttrueijv(3,:), 9,9);
if nnz(T - Ttrue) ~= 0
    error(msgid, 'prim_mst failed');
end

% unfortunately, I can't deterministically check if the 
% rooted option works because which tree the algorithm
% will pick is implementation dependent and not guaranteed
load('../graphs/clr-24-1.mat');
% this example should work, however...
T1 = prim_mst(A); % root the tree at vertex 
T2 = prim_mst(A,struct('root',5));
T1T2diff = sparse(9,9); T1T2diff(2,3) = -8; T1T2diff(1,8) = 8;
if nnz(triu(T1-T2)-T1T2diff) ~= 0
    error(msgid, 'prim_mst failed rooted test');
end

%% random_graph_layout
rand('state',0);
X = random_graph_layout(sparse(4,4));
if ~isequal(size(X),[4,2])
  error(msgid,'random_graph_layout(4) wrong output size');
end
if any(any(X<0)) || any(any(X>1))
  error(msgid,'random_graph_layout(4) wrong output area');
end
X = random_graph_layout(sparse(1500,1500),[-1e5,-1e5,1e5,1e5]);
if ~isequal(size(X),[1500,2])
  error(msgid,'random_graph_layout(1500) wrong output size');
end
if any(any(X<-1e5)) || any(any(X>1e5))
  error(msgid,'random_graph_layout(1500) wrong output area');
end

X = random_graph_layout(sparse(256,256),int32([0 0 0 0]));
if ~all(all(X==0))
  error(msgid,'random_graph_layout(256) invalid output');
end
%% shortest_paths

%% star_graph
[A,xy] = star_graph(0); assert(all(size(A)==[0 0]));
[A,xy] = star_graph(1); assert(all(size(A)==[1 1])); assert(nnz(A)==0);
for i=0:10, [A,xy] = star_graph(i); end
for i=50:50:250, [A,xy] = star_graph(i); end

%% test_dag
% Test the dag_test function, which also tests topological order
A = sparse(6,6);
A(3,6) = 1;
A(1,2) = 1;
A(3,5) = 1;
A(1,4) = 1;
A(2,5) = 1;
A(5,4) = 1;

dag = test_dag(A);
if dag == 0
    error(msgid, 'A dag was not identified as a dag');
end

% Test something that isn't a dag
A = cycle_graph(n);

dag = test_dag(A);
if dag == 1
    error(msgid, 'A cycle was identified as a dag');
end

%% test_matching
load('../graphs/matching_example.mat');
[m_max,v] = matching(A);
[m_not_max,v] = matching(A,struct('augmenting_path','none'));
if ~test_matching(A,m_max) || test_matching(A,m_not_max)
    error(msgid, 'test_matching failed');
end

%% topological_order
% Test the topological order function
n = 10;
A = sparse(1:n-1, 2:n, 1, n, n);
p = topological_order(A);
if any(p - (1:n)')
    error(msgid, 'topological_order failed on simple case');
end

%% wheel_graph
[A,xy] = wheel_graph(0); assert(all(size(A)==[0 0]));
[A,xy] = wheel_graph(1); assert(all(size(A)==[1 1])); assert(nnz(A)==0);
for i=0:10, [A,xy] = wheel_graph(i); end
for i=50:50:250, [A,xy] = wheel_graph(i); end

%%
% ***** end test_main *****
end


%% Accessory functions

    function dv=dist_uv_bfs(A,u,v)
      vstar = v;
      dmap = ipdouble(zeros(size(A,1),1));
      function stop=on_tree_edge(ei,u,v)
        dmap(v) = dmap(u)+1;
        stop = (v ~= vstar);
      end
      breadth_first_search(A,u,struct('tree_edge',@on_tree_edge));
      dv = dmap(v);
    end
    
    function dist_uv_dfs(A,u,v)
      vstar = v;
      dmap = ipdouble(zeros(size(A,1),1));
      function stop=on_tree_edge(ei,u,v)
        dmap(v) = dmap(u)+1;
        stop = (v ~= vstar);
      end
      breadth_first_search(A,u,struct('tree_edge',@on_tree_edge));
    end
