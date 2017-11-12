function [training, training_labels, validation, validation_labels] = crossValidationLabels(training_data, training_data_labels, percentage_train)
    %convert percentage into test related value
    percentage_train = 1 - percentage_train;
    %compute random indices at given percentage for test
    [train, test] = crossvalind('HoldOut',size(training_data,1),percentage_train);
    %split up data
    training = training_data(train,:);
    training_labels = training_data_labels(train,:);
    validation = training_data(test,:);
    validation_labels = training_data_labels(test,:);
end