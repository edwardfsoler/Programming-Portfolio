% Parameters: Audio xml data, start time of current suspected event, end 
%             time of current suspected event
% Returns:    Ground truth label of suspected event, index of event hit
%             by current suspected event
% Function searches for the ground truth and index of current suspected
% event within current audio file
function [t,i] = True_Category(xml_datum, start_time, end_time)
    
    %treshold set to 0 to allow for 1% overlaps
    thresh = 0;

    %automatically returns noise label if no match is found
    t = 1;
    
    %loop through all events for current audio files
    for i = 1:size(xml_datum.events,2)
        event = xml_datum.events{i};
        
        %if current event is hit by provided time, return ground label
        if (event.STARTSECOND - thresh < start_time && event.ENDSECOND + thresh > start_time)...
                || (event.STARTSECOND - thresh < end_time && event.ENDSECOND + thresh > end_time)...
                ||(event.STARTSECOND - thresh > start_time && event.ENDSECOND + thresh < end_time)
            t = event.CLASS_ID;
            break;
        end
    end
end