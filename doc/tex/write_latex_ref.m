% A script to write documentation on all the m file functions

to_list = ...
    {{'Searches', 'bfs', 'dfs', 'breadth_first_search', 'depth_first_search'}, ...
     {'Components', 'components', 'biconnected_components'}, ...
     {'Shortest Paths', 'shortest_paths', 'all_shortest_paths', 'dijkstra_sp', 'bellman_ford_sp', 'dag_sp', 'johnson_all_sp', 'floyd_warshall_all_sp', 'astar_search'}, ...
     {'Minimum Spanning Trees', 'mst', 'kruskal_mst', 'prim_mst'}, ...
     {'Statistics', 'num_edges', 'num_vertices', 'betweenness_centrality', 'clustering_coefficients', 'core_numbers', 'lengauer_tarjan_dominator_tree', 'topological_order', 'test_dag'}, ...
     {'Flow', 'max_flow', 'edmunds_max_flow','kolmogorov_max_flow','push_relabel_max_flow'}, ...
     {'Matchings', 'matching', 'edmonds_maximum_cardinality_matching', 'maximal_matching', 'test_matching'}, ...
     {'Graphs', 'cycle_graph', 'erdos_reyni', 'grid_graph', 'star_graph', 'wheel_graph'}, ...
     {'Visitors', 'combine_visitors'}, ...
     {'Utilities', 'edge_weight_index', 'indexed_sparse', 'path_from_pred', 'tree_from_pred'}, ...
     {'Options', 'set_matlab_bgl_default'}};
 
fid = fopen('matlab-bgl-tex-ref-3.0.tex','w');     

for (ii=1:length(to_list))
    subsec = to_list{ii};
    if (ii > 1)
        fprintf(fid,'\\newpage\n');
    end;
    fprintf(fid,'\\hrule\n');
    fprintf(fid,'\\subsection*{%s}\n', subsec{1});
    fprintf(fid,'\\vspace{1cm}\n');
    fprintf(fid,'\\hrule\n');
    section_start = 1;
    for (jj=2:length(subsec))
        funcname = subsec{jj};
        funcname = strrep(funcname, 'matlab_bgl/', '');
        funcname = strrep(funcname, '_', '\_');
        
        if (~section_start) 
            fprintf(fid,'\\newpage\n');
        end;
        fprintf(fid,'\\subsubsection*{%s}\n', funcname);
        helpstr = help(subsec{jj});
        fprintf(fid,'\\begin{mcode}\n%s\\end{mcode}\n',helpstr);
        
        section_start = 0;
    end;
end;

fclose(fid);