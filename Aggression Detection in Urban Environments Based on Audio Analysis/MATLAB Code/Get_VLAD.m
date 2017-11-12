% Parameters: image features, training set vocabulary
% Return: VLAD descriptor
% Function computes VLAD descriptor based on current image features and
% training vocabulary
function vlad = Get_VLAD(data,vocabulary)
 
    assignment = zeros(size(data,2),1);
    dist = zeros(size(vocabulary,2),1);
    
    %Loop through features
    for i = 1:size(data,2)
        %Loop through vocabulary
        for j = 1:size(vocabulary,2)                
            %Compute normalised distance between each feature and each
            %vocabulary entry
            dist(j) = norm(vocabulary(:,j) - single(data(:,i)));
        end
        %store vocabulary word closest to current feature
        [~,assignment(i)] = min(dist);
    end
    
    
    v = [];
    %loop through vocabulary
    for j = 1:size(vocabulary,2)
        %find index of all features assigned to current vocabulary word
        idx = find(assignment == j);        
        %compute mean centroid of all features associated with current word
        residuals = bsxfun(@minus,single(data(:,idx)),vocabulary(:,j));
        v = [v; sum(residuals,2)];        
    end
    
    %Normalise VLAD descriptor
    vlad = v/norm(v);
end