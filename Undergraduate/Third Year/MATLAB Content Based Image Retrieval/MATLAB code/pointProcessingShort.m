function pointProcessingNew

im = imread('Liberty.jpg');

im = im./255

figure;
subplot(1,3,1);
imshow(double(im));
subplot(1,3,2);
imshow(1-double(im));
subplot(1,3,3);
imshow(double(double(im) > 0.1));