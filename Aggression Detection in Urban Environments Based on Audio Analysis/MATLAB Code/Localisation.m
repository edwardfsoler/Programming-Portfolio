% Parameters: directory of audio and xml data, xml data of training data,
%             vector of indices of audio files within the validation set,
%             boolean indicating whether to use validation set or test set,
%             integer dictating image representation to be used
%             (Gammatonegram, Linear-Spectrogram, Log-Spectrogram)
% Returns: cell of data for all suspected events, vector of track lengths
%          for each track
% Function applies line detection to a gammatone representation of a test
% audio file. The response is summed and search for peaks (maxima and
% minima). These peaks and their representations are listed as suspected events.
function [peak, track_length] = Localisation(directory, xml_data, val_set, val_bool, rep)

    %Add path for BCOSFIRE line detection
    addpath(genpath('v1.2\'));
    
    if val_bool == true
        %directories for audio and xml data
        audio_dir = dir(strcat(directory,'\training\sounds\*.wav'));
        num_files = round(numel(audio_dir));
        
        %Initialise cells to store audio data and frequency data
        Y_test = cell(1,numel(val_set));
        Fs_test = cell(1,numel(val_set));

        %Initialise cell to store track lengths of each audio file
        track_length = cell(1,numel(val_set));
    else
        %directories for audio and xml data
        audio_dir = dir(strcat(directory,'\testing\sounds\*.wav'));  
        xml_dir = dir(strcat(directory,'\testing\*.xml'));
        num_files = round(numel(audio_dir));
        
        %Initialise cells to store audio data and frequency data
        Y_test = cell(1,num_files);
        Fs_test = cell(1,num_files);

        %Initialise cell to store track lengths of each audio file
        track_length = cell(1,num_files);
        
        %Initialise cell to hold xml data of test files
        xml_data = cell(1,numel(xml_dir));

        %Load xml testing data from file
        for true_i=1:numel(xml_dir)
            xml_data{true_i} = xml_load(strcat(directory,'\testing\', xml_dir(true_i).name));
        end
    end

    %Initialise struct to store suspected events data
    x = struct;
    %Set window for gammatonegram computation to 0.003 seconds
    gammatone_window = 0.003;
    %Initialise cell to store suspected events
    peak = {};
    %Initialise upperbound for search of validation audio files
    max_iter = num_files;
    %Set counter to keep track of all audio files
    i = 1;
    %Set counter to keep track of audio files in validation set
    true_i = i;
    %Set value for window (secs) when computing gammatonegram
    %representation
    
    %%
    %Loop through all audio files
    while i <= max_iter
        
        %extract xml data for current group of audio files
        if mod(i,8) == 1
            if ceil(i/8) < size(xml_data,2)
                xml_datum = xml_data{ceil(i/8)};
            else
                break;
            end
        end
        
        %if current audio file is not in validation set, skip
        if val_bool == true && numel(val_set(val_set == i)) < 1
            i = i+1;
            max_iter = max_iter + 1;
            continue;
            %if current audio file is first or second in test set, skip
        elseif val_bool == false && (mod(i,8) == 1 || mod(i,8) == 2)
            i = i+1;
            max_iter = max_iter + 1;
            continue;
        end
        
        %load test audio file
        if val_bool == true
            [Y_test{true_i}, Fs_test{true_i}] = audioread(strcat(directory,'\training\sounds\',audio_dir(i).name));
        else
            [Y_test{true_i}, Fs_test{true_i}] = audioread(strcat(directory,'\testing\sounds\',audio_dir(i).name));
        end
        
        %store track length of current audio file
        track_length{true_i} = size(Y_test{true_i},1)/Fs_test{true_i};
        
        %compute gammatonegram for audio input
        d = flipud(gammatonegram(Y_test{true_i}, Fs_test{true_i},gammatone_window*numel(Y_test{true_i})/Fs_test{true_i},0.0015*numel(Y_test{true_i})/Fs_test{true_i},1000));
                
        %% Symmetric filter params
        symmfilter = struct();
        symmfilter.sigma     = 2;
        symmfilter.len       = 10;
        symmfilter.sigma0    = 3;
        symmfilter.alpha     = 0.001;

        %% Asymmetric filter params
        asymmfilter = struct();
        asymmfilter.sigma     = 1.8;
        asymmfilter.len       = 22;
        asymmfilter.sigma0    = 2;
        asymmfilter.alpha     = 0.1;
        
        %compute line detection with current sigma parameter value
        response = BCOSFIRE(d, symmfilter, asymmfilter);

        %compute sum of line response
        sum_resp = sum(response);

        %Create thresholded copy of summed response
        sum_resp_thresholded = sum_resp;
        sum_resp_thresholded(sum_resp < 2500) = 0;

        %locate peaks of summed response
        [mxlocs,mnlocs] = Get_Peaks(sum_resp_thresholded);

        %Set counter to keep track of all peaks within the summed
        %response
        j = 1;
        %Set counter to keep track of trimmed set of peaks within
        %summed response
        true_j = 1;
        %Set counter to keep track of correctly classified snippets
        corr = 0;
        %Set vector to keep track of all visited snippets
        vis = [];

        %%
        %loop through all peaks within current audio file
        while j <= size(mxlocs,2)
            
            %store indices of correct start end points within mnlocs vector
            after = find(mnlocs > mxlocs(j),1,'first');
            before = find(mnlocs < mxlocs(j),1,'last');

            %if no minimum points exist before or after current maximum,
            %ignore
            if numel(after) == 0  || numel(before) == 0
                j = j+1;
                continue;
            end

            %switch from storing indices to storing actual values
            after = mnlocs(after);
            before = mnlocs(before);

            %store distribution of summed response
            x.resp_dist = normr(sum_resp_thresholded(before:after));

            %compute segment ratios between point values
            for k = 1:size(x.resp_dist,2)-1
                x.resp_dist(k) = (x.resp_dist(k) + x.resp_dist(k+1))/2;
            end
            x.resp_dist(k) = [];

           %store center time of suspected event in seconds
           x.center = (mxlocs(j)/size(sum_resp_thresholded,2))*track_length{true_i};

           %store end time of suspected event in seconds
           x.end = (after/size(sum_resp_thresholded,2))*track_length{true_i};% + time_offset;

           %store start time of suspected event in seconds
           x.start = (before/size(sum_resp_thresholded,2))*track_length{true_i};% - time_offset;

           %store background noise type of current audio file
           for l = 1:size(xml_datum.background,2)
                if l == 1
                    x.event_class_noise = xml_datum.background{l}.SUBCLASS;
                else
                    x.event_class_noise = strcat(x.event_class_noise,'/');
                    x.event_class_noise = strcat(x.event_class_noise,xml_datum.background{l}.SUBCLASS);
                end
           end

           x.event_class = [];

           %store true label of current suspected event
           for k = 1:size(xml_datum.events,2)
                if x.center >=  xml_datum.events{1,k}.STARTSECOND...
                && x.center <=  xml_datum.events{1,k}.ENDSECOND ...
                && numel(vis(vis == xml_datum.events{1,k}.STARTSECOND)) == 0
                   x.event_class = xml_datum.events{1,k}.CLASS_ID;
                   %update vector of visited snippets with start second
                   %of current suspected event
                   vis = [vis xml_datum.events{1,k}.STARTSECOND];
                   break;
                end
           end

           %if event class hasn't been assigned, then consider to be
           %background noise
           if isempty(x.event_class)
               x.event_class = 1;
           end

           %extract audio data of suspected event between start and end
           %time
           Fs = Fs_test{true_i};
           Y = Y_test{true_i}(x.start*Fs:x.end*Fs);
           
           if rep == 1
               %produce gammatonegram representation
                x.gamma = flipud(gammatonegram(Y,Fs,gammatone_window*numel(Y)/Fs,0.0015*numel(Y)/Fs,150));
           elseif rep == 2       
               %produce linear spectrogram representation
               [~,~,~,p] = spectrogram(Y,round(numel(Y)/400),[],300);
               x.gamma = flipud(10*log10(abs(p)));
           elseif rep == 3
               %produce log spectrogram representation
               [x.gamma,~,~,~] = logfsgram(Y,2000,Fs,80);
            end
           
           %store actual index of current audio file
           x.audio_ind = i;
           %store localisation response score
           x.localisation_score = sum_resp_thresholded(mxlocs(j));

           %store suspected event 
           peak{true_i,true_j} = x;

           j = j + 1;
           true_j = true_j + 1;
        end
        
        i = i + 1;
        true_i = true_i + 1;
    end

    %Save localisation data
    fprintf('Saving localisation data...\n');
    save('Localisation_Data.mat','peak','track_length');
end

% Parameters: Summed responsed of line detection
% Returns: Locations of maxima and minima within the summed response
% Function analyses the summed line detection response, returning locations
% of both maximal and minimal peaks
function [mxlocs, mnlocs] = Get_Peaks(signal)
    %find max peaks
    [~,mxlocs] = findpeaks(signal);%,'MinPeakDistance',10);
    %Invert response
    invertsignal = max(signal(:)) - signal;
    %find min peaks
    [~,mnlocs1] = findpeaks(invertsignal);
    %find trailing zero values
    mnlocs2 = find(diff(signal == 0) < 0);
    %merge list of min locs into one sorted list
    mnlocs = sort([mnlocs1 mnlocs2]);
end