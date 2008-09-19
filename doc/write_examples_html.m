publish('../examples/red_black.m', struct('format','html','stylesheet',fullfile(pwd,'../doc/','mxdom2mbgl-html.xsl'), 'outputDir', fullfile(pwd,'html/red_black/')));
publish('../examples/record_alg.m', struct('format','html','stylesheet',fullfile(pwd,'../doc/','mxdom2mbgl-html.xsl'), 'outputDir', fullfile(pwd,'html/record_alg/')));
publish('../examples/core_numbers_example.m', struct('format','html','stylesheet',fullfile(pwd,'../doc/','mxdom2mbgl-html.xsl'), 'outputDir', fullfile(pwd,'html/core_numbers_example/'), 'figureSnapMethod','getframe'));
publish('../examples/reweighted_graphs.m', struct('format','html','stylesheet',fullfile(pwd,'../doc/','mxdom2mbgl-html.xsl'), 'outputDir', fullfile(pwd,'html/reweighted_graphs/')));
