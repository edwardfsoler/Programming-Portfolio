% Parameters: Number of files in which saved event data is split, total
%             number of training audio files being considered, size of
%             vocabulary to compute
% Returns:    Vocabulary of words on which the Bag of Words model shall be
%             based
% Function computes spatial pyramid of SIFT features for each training
% event and noise gammatone representation. These features are then
% clustered into K clusters which are chosen as the new vocabulary

function vocab = Build_Vocab(num_files, tot_files, vocab_size)

    %Initialise matrix to store features of each event/noise representation
    feats = [];
    %Set the number of keypoints to be extracted from each representation
    keypoint_size = 1000;
    %Set counter to keep track of number of general set of events
    whole_i = 1;
    %Set the number of features to randomly sample from each event for
    %vocabulary
    vocab_feat_size = 100;
    %Initialise cell to store event data
    events = {};
    %Initialise cell to store noise data
    noise = {};

%%
    %loop through all data files
    for f = 1:num_files
        %load event data for file f
        fprintf('Loading data from file %d of %d...\n',f,num_files');
        load(strcat('Processed_Data_',num2str(f),'.mat'));
        load(strcat('Processed_Data_Noise_',num2str(f),'.mat'));

        %loop through different audio files within current
        for i = 1:size(events,1)
            tic;
            fprintf('Bagging file %d of %d\n',whole_i,tot_files);

            %loop through events for each audio file
            j = 1;
            while j < size(events,2) && isfield(events{i,j},'D')
                %enter if event is in training set
                if ~isempty(events{i,j}.D) && rand > 0.5
                    %calculate required step size to compute ~1000 keypoints
                    x_size = size(events{i,j}.D,1);
                    y_size = size(events{i,j}.D,2);
                    points = (x_size - 9) * (y_size - 9);
                    step = sqrt(points/keypoint_size);

                    %extract keypoints for current event gammatonegram
                    [~, d] = vl_dsift(single(events{i,j}.D),'fast','step',step);
                    %normalise feature descriptor
                    d = normc(single(d));

                    %add features for current event to matrix of features
                    feats = [feats; d'];
                end
                %increment counter
                j = j + 1;
            end

            %loop through noise events for each audio file
            j = 1;
            while j < size(noise,2) && isfield(noise{i,j},'D')
                %enter if noise event exists
                if ~isempty(noise{i,j}.D) && rand > 0.5

                    %calculate required step size to compute ~1000 keypoints
                    x_size = size(noise{i,j}.D,1);
                    y_size = size(noise{i,j}.D,2);
                    points = (x_size - 9) * (y_size - 9);
                    step = sqrt(points/keypoint_size);

                    %extract keypoints for current event gammatonegram
                    [~, d] = vl_dsift(single(noise{i,j}.D),'fast','step',step);
                    %normalise feature descriptor
                    d = normc(single(d));

                    d = datasample(d,vocab_feat_size,2,'Replace',false);

                    %add features of current noise snippet to matrix of
                    %features
                    feats = [feats; d'];
                end
                %increment counter
                j = j + 1;
            end
            whole_i = whole_i + 1;
            toc;
        end
        %save current set of features within file and re-initialise feats
        %matrix
        save(strcat('Features_',num2str(f),'.mat'),'feats');
        feats = [];
    end

    %clear all variables except num_files to save primary memory
    clearvars -except num_files vocab_size

    %Initialise matrix to store event features for all events within the
    %saved files
    combined_feats = [];

    %reload all feature files and combine into sigle vector
    for f = 1:num_files
        fprintf('Reloading feature file %d of %d...\n',f,num_files);
        load(strcat('Features_',num2str(f),'.mat'));
        combined_feats = [combined_feats; feats];
    end

    %Perform K-Means clustering on full set of features with K = vocab_size
    fprintf('Performing K-Means clustering of vocabulary\n');
    [vocab, ~] = vl_kmeans(single(combined_feats'),vocab_size);
    %[vocab, ~] = vl_kmeans(single(combined_feats'),vocab_size,'Algorithm','ANN','MaxNumComparisons',3);

    %Save computed vocabulary
    fprintf('Saving vocabulary...\n');
    save('Gammatone_Vocab.mat','vocab');
end