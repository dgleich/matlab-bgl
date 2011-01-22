%% A Flow Refinement Idea 
% Reid Andersen and Kevin Lang propose an algorithm to take a graph
% partition and improve it using a sequence of max-flow problems in the
% paper "An Algorithm for Improving Graph Partitions."  Proceedings of the 
% 19th annual ACM-SIAM Symposium on Discrete Algorithms (SODA2008), 2008, 
% pp. 651-660.  We can quickly implement and test their idea using the
% max-flow routine in MatlabBGL.  

%% Adjust the figures to be slightly nicer
set(0,'defaultlinelinewidth',.5);
rand('state',0); % ensure we are looking at the same output!

%% The code
% The implementation follows the notation of |Improve| algorithm in the 
% paper closely.  
type flow_improve.m

%% Random geometric networks
% In the presentation, Reid and Kevin showed many plots of their algorithm
% improving a partitions in a geometric graph.  We can dynamically generate
% these examples in Matlab using a simple wrapper around the Delauney
% triangulation routines.
type randgeom.m

%%
% Generate a plot a random square graph.  This positions points randomly in
% the square and then 
[G,xy] = randgeom(500); % G is the adjacency matrix, xy are the coordinates
gplot(G,xy,'k.-'); axis tight; axis square; axis off;

%%
% The improve algorithms takes an initial partition and improves the
% quality of the cut.  Let's generate a really crazy partition of this
% graph.  We will bisect the graph based on its geometric coordinates to
% split it precisely in the middle.  We'll draw points on one side of the
% partition with a red color.
A = xy(:,1)>0.5;
gplot(G,xy,'k.-');  axis tight; axis square; axis off;
hold on; plot(xy(A,1), xy(A,2),'r.'); hold off;

%%
% Unfortunately, this isn't a particularly good partition because there are
% some really simple problems with the local density of the random points.
% The |flow_improve| routine fixes these issues.
S = flow_improve(G,A,[]);

%%
% This produces a new partition S.  The output indicates that we went from
% cutting 100 edges to cutting 96 and the balance changed from exactly 250
% vertices on each side, to 249 on one side.  Not bad!  Let'see the
% partition.

gplot(G,xy,'k.-');  axis tight; axis square; axis off;
hold on; plot(xy(S,1), xy(S,2),'r.'); hold off;

%% 
% It wasn't a huge change, but a few points moved.

%%
% Let's look at the same thing for random points in the circle.  First, we
% have to construct a function which generates random points in a circle.

polar = @(r,t) [r.*cos(t) r.*sin(t)];
randcirc = @(n,z) polar(sqrt(rand(n,1)), 2*pi*rand(n,1));

%%
% The |randgeom| routine uses this routine to give us a geometric graph,
% which we split horizontally (initially)
[G,xy] = randgeom(5000,randcirc);
A = xy(:,2)>0;
gplot(G,xy,'k.-');  axis tight; axis square; axis off;
hold on; plot(xy(A,1), xy(A,2),'r.'); hold off;

%%
% Again, let's use flow improvement.
S = flow_improve(G,A,[]);
gplot(G,xy,'k.-');  axis tight; axis square; axis off;
hold on; plot(xy(S,1), xy(S,2),'r.'); hold off;

%% 
% There were just some slight changes, but we improved the cut
% considerably: from 356 cut edges in the original to 296 in the improvement.


%% The US Road Network
% We'll examine these algorithms with the continential US road network.
% This example will model a real world "bigger" graph.

G = readSMAT('/home/dgleich/data/usroads/usroads-cc.smat');
xy = load('/home/dgleich/data/usroads/usroads-cc.latlong');
fprintf('verts = %7i\n', size(G,1));
fprintf('edges = %7i\n', nnz(G));

%%
% A 
ms = 3; % set the marker size to 3 pt
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);  axis tight; axis off; 
print(gcf,'-depsc2','untitled.eps');

%%
% Bisect the US horizontally.  We plot one side of the partition with
% red dots.  This command bisects the US at the 35th latitude.
A = xy(:,2)>35;
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);  axis tight; axis off; 
hold on; plot(xy(A,1), xy(A,2),'r.','MarkerSize',ms); hold off;

%%
% Using the flow_improve scheme, we improve the partition so that it 
% tracks along the Chesepeak bay
S = flow_improve(G,A,[]);
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);  axis tight; axis off; 
hold on; plot(xy(S,1), xy(S,2),'r.','MarkerSize',ms); hold off;

%%
% Bisect the US vertically
A = xy(:,1)>-90; 
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);  axis tight; axis off; 
hold on; plot(xy(A,1), xy(A,2),'r.','MarkerSize',ms); hold off;

%%
% Improve the partition
S = flow_improve(G,A,[]);
[px,py]=gplot(G,xy); plot(px,py,'k.-','MarkerSize',ms);  axis tight; axis off; 
hold on; plot(xy(S,1), xy(S,2),'r.','MarkerSize',ms); hold off;
