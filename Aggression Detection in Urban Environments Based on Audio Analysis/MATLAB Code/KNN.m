% Parameters: set of training features from training images, set of test
%             features from test images
% Returns:    predicted categories for each test image
% Function computes K-nearest neighbour algorithm to classify test images
function predicted_categories = KNN(train_feats,test_feats)
    
    tf = [train_feats.bags];
    lf = [train_feats.event_class];
    
    %execute k-nearest neighbour algorithm
    predicted = knnclassify(test_feats',tf',lf);
    
    %initialise cell to store predicted categories
    predicted_categories = cell(numel(predicted),1);
    
    %create and save structs for each test image
    for i=1:numel(predicted)
        x.cat = predicted(i);
        x.score = 0;
        predicted_categories{i} = x;
    end
end