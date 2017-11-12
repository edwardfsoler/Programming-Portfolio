% Parameters: Vocabulary for bag of words, number of files in which to split
%             processed data, total number of audio files considered,
%             manner in which images are representated (BoW/VLAD), number
%             of pyramid levels
% Returns:    Histogram of words for each event/noise within training set
% Function extracts SIFT features for each training representation,
% selecting the closest word within the vocabulary to represent it. A
% histogram of words is then created for each image
function bags_training = Get_Bag_Of_Sifts(vocab, num_files, tot_files, representation, pyramid_levels)

    %Store copy of vocab size
    vocab_size = size(vocab,2);
    %Initialise histogram for each feature
    bag = zeros(1,vocab_size);
    %Initialise matrix to store histogram of words for each event/noise
    bags_training = [];
    %Initialise number of keypoints to extract from each event/noise
    keypoint_size = 1000;
    %Set counter to keep track of all events across all files
    whole_i = 1;
    %Initialise cells to store event and noise data
    events = {};
    noise = {};

    %build kdtree from vocab
    kdtree = vl_kdtreebuild(vocab);

    %%
    %loop through all data files
    for f = 1:num_files
        %load event data for file f
        fprintf('Loading data from file %d of %d...\n',f,num_files');
        load(strcat('Processed_Data_',num2str(f),'.mat'));
        load(strcat('Processed_Data_Noise_',num2str(f),'.mat'));

        %%
        %loop through audio files
        for i = 1:size(events,1)
            fprintf('Computing histogram of words for image %d of %d\n',whole_i, tot_files);
            %loop through events in current audio file
            tic;
            %%
            for j = 1:size(events,2)

                %skip current event if not representation is found
                if ~isfield(events{i,j},'D')
                    continue;
                end;
                
                %Set matrix to store features of each spatial tile for
                %current event/noise
                cur_image_feats = [];
                
                %Loop through all pyramid levels 
                for level = 1:pyramid_levels
                    %Compute dimensions of spatial tiles in current level
                    spatial_tiling = 2^(level-1);
                    
                    %compute coordinates of split for spatial tiling
                    ymin = 1;

                    %Compute pixel size of spatial tiles
                    jump_x = floor(size(events{i,j}.D,1)/spatial_tiling);
                    jump_y = floor(size(events{i,j}.D,2)/spatial_tiling);

                    %calculate required step to compute ~1000 keypoints
                    x_size = size(events{i,j}.D,1);
                    y_size = size(events{i,j}.D,2);
                    points = (x_size - 9) * (y_size - 9);
                    step = sqrt(points/keypoint_size);

                    %loop through tiles vertically
                    for k = 1:spatial_tiling
                        xmin = 1;
                        %loop through tiles horizontally
                        for l = 1:spatial_tiling
                            %execute sift on current tile dimensions
                            [~, feats] = vl_dsift(single(events{i,j}.D(xmin:xmin+jump_x-1,ymin:ymin+jump_y-1)),'fast','step',step);
                            feats = normc(single(feats));                              

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
                                vlad = Get_VLAD(feats,vocab);
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

                %update histogram of words, event class, noise class and
                %noise level for current event
                x.bags = cur_image_feats;
                x.event_class = events{i,j}.event_class;
                x.noise_class = events{i,j}.noise_class;
                x.noise_level = events{i,j}.noise_level;

                %add histogram and data of current event to matrix holding
                %all events/noises
                bags_training = [bags_training; x];
            end

            %%
            %loop through noise snippets within the training set
            for j=1:size(noise,2)
                
                if ~isfield(noise{i,j},'D')
                    continue;
                end;
                
                %Set matrix to store features of each spatial tile for
                %current event/noise
                cur_image_feats = [];
                
                for level = 1:pyramid_levels
                    %Compute dimensions of spatial tiles in current level
                    spatial_tiling = 2^(level-1);
                
                    %compute pixel dimensions of spatial tiles
                    jump_x = floor(size(noise{i,j}.D,1)/spatial_tiling);
                    jump_y = floor(size(noise{i,j}.D,2)/spatial_tiling);

                    ymin = 1;

                    %calculate required step size to compute ~1000 keypoints
                    x_size = size(noise{i,j}.D,1);
                    y_size = size(noise{i,j}.D,2);
                    points = (x_size - 9) * (y_size - 9);
                    step = sqrt(points/keypoint_size);

                    %loop through tiles vertically
                    for k = 1:spatial_tiling
                        xmin = 1;
                        %loop through tiles horizontally
                        for l = 1:spatial_tiling
                            %execute sift on current tile dimensions
                            [~, feats] = vl_dsift(single(noise{i,j}.D),'fast','step',step,'bounds', [xmin, ymin, xmin+jump_x-1, ymin+jump_y-1]);
                            feats = normc(single(feats));
                            
                            %compute appropriate representation for images
                            %(BoW/VLAD)
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
                                %compute VLAD descriptor
                                vlad = Get_VLAD(feats,vocab);
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

                %update histogram of words, event class, noise class and
                %noise level for current event
                x.bags = cur_image_feats;
                x.event_class = 1;
                x.noise_class = noise{i,j}.noise_class;
                x.noise_level = noise{i,j}.noise_level;

                %add histogram and data of current event to matrix holding
                %all events/noises
                bags_training = [bags_training; x];
            end
            whole_i = whole_i + 1;
            toc;
        end
    end

    %save bag of sifts
    save('Bag_Of_Sifts.mat','bags_training');
end