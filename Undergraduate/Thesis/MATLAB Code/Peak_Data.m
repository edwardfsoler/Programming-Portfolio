% Parameters: index of audio file to which suspected event relates, ground
%             truth of suspected event, xml data of current audio file,
%             predicted category of suspected event
% Returns:    Struct holding noise level, ground truth, predicted category
%             and noise class of suspected event
% Function creates and populates a struct with data for each suspected
% event
function z = Peak_Data(ind, ground, xml_datum, predicted)
    
    z = [];
   
    %store noise level of current event (5dB, 10dB, 15dB, 20dB, 25dB, 30dB)
    if mod(ind,8) == 0
        z.noise_level = 30;
    else
        z.noise_level = (mod(ind,8)-2)*5;
    end

    %store ground truth of current event
    z.ground_truth = ground;
    %store predicted label of current event
    z.predicted = predicted;

    %store background noise type of current audio file
    for l = 1:size(xml_datum.background,2)
        %if multiple classes of background noise are precent,
        %concatinate event class strings
        if l == 1
            z.noise_class = xml_datum.background{l}.SUBCLASS;
        else
            z.noise_class = strcat(z.noise_class,'/');
            z.noise_class = strcat(z.noise_class,xml_datum.background{l}.SUBCLASS);
        end
    end
end