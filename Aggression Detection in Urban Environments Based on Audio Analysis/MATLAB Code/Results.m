% Parameters: Audio xml data, irectory of dataset, cell of predicted
%             categories for suspected events, cell matrix of suspected events for each
%             audio file, vector of indices of validation files, Boolean dictating use
%             of validation/testing set
% Returns:    Vector of ground labels for suspected events
% Function    Function evaluates classification of suspected events, by
%             matching the label and location of suspected events with the
%             ground truth
function ground_labels = Results(xml_data, directory, predicted, peak, val_set, val_bool)
    
    x = 1;
    TP = [];
    FP = [];
    TN = [];
    FN = [];
    FN_2 = [];
    tot_events = [];
    ground_labels = [];
    
    %if function is set to use test set, modify directories 
    if val_bool == false
        %Initialise directory for xml data
        xml_dir = dir(strcat(directory,'\testing\*.xml'));
        %Initialise cell to hold xml data of test files
        xml_data = cell(1,numel(xml_dir));

        %Load xml testing data from file
        for true_i=1:numel(xml_dir)
            xml_data{true_i} = xml_load(strcat(directory,'\testing\', xml_dir(true_i).name));
        end
        
        %create new val_set for test data
        val_set = 1:numel(xml_dir)*8;
        val_set = val_set(~(mod(val_set,8) == 1 | mod(val_set,8) == 2));
    end

    %loop through audio files to evaluate
    for i = 1:size(peak,1)
        
       %find audio file which relates to current labels
       ind = val_set(i);
       
       %store copy of xml data for current file
       xml_datum = xml_data{ceil(ind/8)};
       
       %loop through suspected events within current audio file
       for j = 1:size(peak,2)
           %if current cell is empty, skip
           if isempty(peak{i,j})
               continue;
           end
           
           %store copy of ground truth and sequence number of event of hit
           [ground,event_ind] = True_Category(xml_datum,peak{i,j}.start,peak{i,j}.end);
           ground_labels = [ground_labels ground];
           
           %if suspected event set to be disregarded by neighbourhood
           %reduction, skip
           if isfield(peak{i,j},'disregard') && peak{i,j}.disregard == true
%                z = Peak_Data(ind, ground, xml_datum, predicted{x}.cat);
%                TN = [TN; z];
                x = x + 1;
               continue;
           end
        
           if predicted{x}.cat == ground && predicted{x}.cat ~= 1
               %correct classification of event
               
               %enter if current event has not already been hit by TP
               if ~isfield(xml_datum.events{event_ind},'hit') || xml_datum.events{event_ind}.hit ~= 1
                   
                    %compute data for current peak
                    z = Peak_Data(ind, ground, xml_datum, predicted{x}.cat);
                    TP = [TP; z];
                    %mark that true event has been visited
                    xml_datum.events{event_ind}.hit = 1;
               end
           elseif predicted{x}.cat == ground && predicted{x}.cat == 1
               %correct classification of background noise
               %compute data for current peak
               z = Peak_Data(ind, ground, xml_datum, predicted{x}.cat);
               TN = [TN; z];
           elseif ground == 1
               %actual noise was predicted as event
               
               %compute data for current peak
               z = Peak_Data(ind, ground, xml_datum, predicted{x}.cat);
               FP = [FP; z];
           elseif ~isfield(xml_datum.events{event_ind},'hit')
               %event was predicted as wrong event

               %compute data for current peak
               z = Peak_Data(ind, ground, xml_datum, predicted{x}.cat);
               if predicted{x}.cat == 1
                   FN_2 = [FN_2; z];
               else
                   FN = [FN; z];
               end
               
               %mark that true event has been visited
               if ~isfield(xml_datum.events{event_ind},'hit')
                   xml_datum.events{event_ind}.hit = 2;
               end
           end
           x = x + 1;
       end

       %loop through all events, searching for missed events
       for j = 1:numel(xml_datum.events)
           %compute data for current peak
           z = Peak_Data(ind, xml_datum.events{j}.CLASS_ID, xml_datum, 4);
           tot_events = [tot_events; z];
           
           if ~isfield(xml_datum.events{j},'hit')
               xml_datum.events{j}.hit = 1;
               FN_2 = [FN_2; z];
           end
       end
    end
    
    %compute accuracy (correctly classified events/total number of events)
    accuracy = numel(TP)/numel(tot_events);
    
    fprintf('Total number of events: %d\n',numel(tot_events));
    fprintf('True Positive: %d\n',numel(TP));
    fprintf('True Negative: %d\n',numel(TN));
    fprintf('False Positive: %d\n',numel(FP));
    fprintf('False Negative 1 (Misclassification): %d\n',numel(FN));
    fprintf('False Negative 2 (Miss): %d\n',numel(FN_2));
    fprintf('Accuracy: %d\n',accuracy);
    fprintf('False Rejection: %d\n',(numel(FN) + numel(FN_2))/numel(tot_events));
    
    %Initialise cell to store evaluation sheet
    eval = cell(20,5);
    
    eval{1,2} = 'Glass';
    eval{1,3} = 'Gunshot';
    eval{1,4} = 'Scream';
    eval{2,1} = '5dB';
    eval{3,1} = '10dB';
    eval{4,1} = '15dB';
    eval{5,1} = '20dB';
    eval{6,1} = '25dB';
    eval{7,1} = '30dB';
    eval{8,1} = 'Total';
    eval{1,5} = 'Total';
    eval{10,1} = 'Occurence';
    eval{11,1} = 'TP';
    eval{12,1} = 'TN';
    eval{13,1} = 'FP';
    eval{14,1} = 'FN 1 -predicted';
    eval{15,1} = 'FN 1 -ground';
    eval{16,1} = 'Miss';
    eval{8,5} = accuracy;
    eval{18,1} = 'Glass';
    eval{19,1} = 'Gunshot';
    eval{20,1} = 'Scream';
    
    eval{18,2} = numel(TP([TP.predicted] == 2));
    eval{18,3} = numel(FN([FN.predicted] == 3 & [FN.ground_truth] == 2));
    eval{18,4} = numel(FN([FN.predicted] == 4 & [FN.ground_truth] == 2));
    eval{18,5} = numel(FN_2([FN_2.ground_truth] == 2));
    
    eval{19,2} = numel(FN([FN.predicted] == 2 & [FN.ground_truth] == 3));
    eval{19,3} = numel(TP([TP.predicted] == 3));
    eval{19,4} = numel(FN([FN.predicted] == 4 & [FN.ground_truth] == 3));
    eval{19,5} = numel(FN_2([FN_2.ground_truth] == 3));
    
    eval{20,2} = numel(FN([FN.predicted] == 2 & [FN.ground_truth] == 4));
    eval{20,3} = numel(FN([FN.predicted] == 3 & [FN.ground_truth] == 4));
    eval{20,4} = numel(TP([TP.predicted] == 4));
    eval{20,5} = numel(FN_2([FN_2.ground_truth] == 4));
    
    for i = 2:4
        %populate sheet with accuracy of each event in each noise level
        for j = 2:7
            eval{j,i} = numel(TP([TP.ground_truth] == i & [TP.noise_level] == (j-1)*5)) /...
            numel(tot_events([tot_events.ground_truth] == i & [tot_events.noise_level] == (j-1)*5));
        end
        
        %compute mean accuracy for each event over all noise levels
        eval{8,i} = numel(TP([TP.ground_truth] == i)) /...
            numel(tot_events([tot_events.ground_truth] == i));
        
        %compute total occurences of each event
        eval{10,i} = numel(tot_events([tot_events.ground_truth] == i));
        
        %compute total true positives of each event
        eval{11,i} = numel(TP([TP.predicted] == i));
                
        %compute total number of false positives
        if numel(FP) > 0
            eval{13,i} = numel(FP([FP.predicted] == i));
        else
            eval{13,i} = 0;
        end
        
        %compute total number of false negatives by prediction/ground truth
        eval{14,i} = numel(FN([FN.predicted] == i));
        eval{15,i} = numel(FN([FN.ground_truth] == i));
        
        %compute total number of skips for each event
        if numel(FN_2) > 0
            eval{16,i} = numel(FN_2([FN_2.ground_truth] == i));
        else
            eval{16,i} = 0;
        end
    end
    
    %compute mean scores across all events for each noise level
    for j = 2:7
        eval{j,5} = numel(TP([TP.noise_level] == (j-1)*5)) /...
            numel(tot_events([tot_events.noise_level] == (j-1)*5));
    end
    
    eval{10,5} = numel(tot_events);
    eval{11,5} = numel(TP);
    eval{12,5} = numel(TN);
    eval{13,5} = numel(FP);
    eval{14,5} = numel(FN);
    eval{15,5} = numel(FN);
    eval{16,5} = numel(FN_2);
      
    %save evaluation to file
    save('Evaluation.mat','eval','TP','FP','FN','TN');
end