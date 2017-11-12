import random
from nltk.util import unique_list
from nltk import FreqDist
from nltk.corpus import conll2000 as cs
from nltk import word_tokenize
def datasets():
    #select random number for random selection of data sets
    start = random.random()
    if start > 0.9:
        start = 0.9

    end = start+0.1
    #generate list of tagged data from corpus
    words = cs.tagged_words(tagset='universal')
    startn = round(len(words)*start)
    endn = round(len(words)*end)#segment words into two data sets
    training = words[:startn]+words[endn:]
    testing = words[startn+1:endn-1]
    #return data sets
    return [training, testing]

def unigramtrainingset(a):
    # create frequency distribution of word, tag pairs in the training set
    fd = FreqDist(a)
    # seperate words from tags
    x = [y[0] for y in a]
    # create frequency distribution of words in the training set
    fd2 = FreqDist(x)
    # create list of unique words
    words = unique_list([x[0] for x in fd])
    # create list of unique tags (all possible tags)
    tags = ['NOUN', 'ADP', 'ADV', 'NUM', 'VERB', '.', 'PRON', 'DET', 'ADJ', 'PRT', 'CONJ']
    # initialise output list
    out = []

    # loop through each unique word
    for word in words:
        # reinitialise tagso list
        tagso = []
        # store frequency of current word
        denom = fd2.freq(word)
        # loop through each tag
        for tag in tags:
            # compute probability of current tag being paired with current word
            prob = fd.freq((word, tag)) / denom
            # create list of tag, probability pairs
            tagso.append((tag, prob))
        # append word, tag-probabilities to out list
        out.append((word, tagso))
    return out

def bigramTags(a):
    #create lit of all tags
    tags = [x[1] for x in a]
    #create list of tag bigrams
    btags = [(tags[i],tags[i+1]) for i in range(len(tags) -1)]
    #create frequency distribution of bigram tags
    btagsf = FreqDist(btags)
    #create list of unique bigram tags
    btagscombo = [(x,y) for x in unique_list(tags) for y in unique_list(tags)]
    out = []
    #loop through unique bigram tags
    for i in range(len(btagscombo)):
        #add bigran tag with frequency probability to list
        out.append((btagscombo[i], btagsf.freq(btagscombo[i])))
    return out

def viterbiTagger(tags, words, input):
    tags2 = ['NOUN', 'ADP', 'ADV', 'NUM', 'VERB', '.', 'PRON', 'DET', 'ADJ', 'PRT', 'CONJ']
    #tokenise input
    observations = word_tokenize(input)

    #declare and initialise arrays
    viterbi = [[0 for x in range(len(observations))] for y in range(len(tags2) + 2)]
    backpointer = [[0 for x in range(len(observations))] for y in range(len(tags2) + 2)]
    #create list of tag probability pairs for first token
    tstemp = [x[1] for x in words if x[0] == observations[0]]
    #if first word is unknown then enter
    if tstemp == []:
        #assign default tag as noun
        viterbi[0][0] = 1.0
        backpointer[0][0] = 0
    else:
        # extract nested list
        ts = tstemp[0]

        #assign each path a weight based on the unigram tag probability
        for s in range(len(tags2)):
            viterbi[s][0] = ts[s][1]
            backpointer[s][0] = (0,0)

    tstemp = []
    #loop through observations
    for t in range(1, len(observations)):
        #get tag-probability pairs for current observation
        tstemp = [x[1] for x in words if x[0] == observations[t]]
        #if unseen token found enter
        if tstemp == []:
            #set token type to noune (default)
            viterbi[0][t] = 1.0
            temp = []
            for s2 in range(len(tags2)):
                # create bigram of tags from previous tag to noun
                transtags = (tags2[s2], tags2[0])
                # get probability of bigram of tags
                transprob = [x[1] for x in tags if x[0] == transtags][0]
                # add probability to list
                temp.append(viterbi[s2][t - 1] * transprob)
            # find index of previously most probable path
            ind = temp.index(max(temp))
            # store index of previously most probable path
            backpointer[0][t] = (ind, t - 1)
            continue
        ts = tstemp[0]
        #loop through possible tags
        for s in range(len(tags2)):
            temp = []
            #loop through  possible tags
            for s2 in range(len(tags2)):
                #create bigram of tags
                transtags = (tags2[s2],tags2[s])
                #get probability of bigram of tags
                transprob = [x[1] for x in tags if x[0] == transtags][0]
                #add probability to list
                temp.append(viterbi[s2][t-1]*transprob)
            #set index to store porbability of most likely bigram series multiplied by probability of current tag in current observation
            viterbi[s][t] = max(temp)*ts[s][1]
            #find index of previously most probable path
            ind = temp.index(max(temp))
            #store index of previously most probable path
            backpointer[s][t] = (ind,t-1)

    temp = []
    #combile list of final observation values
    for i in range(len(tags2)):
        temp.append(viterbi[i][len(observations)-1])
    #store index of final tag selected
    index = (temp.index(max(temp)),len(observations)-1)

    out = []
    next = index
    for i in range(len(observations)):
        x = next[0]
        y = next[1]
        out.insert(0,tags2[x])
        next = backpointer[x][y]
    return out

def tag(input):
    a2, b = datasets()
    a = a2[:200000]
    tags = bigramTags(a)
    words = unigramtrainingset(a)
    return viterbiTagger(tags,words,input)

#tag('Bill saw that man yesterday')

a,b2 = datasets()
#reduce test data size
b = b2[:1000]
#extract words from test input
testinput = [x[0] for x  in b]
#form sentences from test input list
testinput = ' '.join(testinput)
#extract tags from test input
testtag = [x[1] for x  in b]
#retrieve results from tag function
testoutput = tag(testinput)

#count number of correct occurences
correct = 0
for i in range(len(testtag)):
    if testtag[i] == testoutput[i]:
        correct = correct + 1
#calculate accuracy rate
accuracy = correct/len(testoutput)
#output test input
print(testinput)
#output resulting tags
print(testoutput)
#output corpus tags
print(testtag)
#output accuracy rate
print(accuracy)