function [training, validation] = crossValidation(training_data, percentage_train)
    %convert percentage into test related value
    percentage_train = 1 - percentage_train;
    %compute random indices at given percentage for test
    [train, test] = crossvalind('HoldOut',size(training_data,1),percentage_train);
    %split up data
    training = training_data(train,:);
    validation = training_data(test,:);
end