% Author: Dr. George Azzopardi
% University of Malta
% ICS3129 - Content Based Image Retrieval and Categorization
% Bag of Visual Words

function bagofwords

% The following line assumes you have the library vlfeat-0.9.20 in a
% sub folder
run vlfeat-0.9.20/toolbox/vl_setup

d = dir('*.jpg');
totaldesc = [];
for i = 1:numel(d)
    im = imread(d(i).name);
    im = single(rgb2gray(im));
    
    [~,desc] = vl_sift(im);
    totaldesc = [totaldesc desc];
end

% Create Vocabulary
[~,vocabulary] = kmeans(double(totaldesc'),10);
vocabulary = vocabulary';

d = dir('*.jpg');
for i = 1:numel(d)
    im = imread(d(i).name);
    im = single(rgb2gray(im));
    
    [~,desc] = vl_sift(im);   
    bag = zeros(1,size(vocabulary,2));
    for j = 1:size(desc,2)
        dist = [];
        for k = 1:size(vocabulary,2)
            dist(k) = norm(double(desc(:,j)) - double(vocabulary(:,k)));
        end
        [~,bin] = min(dist);
        bag(bin) = bag(bin) + 1;
    end
end

figure;bar(bag);
