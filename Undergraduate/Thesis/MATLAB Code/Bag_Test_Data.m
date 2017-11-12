% Parameters: Cell of suspected events within test data, vocabulary for bag
%             of words, representation to use for images (BoW/VLAD), number
%             of pyramid levels
% Returns:    Histogram of words for suspected events, ground truth labels
%             for suspected events
% Function extracts SIFT features for each suspected event, representing
% each suspected event with a histogram of words closest to each feature.
% The ground truth for each suspected event is also extracted and returned
function [bags, labels] = Bag_Test_Data(peak, vocab, representation, pyramid_levels)

    %Store copy of vocab size
    vocab_size = size(vocab,2);
    %Initialise matrix to hold histograms for each suspected event
    bags = [];
    %Initialise matrix to store ground truth for each suspected event
    labels = [];
    %Set the number of keypoints to be extracted for each suspected event
    keypoint_size = 1000;
    %build kdtree from vocab
    kdtree = vl_kdtreebuild(vocab);
    
    %loop through audio files
    for i = 1:size(peak,1)
        fprintf('Computing histogram of words for file %d of %d\n',i,size(peak,1));
        tic;
        %loop through events in audio files
        for j = 1:size(peak,2)
           
           if ~isfield(peak{i,j},'gamma')
               continue;
           end;
           
          %Initialise matrix to store histograms of each spatial tile
           cur_image_feats = [];
           
            %calculate segment sizes of peak gammatone wrt localisation
            %weights
            div_ratio = size(peak{i,j}.gamma,2)/(numel(peak{i,j}.resp_dist)+1);
           
           for level = 1:pyramid_levels
           
               %Compute spatial tile dimensions for current level
               spatial_tiling = 2^(level-1);
                          
               ymin = 1;
               %compute pixel dimensions for spatial tiles
               
               jump_x = floor(size(peak{i,j}.gamma,1)/spatial_tiling);
               jump_y = floor(size(peak{i,j}.gamma,2)/spatial_tiling);

               %calculate required step to compute ~1000 keypoints
                x_size = size(peak{i,j}.gamma,1);
                y_size = size(peak{i,j}.gamma,2);
                points = (x_size - 9) * (y_size - 9);
                step = sqrt(points/keypoint_size);
                
               %loop through tiles vertically
               for k = 1:spatial_tiling
                   xmin = 1;
                   %loop through tiles horizontally
                   for l = 1:spatial_tiling
                        %execute sift on current tile dimensions
                        [loc, feats] = vl_dsift(single(peak{i,j}.gamma(xmin:xmin+jump_x-1,ymin:ymin+jump_y-1)),'fast','step',step);
                                       
                        %Apply grading multiplier to spatial pyramid
                        %features based on pyramid level
                        for m = 1:size(feats,2)
                            ind = ceil(loc(2,m)/div_ratio);
                            feats(:,m) = feats(:,m)*(1+(peak{i,j}.resp_dist(ind))^2);
                        end
                        
                        %normalise features
                        feats = normc(single(feats));

                        %Initialise histograms for each suspected event
                        bag = zeros(1,vocab_size);

                        %compute high level representation based on
                        %parameter selected (BoW/VLAD)
                        if strcmp(representation,'BagOfWords')
                            %create histogram with words of current event
                            for m = 1:size(feats,2)
                                [bin,~] = vl_kdtreequery(kdtree,vocab,single(feats(:,m)),'NumNeighbors',1);
                                bag(bin) = bag(bin)+1;
                            end

                            %normalize histogram
                            bag = (normc(bag'))';

                            %add histogram to matrix holding features of each
                            %spatial tile for current event/noise
                            cur_image_feats = [cur_image_feats  bag .* (1/2^(pyramid_levels-level))];

                        elseif strcmp(representation,'VLAD')
                            vlad = getVLADdescriptor(feats,vocab);
                            %add histogram to matrix holding features of each
                            %spatial tile for current event/noise
                            cur_image_feats = [cur_image_feats; vlad .* (1/2^(pyramid_levels-level))];
                        end

                        %increment x coordinate of next window
                        xmin = xmin + jump_x;
                   end
                   %increment y coordinate of next window
                   ymin = ymin + jump_y;
               end
           end
            
           %store bag of words and label for each suspected event
           if strcmp(representation,'BagOfWords')
               bags = [bags; cur_image_feats];
           elseif strcmp(representation,'VLAD')
               bags = [bags cur_image_feats];
           end
            labels = [labels; peak{i,j}.event_class];
        end
        toc;
    end
    
    %Save histograms for suspected events
    save('Bag_Of_Localised.mat','bags','labels');
end