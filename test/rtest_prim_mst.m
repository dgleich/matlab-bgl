function rval=rtest_prim_mst()

A = [  0   1   1;
       1   0   1;
       1   1 0.1];
[i j v] = prim_mst(sparse(A));   

if length(i) == 1, rval= 0; % failure
else rval= 1; % success!
end

