#!/usr/bin/env python

"""
edges2arrays.py

Spit out arrays for the C++ tests
"""

def append_weight(edges):
    return [(i,j,1.) for (i,j) in edges]
    
def graph_from_smat(filename):
    file = open(filename,'rt')
    header = file.readline();
    parts = header.split()
    nverts = int(parts[0])
    graph = [ {} for _ in xrange(nverts) ]
    for line in file:
        parts = line.split()
        i = int(parts[0])
        j = int(parts[1])
        graph[i][j] = 1
    return graph

def graph_from_edges(edges,sym=True):
    graph = []
    size = 0
    for (i,j,val) in edges:
        size = max(i+1,j+1,size)
        if len(graph) < size:
            graph.extend([{} for _ in xrange(size - len(graph))])
        graph[i][j] = val
        if sym:
            graph[j][i] = val
    return graph
                        
def print_arrays(graph):
  n = len(graph)
  rp = [0];
  aj = [];
  w = [];
  for i,v in enumerate(graph):
      for j,val in v.items():
          aj.append(j)
          w.append(val);
      rp.append(len(aj))
  print """
  const mbglIndex n=%i;
  mbglIndex rp[] = {%s};
  mbglIndex ci[] = {%s};
  double w[] = {%s};
  """%(n,",".join([str(i) for i in rp]),",".join([str(i) for i in aj]),
        ",".join([str(i) for i in w]))
  
print "For test_king_ordering"
edges = [(0,3), 
                     (0,5),  
                     (1,2),  
                     (1,4),  
                     (1,6),  
                     (1,9),  
                     (2,3),  
                     (2,4),  
                     (3,5),  
                     (3,8),  
                     (4,6),  
                     (5,6),  
                     (5,7),  
                     (6,7)]
print_arrays(graph_from_edges(append_weight(edges)))

print "For isomorphism 1"
print_arrays(graph_from_smat('Baltz-2005-isospectral-1.smat'))
print "For isomorphism 2"
print_arrays(graph_from_smat('Baltz-2005-isospectral-2.smat'))


print "For bipartite (true)"
edges = [(0, 1), (0, 4), (1, 2), (2, 6), (3, 4), (3, 8), (4, 5), (4, 7), (5, 6), 
      (6, 7), (7, 10), (8, 9), (9, 10)]
print_arrays(graph_from_edges(append_weight(edges)))    

print "For bipartite (false)"
edges = [(0, 1), (0, 4), (1, 2), (2, 6), (3, 4), (3, 8), (4, 5), (4, 7), (5, 6),
      (6, 7), (7, 9), (8, 9)]
print_arrays(graph_from_edges(append_weight(edges)))    
