function crossValidation

  % Classify the fisheriris data with a K-Nearest Neighbor classifier.
  % Assign an inconclusive prediction to samples which K-Nearest 
  % neighbors do not belong all to the same class.
  load fisheriris
  knn = fitcknn(meas,species,'NumNeighbors',4);
  [c,posterior] = resubPredict(knn);
  c(max(posterior,[],2)<1) = {''};
  cp = classperf(species,c) %classification performance (takes thruth labels from dataset and computed labels

  % Create a 10-fold cross-validation on the fisheriris data using
  % linear discriminant analysis and the third column as the only
  % feature for classification.
  load fisheriris
  indices = crossvalind('Kfold',species,10); %indices is an array of index values from 1-10 for each element (bin 1-10)
  cp = classperf(species); % initializes the CP object
  for i = 1:10
      test = (indices == i); train = ~test; %split data into train and test sets for current rotation
      class = classify(meas(test,3),meas(train,3),species(train));
      % updates the CP object with the current classification results
      classperf(cp,class,test);
      error(i) = sum(~strcmp(class,species(test)))/(size(species,1)/10); %calculate the number of mismatches of the assigned class to the dataset from the training set
  end
  cp.CorrectRate % queries for the correct classification rate