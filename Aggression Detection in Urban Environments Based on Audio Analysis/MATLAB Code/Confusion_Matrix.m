% Parameters: cell of predicted categories, ground truth labels
% Returns: Confusion Matrix
% Function computes and displays a confusion matrix
function C = Confusion_Matrix(predicted, true_l)
    pred = cell2mat(predicted);
    C = confusionmat(true_l, [pred.cat]);
    disp(C);
    save('Confusion_Matrix.mat','C');
    fprintf('Accuracy = %s\n',trace(C)/sum(sum(C)));
end