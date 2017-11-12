function correlationExample(sigma)

im = imread('1459072322713.jpg');
im = double(rgb2gray(im))./255;

%set size of filter dimensions
sz = 3;


%./ divides every element within the matrix by 1
%kernel = ones(sz,sz)./(sz^2);
%instead of manually defining the kernel, we can use the Gaussian function
%The Gaussian filter is a better way of blurring, as the manual filter
%(average filter) results in loss of detail
kernel = fspecial('gaussian',round([sigma*6+1,sigma*6+1]),sigma);

%size of kernel - 1 divided by 2 so as to get the extra width on each side
width = (size(kernel) -1) / 2;

output = zeros(size(im));

for i = width(1)+1:size(im,1)-width(1)
    for j = width(2)+1:size(im,2)-width(2)
        area = im(i-width(1):i+width(1), j-width(2):j+width(2));
        output(i,j) = sum(sum(area.*kernel));
    end
end

subplot(1,2,1);imshow(im);
subplot(1,2,2);imshow(output);