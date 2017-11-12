function computeHessian
%unfinished
im = imread('Kiki.jpg');
im = double(rgb2gray(im));
im = im ./ 255;
sigmaList = [1:5];

for i = 1:numel(sigmaList)
    Hessian(:,:,i) = getHessian(im,sigmaList(i));
end

Hessian2 = zeros(size(Hessian(:,:,1)));
for i = 1:numel(sigmaList)
    Hessian2 = Hessian2 + Hessian(:,:,i);
end

Hessian2 = Hessian2 ./ length(sigmaList);
figure;
imagesc(Hessian2);colormap gray;axis image;axis off;

function Hessian = getHessian(im,sigma)
g = fspecial('gaussian',round([sigma*6,sigma*6]),sigma);
Gy = diff(g);
Gyy = diff(Gy);
Gx = Gy';
Gxy = diff(Gy);
Gxx = Gyy';

Lxx = conv2(im,Gxx,'same');
Lyy = conv2(im,Gyy,'same');
Lxy = conv2(im,Gxy,'same');
Hessian = (Lxx.*Lyy) - (Lxy.^2);