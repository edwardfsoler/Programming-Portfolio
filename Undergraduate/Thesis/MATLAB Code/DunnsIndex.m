% Dunns Index - A cluster validity measure

function K = DunnsIndex(data)

    klist = 2:1:40;
    %klist = 50;
    
    validity = zeros(1,numel(klist));

    for k = 1:numel(klist)
        fprintf('Considering K = %d\n',klist(k));
        tic;
        [~, labels] = vl_kmeans(single(data'),klist(k));
        validity(k) = getDunnsIndex(data,labels);
        fprintf('%d',validity(k));
        toc;
    end
    figure;
    plot(klist,validity,'-o')

    [~,mnidx] = min(validity);
    K = klist(mnidx);
    fprintf('The best value of K is %d\n',K);
end

function validity = getDunnsIndex(data,labels)
    % intra variance
    intra = 0.0;
    for i = 1:max(labels)
        datapoints = data(labels == i,:);
        if size(datapoints,1) > 1
            centroid = mean(datapoints);
        else
            centroid = double(datapoints);
        end

        for j = 1:size(datapoints,1)
            intra = intra + norm(double(datapoints(j,:)) - centroid(1,:));
        end
    end
    intra = intra / size(data,1);

    % inter variance
    interlist = [];
    for i = 1:max(labels)-1
        datapoints = data(labels == i,:);
        centroid_i = mean(datapoints);

        for j = i+1:max(labels)
            datapoints = data(labels == j,:);
            if size(datapoints,1) > 1
                centroid_j = mean(datapoints);        
            else
                centroid_j = double(datapoints);
            end
            interlist(end + 1) = norm(centroid_i - centroid_j);
        end
    end
    inter = min(interlist);
    validity = intra / inter;
end
