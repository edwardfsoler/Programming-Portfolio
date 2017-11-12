% Parameters: histogram of words for training data, histogram of words for
%             suspected events, lambda value and kernel value, vocabulary
%             size
% Returns:    Predicted category for each suspected event
% Function trains an SVM classifier to classify the suspected events and
% return their predicted categories
function predicted_categories = SVM(train_feats, test_feats, lambda, kernel_val, vocab_size)
    
    %Initialise cell for predicted categories of suspected events
    predicted_categories = cell(size(test_feats,1),1);
    %Create cell holding all possible categories
    cats = {1 2 3 4};
    %Initialise a cell to store the classification score for each SVM for
    %each noise level an category
    temp_score = cell(size(cats,2),7);

    %% Compute SVM Values
    fprintf('Computing SVM score values...\n');

    if strcmp(kernel_val,'HistogramIntersection')
        h_i = true;
        model = cell(size(cats,2),7);
        kernel = @(X,Y) Custom_Kernel(X,Y);
    else
        h_i = false;

        %Initialise cells to hold SVM values
        w = cell(size(cats,2),7);
        b = cell(size(cats,2),7);

        %set kernel to kernel parameter
        hom.kernel = kernel_val;
    end

    %Loop through all possibile categories
    for i=1:size(cats,2)
        %loop through all the possible noise levels
        for j=1:7

            %train one svm with all data
            if j == 7
                temp_feats = train_feats;
            else
                %reduce feature set to those with same noise levels to
                %train SVM for specific noise level
                temp_feats = train_feats([train_feats.noise_level] == j*5);
            end

            %prepare binary distinguisher for svm
            x = single(cats{i} == [temp_feats.event_class]);
            x(x == 0) = -1;

            if h_i == true
                tf = [temp_feats.bags];
                tf = reshape(tf,numel(tf)/size(temp_feats,1),size(temp_feats,1));
                tf = tf';
                [tf,test_feats] = Normalize_Data(tf,test_feats,vocab_size);

                trainingKernel = [(1:size(tf,1))', kernel(tf,tf)];
                model{i,j} = svmtrain(double(x)',trainingKernel,'-t 4 -c 1 -q -b 1');

                testingKernel = [(1:size(test_feats,1))', kernel(test_feats,tf)];    
                [~,~,temp] = svmpredict(double(ones(size(testingKernel,1),1)),testingKernel,model{i,j});    
                temp_score{i,j} = temp;
            else
                %map dataset onto new domain through passed kernel
                dataset = vl_svmdataset(tf,'homkermap',hom);
                %store SVM raters in cell array
                [w{i,j},b{i,j}] = vl_svmtrain(dataset.data, double(x),lambda);
            end
        end
    end

    temp_score_2 = reshape(temp_score,1,28);
    s = cell2mat(temp_score_2);

    fprintf('Predicting categories...\n');
    
    %Initialise matrix to store classification scores for each category for
    %each suspected event
    scores = [];
    
    if h_i == true;
        for k=1:size(test_feats,1)
            temp = [];
            for i = 1:numel(cats)
                for j = 1:7
                    temp = [temp temp_score{i,j}(k)];
                end
            end
            [val,ind] = max(temp);

            predicted_categories{k}.cat = cats{ceil(ind/7)};
            predicted_categories{k}.score = val;
        end
    else
        %loop through all test feats to calculate score for each one
        for k = 1:size(test_feats,1)
            %loop through different categories
            for i=1:size(cats,2)
                %loop through different noise levels
                for j=1:7
                    %compute score for current event for current category and
                    %noise leveled SVM
                    temp_score{i,j} = sum(w{i,j}'*test_feats(k,:)')+b{i,j}; 
                end 
            end
            %store highest score of all noise leveled classifiers for each category
            scores = [scores max(cell2mat(temp_score)')'];
        end
        
        %Loop through all suspected events, setting the category with the
        %greatest score to be the predicted category
        for i = 1:size(scores,2)
            [val,ind] = max(scores(:,i));

            predicted_categories{i}.cat = cats{ceil(ind/7)};
            predicted_categories{i}.score = val;
        end
    end
    
    %Save predicted categories
    fprintf('Saving predicted categories...\n');
    save('Predicted_Categories.mat','predicted_categories');
end