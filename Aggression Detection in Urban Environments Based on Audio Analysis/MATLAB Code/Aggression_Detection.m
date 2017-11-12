% Main System Function
%-------Pipeline------
% - Loads training audio files, sorting and computing gammatone
%   representation of events
% - Computes Bag of Words vocabulary based on training data
% - Computes Bag of Sifts for each training event
% - Locates suspected events within test data, computing gammatone
%   representations for each suspected event
% - Computes Bag of Sifts for each suspected event
% - Performs classification by SVM
% - Evaluates accuracy of system
function Aggression_Detection

    warning off;
    
    %load VL_Feat Library
    run('vlfeat-0.9.20/toolbox/vl_setup');

    %Load directory of training audio
    audio_dir = dir('..\DataSet\MIVIA_DB4_dist\training\sounds\*.wav');
    
    %set file directory
    directory = '..\DataSet\MIVIA_DB4_dist';

    %number of files in which to training data shall be segmented to
    %accomodate for memory requirements of the data set
    num_files = 15;

    %number of audio files to load from training set
    tot_files = round(numel(audio_dir));
    
    %Set representation of features
    %representation = 'BagOfWords';
    representation = 'VLAD';
    
    %set size of vocabulary
    vocab_size = 50;
    
    %set whether to use validation set (true) or test set(false)
    val_bool = true;
    
    %set representation to be used (1 = gammatonegram, 2 = linear
    %spectrogram, 3 = log spectrogram)
    rep = 3;
    
    %set size of neighbourhood (seconds) in which to perform reduction
    neighbourhood_size = 3;
    
    %set comparison method for neighbourhood reduction (1 = SVM score, 2 =
    %localisation score)
    comparator = 2;
    
    %set number of levels for spatial pyramid
    pyramid_levels = 3;

%%
    if ~exist('Processed_Data.mat','file')
        %call function to load training data, segment events and compute event
        %representations
        fprintf('Processing training data\n');
        [xml_data, val_set] = Process_Dataset(directory, 0.8, num_files, tot_files, val_bool, rep);
    else
        %Load processed data
        fprintf('Loading processed dataset...\n');
        load('Processed_Data.mat');
    end

%%
    if ~exist('Gammatone_Vocab.mat','file')
        %Function builds vocabulary from training data
        fprintf('Computing vocabulary\n');
        vocab = Build_Vocab(num_files, tot_files, vocab_size);
    else
        %Load vocabulary
        fprintf('Loading vocabulary...\n');
        load('Gammatone_Vocab.mat');
    end

%%
    if ~exist('Bag_Of_Sifts.mat','file')
        %Function computes bag of sifts from training data and vocabulary
        fprintf('Computing bag of sifts\n');
        bags_training = Get_Bag_Of_Sifts(vocab, num_files, tot_files, representation, pyramid_levels);
    else
        %Load bag of sifts
        fprintf('Loading bag of sifts...\n');
        load('Bag_Of_Sifts.mat');
    end

%%
    if ~exist('Localisation_Data.mat','file')
        %Function locates suspected events from test data
        fprintf('Locating events in test data\n');
        [peak, track_length] = Localisation(directory, xml_data, val_set, val_bool, rep);
    else
        %Load localisation data
        fprintf('Loading localisation data...\n');
        load('Localisation_Data.mat');
    end

%%
    if ~exist('Bag_Of_Localised.mat','file')
        %Function computes bag of sifts for suspected events
        fprintf('Computing bag of sifts for localised events\n');
        [bags, labels] = Bag_Test_Data(peak, vocab, representation, pyramid_levels);
    else
        %Load bag of sifts for localised data
        fprintf('Loading Bag of sifts for localised events\n');
        load('Bag_Of_Localised.mat');
    end

%         lamb = [0.1 0.001 0.0001 0.00001];
%         kern = {'polynomial', 'KCHI2', 'linear', 'quadratic', 'rbf', 'mlp'};
%         accuracy = [];
%     
%         for i=1:size(lamb,2)
%             for j=1:size(kern,2)
%                 predicted_categories = SVM(bags_training, bags, lamb(i), kern{j});
%                 load('Localisation_Data.mat');
%                 [peak, predicted_categories] = Neighbourhood_Reduction(peak, track_length, predicted_categories, val_set, 3);
%                 C = ConfusionMat(predicted_categories,labels, peak);
%                 accuracy = [accuracy Results(xml_data, predicted_categories, peak, val_set)];
%             end
%         end
%     
%         [xx,yy] = max(accuracy);
%     
%         fprintf('Max accuracy of %d achieved at index %d\n',xx,yy);

%%
    if ~exist('Predicted_Categories.mat','file')
        if strcmp(representation,'BagOfWords')
            %Function applies SVM for test data classification
            fprintf('Classifying test data with SVM\n');
            %Set lambda value for SVM
            lambda = 0.001;
            %Set kernel for SVM
            kernel = 'HistogramIntersection';
            predicted_categories = SVM(bags_training, bags, lambda, kernel, size(vocab,1));
        elseif strcmp(representation,'VLAD')
            predicted_categories = KNN(bags_training,bags);
        end
        save('Predicted_Categories.mat','predicted_categories');
    else
        %Load predicted categories for test data
        fprintf('Loading predicted categories...\n');
        load('Predicted_Categories.mat');
    end

%%
    %Reduce neighbourhood of suspected events, ruling out predicted events
    %with weak responses
    [peak, predicted_categories] = Neighbourhood_Reduction(peak, track_length, predicted_categories, neighbourhood_size, comparator);
    
    %Evaluate results of system, returning ground truth and evalluation
    %sheet
    ground_labels = Results(xml_data, directory, predicted_categories, peak, val_set, val_bool);
    
    %plot confusion matrix for test data
    Confusion_Matrix(predicted_categories,ground_labels);
end