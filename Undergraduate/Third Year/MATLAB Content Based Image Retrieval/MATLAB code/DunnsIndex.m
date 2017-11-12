% Dunns Index - A cluster validity measure

function validity = DunnsIndex(data)
klist = 1:1000;

for k = 1:numel(klist)
    labels = kmeans(data,klist(k));
    validity(k) = getDunnsIndex(data,labels);
end
figure;
plot(1-validity);

[~,mnidx] = min(validity);
fprintf('The best value of K is %d\n',klist(mnidx));

function validity = getDunnsIndex(data,labels)

% intra variance
intra = 0;
for i = 1:max(labels)
    idx = find(labels == i);
    datapoints = data(idx,:);
    centroid = mean(datapoints);
    
    for j = 1:size(datapoints,1)
        intra = intra + norm(datapoints(j,:) - centroid);
    end
end
intra = intra / size(data,1);

% inter variance
interlist = [];
for i = 1:max(labels)-1
    idx = find(labels == i);
    datapoints = data(idx,:);
    centroid_i = mean(datapoints);
    
    for j = i+1:max(labels)
        idx = find(labels == j);
        datapoints = data(idx,:);
        centroid_j = mean(datapoints);        
        interlist(end + 1) = norm(centroid_i - centroid_j);
    end
end
inter = min(interlist);
validity = intra / inter;
