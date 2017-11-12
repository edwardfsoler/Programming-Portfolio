function integralImage

%One of the only algorithms which can run in real-time as it is so fast

im = imread('Kiki.jpg');
im = double(rgb2gray(im));
imi = cumsum(cumsum(im,2));

width = 3;
tic;
outputx = zeros(size(im));
outputy = zeros(size(im));
outputxy = zeros(size(im));
for i = width+1:size(im,1)-width;
    for j = width+1:size(im,2)- width
        A = imi(i-width,j-width);
        B = imi(i-width,j);
        C = imi(i-width,j+width);
        D = imi(i+width,j-width);
        E = imi(i+width,j);
        F = imi(i+width,j+width);
        outputy(i,j) = A - (2*B) + C - D + (2*E) - F; %for vertical edges
        D = imi(i,j-width);
        E = imi(i,j);
        F = imi(i,j+width);
        G = imi(i+width,j-width);
        H = imi(i+width,j);
        I = imi(i+width,j+width);
        outputx(i,j) = (2*F) - (2*D) - C + A - I + G; % for horizontal edges
        outputxy(i,j) = A - (2*B) + C - (2*D) + (4*E) - (2*F) + G - (2*H) + I; %for diagonal edges
        tempout(i,j) = sqrt(outputx(i,j).^2 + outputy(i,j).^2); %merging of horizontal with vertical edges
        output(i,j) = sqrt(tempout(i,j).^2 + outputxy(i,j).^2); %merging of horizontal and vertical edges with diagonal
        tout(i,j) = nthroot(outputx(i,j).^3 + outputxy(i,j).^3 + outputy(i,j).^3,3);
    end
end
toc; %shows time elapsed to rach here from tic
figure;
subplot(2,2,1);imagesc(im);colormap gray;axis image;axis off;
subplot(2,2,2);imagesc(imi);axis image;
subplot(2,2,3);imagesc(tout);colormap gray;axis image;axis off;
subplot(2,2,4);imagesc(output);colormap gray;axis image;axis off;
