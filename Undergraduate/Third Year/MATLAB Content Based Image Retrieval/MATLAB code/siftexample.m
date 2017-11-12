function siftexample
%SIFT Keypoint detection algorithm

warning off;
addpath(genpath('C:\Program Files\MATLAB\vlfeat-0.9.20\toolbox\'));

I = single(rgb2gray(imread('Kiki.jpg')));
figure;
imagesc(I);
colormap(gray);
axis image;
axis off;

[f,d] = vl_sift(I); %f is 4 rows, where the first 2 are the coordinates, the 3rd is the size of the neighbourhood and the fourth is the orientation
d = normc(single(d)); %normalize the feature vectors
d(d > 0.2) = 0.2; %cap the maximum value of bins to 0.2 to accomodate possible noise
d = (normc(d)); %renormalize the feature vectors

%for visualization:

perm = randperm(size(f,2));
sel = perm(1:20); %choose 50 random key points
h1 = vl_plotframe(f(:,sel));
h2 = vl_plotframe(f(:,sel));
set(h1,'color','k','linewidth',2);
set(h2,'color','y','linewidth',2);

%other way of visualising
h3 = vl_plotsiftdescriptor(d(:,sel),f(:,sel));
set(h3,'color','g');