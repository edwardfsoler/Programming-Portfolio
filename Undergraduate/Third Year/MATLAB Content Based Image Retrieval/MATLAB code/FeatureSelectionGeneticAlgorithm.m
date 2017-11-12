% Genetic algorithm for the selection of the best subset of features
% It requires the libsvm which can be downloaded from http://www.csie.ntu.edu.tw/~cjlin/libsvm/
% Author: George Azzopardi
function bestchromosome = FeatureSelectionGeneticAlgorithm(train_feats,train_labels)

%Initialising variables
nchroms = 100; %Chromosomes
nepochs = 10; %Number of generations of population
nparentsratio = 0.2; %20% of parents picked
mutateprob = 1; %Propapility rate of permutation. Originally 0.1

figure;hold on;
colorlist = jet(nepochs);
[lbls,h] = grp2idx(train_labels);

for epoch = 1:nepochs
    fprintf('epoch %d of %d\n',epoch,nepochs);
    if epoch == 1
        pop = generateInitialPopulation(nchroms,size(train_feats,2)); %Generate initial population   
    else
        pop = getnewpopulation(pop,score,nparentsratio,mutateprob);
    end
    
    pop = logical(pop);
    
    score = zeros(1,nchroms);
    for i = 1:nchroms
        %score0(i) = DunnIndex(train_feats(:,pop(i,:)),lbl,15);
        score(i) = getScore(pop(i,:),train_feats,lbls);    
    end    
    plot(sort(score,'descend'),'color',colorlist(epoch,:));
    pause(1);
end
[~,mxind] = max(score);
bestchromosome = pop(mxind,:);

function newpop = getnewpopulation(pop,score,nparentsratio,mutateprob)

[~,ind] = sort(score,'descend');
nparents = nparentsratio * size(pop,1); %Number of parents to keep 

newpop = zeros(size(pop)); %Initialise new population
newpop(1:nparents,:) = pop(ind(1:nparents),:); %First 20 are top form previous 

topparents = pop(ind(1:nparents),:);

%Filling the other 80 (21 to 100)
%Select random parents
%First two randomly selected parents
%Get offspring from parents
for j = nparents+1:size(pop,1)
    randparents = randperm(nparents);    
    newpop(j,:) = getOffSpring(topparents(randparents(1),:),topparents(randparents(2),:),mutateprob);    
end

function offspring = getOffSpring(parent1,parent2,mutateprob)
%Chose cut off point randomly excluding first and last
%Left from 1st and Right from 2nd parent 
idx = ceil(rand(1)*(numel(parent1)-2))+1;
offspring = [parent1(1:idx),parent2(idx+1:end)];
%Mutation, according to mutateprob you invert
mutateidx = rand(1,numel(parent1)) < mutateprob;
offspring(mutateidx) = ~logical(offspring(mutateidx));


function score = getScore(chromosome,train_feats,labels) %Getting fitness function
%Getting accuracy by partitinging into blocks (Kfold2)
cv = cvpartition(labels,'Kfold',2);
%First block testing, second training
%Done again second block testing, first traingng
%Mean of blocks
for i = 1:cv.NumTestSets    
    c = knnclassify(train_feats(cv.test(i),chromosome),train_feats(cv.training(i),chromosome),labels(cv.training(i)));
    acc(i) = sum(c == labels(cv.test(i)))/numel(c);
end
%fitness = 10^4 * accuracy * 0.4 * #zeros
% %Zeros calculation is code added after
% for i = 1:13
%     noOfZeros = sum(chromosome(i) == 0);
score = mean(acc) + noOfZeros; %Original score = mean(acc)
%score = svmtrain(labels,train_feats(:,chromosome),'-s 0 -t 0 -v 2 -q');

function pop = generateInitialPopulation(n,ndim)
pop = zeros(n,ndim);
pop(1,:) = ones(1,ndim);
for i = 2:n
    %pop(i,:) = round(rand(1,ndim));
    pop(i,randperm(ndim,mod(i,round(ndim/2))+1)) = 1;
end