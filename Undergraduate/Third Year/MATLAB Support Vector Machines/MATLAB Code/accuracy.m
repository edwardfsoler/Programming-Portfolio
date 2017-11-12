function result = accuracy(actual_outcome, predicted_outcome)
%compute accuracy by comparing actual outcome to the predicted outcome

    %ensure matrix are oriented correctly
    if size(actual_outcome,1) == 1
        actual_outcome = actual_outcome';
    end
    
    if size(predicted_outcome,1) == 1
        predicted_outcome = predicted_outcome';
    end
    
    agreement = [];
    %loop through outcomes comparing whether they agree
    for i = 1:size(actual_outcome,1)
        if actual_outcome(i) == predicted_outcome(i)
            agreement = [agreement; 1];
        else
            agreement = [agreement; 0];
        end
    end
    %compare total number of agreements to total number of cases to get the
    %accuracy rating
    result = size(agreement(agreement == 1),1)/size(agreement,1);
end