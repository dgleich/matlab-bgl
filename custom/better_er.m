function A=better_er(n,p,issym)
edges = [];
for i=1:n
 flips=rand(1,n);
 links = find(flips<=p);
 nlinks = length(links);
 edges = [edges; i*ones(nlinks,1) links(:)];
end
A = sparse(edges(:,1),edges(:,2),1,n,n);
if issym
 A = triu(A,1);
 A = A + A';
end