% Parameters: cell of suspected events, cell of track lengths, cell of
%             predicted categories, size of neighbourhood (seconds),
%             comparison method for neighbourhood reduction
%             (SVM/Localisation)
% Returns:    Adjusted cell of suspected events and adjusted cell of 
%             predicted categories following neighbourhood reduction
% Function searches within a neighbourhood, keeping the suspected events
% with the greatest localisation response, removing remaining suspected
% events
function [peak, predicted_categories] = Neighbourhood_Reduction(peak, track_length, predicted_categories, neighbourhood_size, comparator)
    
    %Set counter to keep track of predicted_categories index
    x = 1;

    %loop through all audio files in validation set
    for i = 1:size(peak,1)
        %loop through all events in current audio file
        for j = 1:size(peak,2)
            %if current peak cell is emppty, skip
            if isempty(peak{i,j})
                continue;
            end
            
            %if current peak is to be disregarded, increment
            %predicted_categories counter
            if isfield(peak{i,j},'disregard')
                x = x + 1;
                continue;
            else
                peak{i,j}.disregard = false;
            end
            
            %compute right bound for current neighbourhood
            right_bound = peak{i,j}.end + neighbourhood_size/2;
            %ensure right bound is not past the end of the file
            if right_bound > track_length{i}
                right_bound = track_length{i} - 0.01;
            end
            
            %initialise neighbourhood to consider current suspected event
            neighbours = 0;
            
            %find all suspected events to the RHS of current event which
            %fall within neighbourhood
            for k = j+1:size(peak,2)
                %if current peak being considered has been set to
                %disregard, skip
                if ~isempty(peak{i,k}) && ~isfield(peak{i,k},'disregard')
                    %if current suspected event is within neighbourhood, store
                    %index
                    if peak{i,k}.start < right_bound
                        neighbours = [neighbours k-j];
                    else
                        %remaining suspected events cannot be within
                        %neighbourhood, so break
                        break;
                    end
                end
            end
            
            svm_score = [];
            localisation_score = [];
            
            %loop through all suspected events in neighbourhood
            for k = 1:numel(neighbours)
                %generate vector with all scores of suspected events in
                %neighbourhood
                if comparator == 1
                    svm_score = [svm_score predicted_categories{x+neighbours(k)}.score];
                elseif comparator == 2
                    localisation_score = [localisation_score peak{i,j+neighbours(k)}.localisation_score];
                end
            end   
            
            if comparator == 1
                [val,~] = max(svm_score);
                neighbours = neighbours(svm_score ~= val);
            elseif comparator == 2
                [val,~] = max(localisation_score);
                neighbours = neighbours(localisation_score ~= val);
            end

            %loop through neighbours, deleting predicted_categories and
            %peak indices for disregarded neighbours
            for k = 1:numel(neighbours)
                peak{i,j + neighbours(k)}.disregard = true;
                predicted_categories{x + neighbours(k)}.cat = 1;
            end
            
            x = x + 1;
        end
    end
end