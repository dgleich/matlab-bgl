G = readSMAT('/home/dgleich/data/usroads/usroads-cc.smat');
xy = load('/home/dgleich/data/usroads/usroads-cc.latlong');

gplot(G,xy);

%%
% bisect the US horizontally
A = xy(:,2)>35;
gplot(G,xy); hold on; plot(xy(A,1), xy(A,2),'r.'); hold off;

%%
S = flow_improve(G,A,[]);
gplot(G,xy); hold on; plot(xy(S,1), xy(S,2),'r.'); hold off;

%%
% bisect the US vertically
A = xy(:,1)>-90; 
gplot(G,xy); hold on; plot(xy(A,1), xy(A,2),'r.'); hold off;

%%
S = flow_improve(G,A,[]);
gplot(G,xy); hold on; plot(xy(S,1), xy(S,2),'r.'); hold off;


%%
load('../graphs/minnesota.mat');
G = A;
gplot(G,xy);

%%
A = xy(:,2)<46;
gplot(G,xy); hold on; plot(xy(A,1), xy(A,2),'r.'); hold off;

%%
S = flow_improve(G,A);
gplot(G,xy); hold on; plot(xy(S,1), xy(S,2),'r.'); hold off;
