function SVM_Trainer
    %call vlfeat toolbox
    run('vlfeat-0.9.20/toolbox/vl_setup');
    
    %if dataset1 has been saved, then load it, otherwise compute it
    if ~exist('dataset1_training.mat','file')
        dataset1_training = DataSet1Import_Training('Dataset/Dataset 1/datatraining.txt',2,8143);
        save('dataset1_training.mat','dataset1_training');
    else
        load('dataset1_training.mat');
    end
    
    if ~exist('dataset1_validation.mat','file')
        dataset1_validation = DataSet1Import_Validation('Dataset/Dataset 1/datatest.txt',2,2805);
        save('dataset1_validation.mat','dataset1_validation');
    else
        load('dataset1_validation.mat');
    end
    if ~exist('dataset1_testing.mat','file')
        dataset1_testing = DataSet1Import_Testing('Dataset/Dataset 1/datatest2.txt',2,9753);
        save('dataset1_testing.mat','dataset1_testing');
    else
        load('dataset1_testing.mat');
    end
    
    if ~exist('dataset2_training.mat','file')
        dataset2_training = DataSet2Import_Training('Dataset/Dataset 2/SPECT_train.txt',1,80);
        save('dataset2_training.mat','dataset2_training');
    else
        load('dataset2_training.mat');
    end
    
    if ~exist('dataset2_testing.mat','file')
        dataset2_testing = DataSet2Import_Testing('Dataset/Dataset 2/SPECT_test.txt',1,187);
        save('dataset2_testing.mat','dataset2_testing');
    else
        load('dataset2_testing.mat');
    end
    
    if ~exist('dataset3_training.mat','file')
        dataset3_training = DataSet3Import_Training('Dataset\Dataset 3\UCI HAR Dataset\train\X_train.txt',1,7352);
        save('dataset3_training.mat','dataset3_training');
    else
        load('dataset3_training.mat');
    end
    
    if ~exist('dataset3_testing.mat','file')
        dataset3_testing = DataSet3Import_Testing('Dataset\Dataset 3\UCI HAR Dataset\test\X_test.txt',1,2947);
        save('dataset3_testing.mat','dataset3_testing');
    else
        load('dataset3_testing.mat');
    end
    
    if ~exist('dataset3_training_labels.mat','file')
        dataset3_training_labels = DataSet3Import_TestingLabels('Dataset\Dataset 3\UCI HAR Dataset\train\y_train.txt',1,7352);
        save('dataset3_training_labels.mat','dataset3_training_labels');
    else
        load('dataset3_training_labels.mat');
    end
    
    if ~exist('dataset3_testing_labels.mat','file')
        dataset3_testing_labels = DataSet3Import_TestingLabels('Dataset\Dataset 3\UCI HAR Dataset\test\y_test.txt',1,2947);
        save('dataset3_testing_labels.mat','dataset3_testing_labels');
    else
        load('dataset3_testing_labels.mat');
    end
    
    
    kernels = {'linear', 'quadratic', 'polynomial', 'rbf', 'mlp'};
    %iterscores = [];
    %for its=1:100
        %loop through different kernels
        %kernelscores = [];
        %for ks=1:size(kernels,2)
            ks = 4;
            fprintf('\n\nUsing kernel %s\n\n',kernels{ks});
            %%-----------DATA SET 1----------%%
            %lambda value for the svm
            %lambda = 0.00001;
            lambda = 0.00001;
            %loop through different lambda values
            scores = [];
            %while lambda < 1
                fprintf('Training SVM classifier on Data Set 1 with lambda=%s\n',lambda);
                %create vector with dataset outcomes
                labels1_train_pos = dataset1_training(:,6); %get last column of dataset (corresponds to occupancy result)
                labels1_train_pos(labels1_train_pos == 0) = -1; %convert all 0 values into -1 for vl_svmtrain

                %definition of SVM kernel
                hom.kernel = kernels{ks};
                %mapping of dataset onto new domains through kernel
                dataset = vl_svmdataset(dataset1_training(:,1:5)','homkermap',hom);

                [w1_pos b1_pos] = vl_svmtrain(dataset.data,labels1_train_pos',lambda);

                predicted_outcome = [];
                %loop through validation dataset
                %for i = 1:size(dataset1_validation,1)
                for i = 1:size(dataset1_testing,1)
                    %compute svm score for each validation case
                    %scores_val1 = sum(w1_pos'*dataset1_validation(i,1:5)')+b1_pos;
                    scores_val1 = sum(w1_pos'*dataset1_testing(i,1:5)')+b1_pos;
                    %classify predicted outcome as 1 or 0
                    if scores_val1 >= 0
                        predicted_outcome = [predicted_outcome 1];
                    else
                        predicted_outcome = [predicted_outcome 0];
                    end
                end
                %retrieve accuracy of classification
                %validation1_result = accuracy(dataset1_validation(:,6),predicted_outcome);
                %fprintf('Validation Accuracy: %.3f\n',validation1_result);
                %scores = [scores validation1_result];
                testing1_result = accuracy(dataset1_testing(:,6),predicted_outcome);
                fprintf('Tested Accuracy: %.3f\n',testing1_result);
                %scores = [scores testing1_result];
                %lambda = lambda * 10;
            %end
            %%------------DATA SET 1-------------%%

            fprintf('\n');

            %%------------DATA SET 2-------------%%
            %lambda value for the svm
            %lambda = 0.00001;
            lambda = 0.1;
            %scores = [scores;];
            %loop through different lambda values
            %while lambda < 1
            ks = 5;
                %fprintf('Creating validation set for Data Set 2\n');
                %[training, validation] = crossValidation(dataset2_training,0.8);
                training = dataset2_training;
                testing = dataset2_testing;
                fprintf('\n\nUsing kernel %s\n\n',kernels{ks});
                fprintf('Training SVM classifier on Data Set 2 with lambda=%s\n',lambda);

                %create vector with dataset outcomes
                labels2_train = training(:,1); %get first column of dataset (corresponds to outcome)
                labels2_train(labels2_train == 0) = -1; %convert all 0 values into -1 for vl_svmtrain
                %definition of SVM kernel
                hom.kernel = kernels{ks};
                %mapping of dataset onto new domains through kernel
                dataset = vl_svmdataset(training(:,2:23)','homkermap',hom);
                %training of svm classifier
                [w2_pos, b2_pos] = vl_svmtrain(dataset.data,labels2_train',lambda);

                predicted_outcome = [];
                %loop through validation dataset
                %for i = 1:size(validation,1)
                for i = 1:size(testing,1)
                    %compute svm score for each validation case
                    %scores_val2 = sum(w2_pos'*validation(i,2:23)')+b2_pos;
                    scores_val2 = sum(w2_pos'*testing(i,2:23)')+b2_pos;
                    %classify predicted outcome as 1 or 0
                    if scores_val2 >= 0
                        predicted_outcome = [predicted_outcome 1];
                    else
                        predicted_outcome = [predicted_outcome 0];
                    end
                end
                %retrieve accuracy of classification
                %validation2_result = accuracy(validation(:,1),predicted_outcome);
                %fprintf('Validation Accuracy: %.3f\n',validation2_result);
                %scores = [scores validation2_result];
                testing2_result = accuracy(testing(:,1),predicted_outcome);
                fprintf('Test Accuracy: %.3f\n',testing2_result);
                scores = [scores testing2_result];
                %lambda = lambda*10;
            %end
            %%------------DATA SET 2-------------%%

            fprintf('\n');

            %%------------DATA SET 3-------------%%
            %lambda value for the svm
            %lambda = 0.00001;
            lambda = 0.00001;
            %scores = [scores;];
            %loop through different lambda values
            %while lambda < 1
                %fprintf('Creating validation set for Data Set 3\n');
                %[training, training_labels, validation, validation_labels] = crossValidationLabels(dataset3_training, dataset3_training_labels, 0.8);
                training = dataset3_training;
                training_labels = dataset3_training_labels;
                testing = dataset3_testing;
                testing_labels = dataset3_testing_labels;
                fprintf('Training SVM classifier on Data Set 3 with lambda=%s\n',lambda);
                scores2 = [];
                ks = 3;
                fprintf('\n\nUsing kernel %s\n\n',kernels{ks});
                for i = 1: size(unique(training_labels),1)
                    temp = training_labels;
                    temp(temp ~= i) = -1;
                    temp(temp ~= -1) = 1;

                    %definition of SVM kernel
                    hom.kernel = kernels{ks};
                    %mapping of dataset onto new domains through kernel
                    dataset = vl_svmdataset(training','homkermap',hom);

                    %train svm for class i
                    [w3, b3] = vl_svmtrain(dataset.data,temp,lambda);
                    score = [];
                    %for j = 1: size(validation,1)
                    for j = 1: size(testing,1)
                        %score = [score sum(w3'*validation(j,:)')+b3];
                        score = [score sum(w3'*testing(j,:)')+b3];
                    end
                    scores2 = [scores2; score];
                end

                predicted_outcome = [];
                %loop through validation dataset
                %for i = 1:size(validation,1)
                for i = 1:size(testing,1)
                   [~,y] = max(scores2(:,i));
                   predicted_outcome = [predicted_outcome y];
                end
                %retrieve accuracy of classification
                %validation3_result = accuracy(validation_labels,predicted_outcome);
                %fprintf('Validation Accuracy: %.3f\n',validation3_result);
                %scores = [scores validation3_result];
                testing3_result = accuracy(testing_labels,predicted_outcome);
                fprintf('Test Accuracy: %.3f\n',testing3_result);
                scores = [scores testing3_result];
                %lambda = lambda * 10;
            %end
            %%------------DATA SET 3-------------%% 
            %kernelscores = [kernelscores; scores];
        %end
        %iterscores = [iterscores; kernelscores];
    %end
%     averagescore = [];
%     averagescores = [];
%     for i=1:size(iterscores,2)
%         for j=1:5
%             averagescore = [averagescore mean(iterscores(j:5:size(iterscores,1),i))];
%         end
%         averagescores = [averagescores; averagescore];
%         averagescore = [];
%     end
%     
%     averagescores = averagescores';

%         lambdascale = [0.00001 0.0001 0.001 0.01 0.1];
%         figure('name','Data set 1');
%         semilogx(lambdascale, averagescores(1,1:5),'-o',lambdascale, averagescores(2,1:5),'-o',lambdascale, averagescores(3,1:5),'-o',lambdascale, averagescores(4,1:5),'-o',lambdascale, averagescores(5,1:5),'-o');
%         str = sprintf('Data set 1 - max: %d', max(max(averagescores(1:5,1:5))));
%         legend('linear', 'quadratic', 'polynomial', 'rbf', 'mlp');
%         title(str);
%         ylabel('precision');
%         xlabel('sigma');
%         
%         figure('name','Data set 2');
%         semilogx(lambdascale, averagescores(1,6:10),'-o',lambdascale, averagescores(2,6:10),'-o',lambdascale, averagescores(3,6:10),'-o',lambdascale, averagescores(4,6:10),'-o',lambdascale, averagescores(5,6:10),'-o');
%         str = sprintf('Data set 2 - max: %d', max(max(averagescores(1:5,6:10))));
%         legend('linear', 'quadratic', 'polynomial', 'rbf', 'mlp');
%         title(str);
%         ylabel('precision');
%         xlabel('sigma');
%         
%         figure('name','Data set 3');
%         semilogx(lambdascale, averagescores(1,11:15),'-o',lambdascale, averagescores(2,11:15),'-o',lambdascale, averagescores(3,11:15),'-o',lambdascale, averagescores(4,11:15),'-o',lambdascale, averagescores(5,11:15),'-o');
%         str = sprintf('Data set 3 - max: %d', max(max(averagescores(1:5,11:15))));
%         legend('linear', 'quadratic', 'polynomial', 'rbf', 'mlp');
%         title(str);
%         ylabel('precision');
%         xlabel('sigma');
    
%     lambdas = {0.00001, 0.0001, 0.001, 0.01, 0.1};
%     
%     [x1,y1] = max(averagescores(1:size(averagescores,1),1:5));
%     [x11,y11] = max(x1);
%     
%     fprintf('Maximum performance of %0.3d for data set 1 achieved with %s kernel at %0.5d lambda\n',x11,kernels{y1(y11)},lambdas{y11});
%     
%     [x2,y2] = max(averagescores(1:size(averagescores,1),6:10));
%     [x21,y21] = max(x2);
%     
%     fprintf('Maximum performance of %0.3d for data set 2 achieved with %s kernel at %0.5d lambda\n',x21,kernels{y2(y21)},lambdas{y21});
%     
%     [x3,y3] = max(averagescores(1:size(averagescores,1),11:15));
%     [x31,y31] = max(x3);
%     
%     fprintf('Maximum performance of %0.3d for data set 3 achieved with %s kernel at %0.5d lambda\n',x31,kernels{y3(y31)},lambdas{y31});