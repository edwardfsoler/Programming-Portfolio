function edgeDetection

X = double(rgb2gray(imread('Kiki.jpg')));
X = X./255

sz = 3;

%Prewitt Operator
%kernel for horizontal edges
%kernelx = [[-1,-1,-1];[0,0,0];[1,1,1]];
%kernel for vertical edges
%kernely = [[-1,0,1];[-1,0,1];[-1,0,1]];

%Sobel operator
%kernel for horizontal edges
%kernel = [[-1,-2,-1];[0,0,0];[1,2,1]];
%kernel for vertical edges
%kernel = [[-1,0,1];[-2,0,2];[-1,0,1]];

%Gaussian Approach
sigma = 5;
g = fspecial('gaussian',round([sigma*6,sigma*6]),sigma);
kernelx = gradient(g);
kernely = kernelx';

width = round((size(kernelx)-1)/2);

outputx = zeros(size(X));
outputy = zeros(size(X));
output = zeros(size(X));

%output = medfilt2(im); -- Achieves the same results as the below loop
for i = width(1)+1:size(X,1)-width(1)
    for j = width(2)+1:size(X,2)-width(2)
        area = X(i-width(1):i+width(1),j-width(2):j+width(2));
        outputx(i,j) = sum(sum(area.*kernelx));
        outputy(i,j) = sum(sum(area.*kernely));
        %combines horizontal and vertical edges into one matrix
        output(i,j) = sqrt(outputx(i,j).^2 + outputy(i,j).^2);
    end
end        

figure;
subplot(1,2,1);
imshow(X);
subplot(1,2,2);
imshow(output);