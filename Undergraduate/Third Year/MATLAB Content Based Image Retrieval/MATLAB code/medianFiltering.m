function medianFiltering
%Median Filtering takes the median value of pixels within the kernel area.
%This is used for filtering of noise such as salt & pepper noise
load clown
X = X./255

sz = 3;

kernel = ones(sz,sz);
width = (size(kernel)-1)/2;

output = zeros(size(X));

%add salt and pepper noise to the image
im = imnoise(X,'salt & pepper');

%output = medfilt2(im); -- Achieves the same results as the below loop
for i = width(1)+1:size(im,1)-width(1)
    for j = width(2)+1:size(im,2)-width(2)
        area = im(i-width(1):i+width(1),j-width(2):j+width(2));
        output(i,j) = median(area(:));
    end
end        

figure;
subplot(1,3,1);
imshow(X);
subplot(1,3,2);
imshow(im);
subplot(1,3,3);
imshow(output);