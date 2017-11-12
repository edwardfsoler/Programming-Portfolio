function logTransformation
%Log Transformation is similar to contrast shifting but the stretching of
%the pixels is not constant, but log-linear, as pixels in different
%positions are shifted by different amounts.

load clown;

X = X./255;
c = 2;
logshifted = c.*log(1+X);

figure;
subplot(1,2,1);
imshow(X);
subplot(1,2,2);
imshow(logshifted);