% Parameters: directory of audio and xml files, percentage of training
%             files to be retained within training/validation set, total
%             number of files within which to split the processed data,
%             total number of training audio files to load, boolean
%             indicating whether to use validation set or test set, integer
%             dictating audio representation to use
% Returns:    struct of xml data for every event in each audio file, vector
%             storing indices of audio files within validation set
% Function loads audio and xml data from training set, identifying audio
% snippets holding events, computing gammatonegram representations. Structs
% holding event and noise representations and data are saved and meta data
% is returned

function [xml_data, val_set] = Process_Dataset(directory, training_percentage, num_files, tot_files, val_bool, rep)

    %Add path to gammatonegram library
    addpath(genpath('\gammatonegram'));

    %If training percentage is not a decimal, set to defaul value
    if training_percentage > 1 || training_percentage <= 0
        training_percentage = 0.8;
    end
    
    %if not using validation set, use all training data set as training
    %data
    if val_bool == false
        training_percentage = 1;
    end

    %Store directories for audio and xml data
    audio_dir = dir(strcat(directory,'\training\sounds\*.wav'));
    xml_dir = dir(strcat(directory,'\training\*.xml'));

    %Set window interval for gammatone representation (secs)
    gammatone_window = 0.003;
    %Set window sample with for spectrogram representation (samples)
    spectrogram_window = 1000;
    %Set sampling frequency of audio files
    Fs = 32000;

    %Initialise cell to hold struct of events
    events = {};
    %Set ration of noise snippets to keep/discard
    background_retention_rate = 0.5;
    %Initialise cell to hold xml data
    xml_data = cell(1,numel(xml_dir));

    %Load xml data from file
    for true_i=1:numel(xml_dir)
        xml_data{true_i} = xml_load(strcat(directory,'\training\', xml_dir(true_i).name));
    end

    %Initialise vector to store indices of validation sets
    val_set = [];
    %set counter to keep track of all files
    whole_i = 1;
    %set counter to keep track of all files not discarded (_00.wav, _01.wav files)
    trim_i = 1;
    
    %compute number of files after skipping first 2 audio files in each set
    reduced_files = tot_files - 2*ceil(tot_files/8);

    %Compute base number of audio files to store in each file to be saved
    base_count = floor(reduced_files/num_files);
    %Compute mod value to event remainder of first (mod_count) files
    mod_count = mod(reduced_files,num_files);

%%
    fprintf('Processing training and validation set\n');

    %loop through number of segments in which processed data will be saved
    for f = 1:num_files

        %compute number of audio files data to be saved within current file
        if f <= mod_count
            iter_max = base_count + 1;
        else
            iter_max = base_count;
        end

        %set counter to keep track of all trimmed audio files within
        %current file
        true_i = 1;
        %set counter to keep track of all audio files within current file
        file_i = true_i;

%%
        %loop through audio recordings to be stored in current file
        while file_i <= iter_max

            %extract xml data for current group of audio files
            if mod(whole_i,8) == 1
                xml_datum = xml_data{ceil(whole_i/8)};
            end

            %skip first two audio files of audio group (_00.wav / _01.wav)
            if mod(whole_i,8) == 1 || mod(whole_i,8) == 2
                true_i = true_i + 1;
                whole_i = whole_i + 1;
                continue;
            end

            %set variable to hold Boolean value for validation variable
            %within event and noise structs
            val = 0;

            %add current audio file to validation set with probability of 1 -
            %training_percentage
            if rand > training_percentage
                val = 1;
                val_set = [val_set whole_i];
            end

            fprintf('Processing file %d of %d\n',whole_i,tot_files);
            tic;

            %store background noise type of current audio file
            for l = 1:size(xml_datum.background,2)
                %if multiple classes of background noise are precent,
                %concatinate event class strings
                if l == 1
                    event_class_noise = xml_datum.background{l}.SUBCLASS;
                else
                    event_class_noise = strcat(event_class_noise,'/');
                    event_class_noise = strcat(event_class_noise,xml_datum.background{l}.SUBCLASS);
                end
            end

            %temporary variable
            last_time = 0;
            %set counter to keep track of noise events retained
            true_j = 1;

%%
            %loop through all events in current audio track
            for j = 1:size(xml_datum.events,2)

                %create struct to store event data
                x = struct;
                %create struct to store noise data
                y = struct;

                %store event start time, end time, class and noise class in
                %struct
                x.event_class = uint8(xml_datum.events{1,j}.CLASS_ID);
                
                %add offset to gunshot events
                if x.event_class == 3%xml_datum.events{1,j}.ENDSECOND - xml_datum.events{1,j}.STARTSECOND < 0.6
                    time_offset = 0.2;
                else
                    time_offset = 0;
                end
                                
                x.start_time = xml_datum.events{1,j}.STARTSECOND - time_offset;
                x.end_time = xml_datum.events{1,j}.ENDSECOND + time_offset;
                x.noise_class = event_class_noise;

                %store noise level of current event (5dB, 10dB, 15dB, 20dB, 25dB, 30dB)
                if mod(whole_i,8) == 0
                    x.noise_level = 30;
                    y.noise_level = 30;
                else
                    x.noise_level = (mod(whole_i,8)-2)*5;
                    y.noise_level = (mod(whole_i,8)-2)*5;
                end

                %store validation Boolean for current event
                x.validation = val;

                %extract audio snippet of event
                [x.Y,x.Fs] = audioread(strcat(directory,'\training\sounds\',audio_dir(whole_i).name), [round(x.start_time*Fs) floor(x.end_time*Fs)]);
                
                if rep == 1
                    %compute gammatone representation of audio snippet
                    x.D = flipud(gammatonegram(x.Y, x.Fs,gammatone_window*numel(x.Y)/x.Fs,0.0015*numel(x.Y)/x.Fs,150));
                elseif rep == 2
                    %compute linear spectrogram representation of audio
                    %snippet
                    [~,~,~,p] = spectrogram(x.Y,round(numel(x.Y)/400),[],300);
                    x.D = flipud(10*log10(abs(p)));
                elseif rep == 3
                    %compute log spectrogram representation of audio
                    %snippet
                    [x.D,~,~,~] = logfsgram(x.Y,2000,x.Fs,80);
                end
                    
                %save details for current event
                events{file_i,j} = x;

%%
                %Initialise start and end times
                next_time = x.start_time;
                y.start_time = last_time;
                y.end_time = next_time;

                %store 1 out of every 2 sections of background noise
                if rand > background_retention_rate

                    %Initialise validation Boolean and class for noise
                    y.validation = val;
                    y.noise_class = event_class_noise;

                    %extract audio snippet of background noise between events
                    if last_time == 0.0
                        [y.Y,y.Fs] = audioread(strcat(directory,'\training\sounds\',audio_dir(whole_i).name), [1 floor(next_time*Fs)]);
                    else
                        [y.Y,y.Fs] = audioread(strcat(directory,'\training\sounds\',audio_dir(whole_i).name), [round(last_time*Fs) floor(next_time*Fs)]);
                    end
                    
                    if rep == 1
                        %Compute gammatone representation of noise snippet
                        y.D = flipud(gammatonegram(y.Y,y.Fs,gammatone_window*numel(y.Y)/y.Fs,0.0015*numel(y.Y)/y.Fs,150));
                    elseif rep == 2
                        %compute linear spectrogram representation of audio
                        %snippet
                        [~,~,~,p] = spectrogram(y.Y,round(numel(y.Y)/400),[],300);
                        y.D = flipud(10*log10(abs(p)));
                    elseif rep == 3
                        %compute log spectrogram representation of audio
                        %snippet
                        [y.D,~,~,~] = logfsgram(y.Y,2000,y.Fs,80);
                    end

                    %save details for current noise snippet
                    noise{file_i,true_j} = y;

                    true_j = true_j + 1;
                end
                last_time = y.end_time;
            end
            true_i = true_i + 1;
            file_i = file_i + 1;
            trim_i = trim_i + 1;
            whole_i = whole_i + 1;
            toc;
        end
        %Save computed data to file, re-initialising event and noise
        %variables
        fprintf('Saving processed dataset part %d of %d...\n',f,num_files);
        save(strcat('Processed_Data_',num2str(f),'.mat'),'events');
        save(strcat('Processed_Data_Noise_',num2str(f),'.mat'),'noise');
        events = {};
        noise = {};
    end

    %Save meta data including xml data of events, set of indices of
    %validation set, total number of audio files
    save('Processed_Data.mat','xml_data','val_set', 'tot_files');
end