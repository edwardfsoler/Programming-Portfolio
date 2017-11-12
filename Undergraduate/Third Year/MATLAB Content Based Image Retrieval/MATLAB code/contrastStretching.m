function contrastStretching

load clown;

im = X;

im = im./255; 

y = min(im(im > 0));

stretchedim = im - y;
stretchedim = stretchedim .* 2;

figure;
subplot(1,2,1); imshow(im);
subplot(1,2,2); imshow(stretchedim);