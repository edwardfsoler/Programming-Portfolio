function pointProcessing

im = imread('Kiki.jpg');
%negative - can also use imcomplement(double(im))
im2 = 255 - rgb2gray(im);
%thresholding
im3 = double(rgb2gray(im))./255;

output = zeros(size(im3));

threshold = 0.5;

%Thresholding
for i = 1:size(im3,1)
    for j = 1:size(im3,2)
        if im3(i,j) > threshold
            output(i,j) = 1;
        else
            output(i,j) = 0;
        end
    end
end

figure;
%Input image
subplot(1,3,1);imshow(im);
%Negative image
subplot(1,3,2);imshow(im2);
%Threshold image
subplot(1,3,3);imshow(output);