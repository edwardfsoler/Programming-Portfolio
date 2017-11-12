/* 
 * Wordifier.java
 *
 * Implements methods for iteratively learning words from a 
 * character-segmented text file, and then evaluating how good they are
 *
 * Students may only use functionality provided in the packages
 *     java.lang
 *     java.util 
 *     java.io
 * 
 * Use of any additional Java Class Library components is not permitted 
 * 
 * PUT BOTH OF YOUR NAMES HERE
 * October 2015
 *
 */

import java.util.*;
import java.io.*;
import java.lang.*;

public class Wordifier {
	
	//global variable for total number of tokens
	public static int totalCount;

    // loadSentences
    // Preconditions:
    //    - textFilename is the name of a plaintext input file
    // Postconditions:
    //  - A LinkedList<String> object is returned that contains
    //    all of the tokens in the input file, in order
    // Notes:
    //  - If opening any file throws a FileNotFoundException, print to standard error:
    //        "Error: Unable to open file " + textFilename
    //        (where textFilename contains the name of the problem file)
    //      and then exit with value 1 (i.e. System.exit(1))
	public static LinkedList<String> loadSentences( String textFilename )
	{
		try
		{
			//creates input stream
			InputStream in = new FileInputStream(textFilename);
			
			//set scanner stream to text file
			Scanner sc = new Scanner(new File(textFilename));
			
			LinkedList<String> ll = new LinkedList<String>();
			
			//keep on reading while theres an element to come
			
			while (sc.hasNext()) 
			{
				//load tokens into linked list
				ll.add(sc.next());
			}
			
			//close input stream
			in.close();
			return ll;
		}
		//catch exception
		catch(IOException e)
		{
			//print to error line
			System.err.println("Error: Unable to open file "+textFilename);
			System.exit(1);
			return null;
		}
		
	}
    // findNewWords
    // Preconditions:
    //    - bigramCounts maps bigrams to the number of times the bigram appears in the data
    //    - scores maps bigrams to its bigram product score 
    //    - countThreshold is a threshold on the counts
    //    - probabilityThreshold is a threshold on the bigram product score 
    // Postconditions:
    //    - A HashSet is created and returned, containing all bigrams that meet the following criteria
    //        1) the bigram is a key in bigramCounts
    //        2) the count of the bigram is >= countThreshold
    //        3) the score of the bigram is >= probabilityThreshold
    //      Formatting note: keys in the returned HashSet should include a space between the two tokens in the bigram
	public static HashSet<String> findNewWords( HashMap<String,Integer> bigramCounts, HashMap<String,Double> scores, int countThreshold, double probabilityThreshold )
	{
		//create new hash set
		HashSet hs = new HashSet();
		
		//copy keyset of bigramCounts hashmap to arraylist arr
		List<String> arr = new ArrayList<String>(scores.keySet());
		
		int i = 0;
		
		//loop through array of total bigrams
		while(i<arr.size())
		{
			//check if bigram meets criteria
			if((bigramCounts.containsKey(arr.get(i))) && (bigramCounts.get(arr.get(i)) >= countThreshold) && (scores.get(arr.get(i)) >= probabilityThreshold))
			{
				//add bigrams that meet criteria to hs hash set
				hs.add(arr.get(i));
			}
			
			i++;
		}
		
		return hs;
	}

    // resegment
    // Preconditions:
    //    - previousData is the LinkedList representation of the data
    //    - newWords is the HashSet containing the new words (after merging)
    // Postconditions:
    //    - A new LinkedList is returned, which contains the same information as
    //      previousData, but any pairs of words in the newWords set have been merged
    //      to a single entry (merge from left to right)
    //
    //      For example, if the previous linked list contained the following items:
    //         A B C D E F G H I
    //      and the newWords contained the entries "B C" and "G H", then the returned list would have 
    //         A BC D E F GH I
	public static LinkedList<String> resegment( LinkedList<String> previousData, HashSet<String> newWords )
	{		
		//create linked list ll
		LinkedList<String> ll = new LinkedList<String>();
		
		int i = 0;
		
		boolean manage = false;
		
		//loop through previousData
		while(i<previousData.size()-1)
		{
			manage = false;
		
			//test if newWords element equals element in list of bigrams
			if(newWords.contains(previousData.get(i)+" "+previousData.get(i+1)))
			{	
				//record that match was true
				manage = true;
			}
			
			if(manage)	
			{
				//join two tokens into single element
				previousData.set(i,previousData.get(i)+previousData.get(i+1));
				
				//remove second token from linked list
				previousData.remove(i+1);
			}
			
			//add element to linked list
			ll.add(previousData.get(i));
			
			i++;
		}
		
		return ll;
	}

    // computeCounts
    // Preconditions:
    //    - data is the LinkedList representation of the data
    //    - bigramCounts is an empty HashMap that has already been created
    // Postconditions:
    //    - bigramCounts maps each bigram appearing in the data to the number of times it appears
	public static void computeCounts(LinkedList<String> data, HashMap<String,Integer> bigramCounts ) {
		
		//loop through linked list
		for(int i = 0; i<data.size()-1; i++)
		{
			String temp = data.get(i)+" "+data.get(i+1);
			//adds elements to bigramCounts if they don't already exist there
			if(bigramCounts.containsKey(temp))
			{
				bigramCounts.put(temp,bigramCounts.get(temp)+1);
			}
			//increment count of element if it already is in the hashmap
			else
			{
				bigramCounts.put(temp,1);
			}
		}
		
		return;
	}

    // convertCountsToProbabilities 
    // Preconditions:
    //    - bigramCounts maps each bigram appearing in the data to the number of times it appears
    //    - bigramProbs is an empty HashMap that has already been created
    //    - leftUnigramProbs is an empty HashMap that has already been created
    //    - rightUnigramProbs is an empty HashMap that has already been created
    // Postconditions:
    //    - bigramProbs maps bigrams to their joint probability
    //        (where the joint probability of a bigram is the # times it appears over the total # bigrams)
    //    - leftUnigramProbs maps words in the first position to their "marginal probability"
    //    - rightUnigramProbs maps words in the second position to their "marginal probability"
	public static void convertCountsToProbabilities(HashMap<String,Integer> bigramCounts, HashMap<String,Double> bigramProbs, HashMap<String,Double> leftUnigramProbs, HashMap<String,Double> rightUnigramProbs ) {
		
		//copy bigramCounts keyset into an ArrayList
		List<String> poop = new ArrayList<String>(bigramCounts.keySet());
		
		int temp = 0;
		
		//create two new arraylists
		ArrayList<String> left = new ArrayList<String>();
		ArrayList<String> right = new ArrayList<String>();
		
		//loop through bigramCounts
		for(int i=0; i<bigramCounts.size(); i++)
		{
			//count total number of bigrams
			temp+=bigramCounts.get(poop.get(i));
		}
		
		double tempprob = 0.0;
		
		//loop through bigramCounts
		for(int i=0; i<bigramCounts.size(); i++)
		{
			//calculate individual probability of each bigram appearing
			tempprob = (double)(bigramCounts.get(poop.get(i)))/(double)(temp);
			
			//add bigram and probability to bigramProbs
			bigramProbs.put(poop.get(i), tempprob);
		}
		
		int index=0;
		
		//loop through bigramProbs
		for(int i=0; i<bigramProbs.size(); i++)
		{			
			//copy individual strings to character array
			char[] charArr = poop.get(i).toCharArray();
			
			//loop through each character array
			for(int j=0; j<charArr.length; j++)
			{
				//find position of whitespace
				if(Character.isWhitespace(charArr[j]))
				{	
					//store index of whitespace in char array
					index = j;
				}
				
			}
			
			//if leftUnigram is not in leftUnigramProbs then add new element and probability
			if(!leftUnigramProbs.containsKey(poop.get(i).substring(0,index)))
			{				
				//add leftUnigram and probability to hash map
				leftUnigramProbs.put(poop.get(i).substring(0,index), ((double)1/(double)temp));
			}
			//if leftUnigram is already in leftUnigramProbs, just recalculate probability
			else if(leftUnigramProbs.containsKey(poop.get(i).substring(0,index)))
			{
				//get probability and alter it
				leftUnigramProbs.put(poop.get(i).substring(0,index), (((double)leftUnigramProbs.get(poop.get(i).substring(0,index)))+((double)1/(double)temp)));
			}
			
			//if rightUnigram is not rightUnigramProbs then add new element and probability
			if(!rightUnigramProbs.containsKey(poop.get(i).substring(index+1)))
			{				
				//add rightUnigrama and rightUnigram probability to the rightUnigramProbs hash map
				rightUnigramProbs.put(poop.get(i).substring(index+1), ((double)1/(double)temp));
			}
			//if rightUnigram is already in rightUnigramProbs, just recalculate probability
			else if(rightUnigramProbs.containsKey(poop.get(i).substring(index+1)))
			{
				rightUnigramProbs.put(poop.get(i).substring(index+1), (((double)rightUnigramProbs.get(poop.get(i).substring(index+1)))+((double)1/(double)temp)));
			}
		}
		return;
	}

    // getScores
    // Preconditions:
    //    - bigramProbs maps bigrams to their joint probability
    //    - leftUnigramProbs maps words in the first position to their probability
    //    - rightUnigramProbs maps words in the last position to their probability
    // Postconditions:
    //    - A new HashMap is created and returned that maps bigrams to
    //      their "bigram product scores", defined to be P(w1|w2)P(w2|w1)
    //      The above product is equal to P(w1,w2)/sqrt(P_L(w1)*P_R(w2)), which 
    //      is the form you will want to use
	public static HashMap<String,Double> getScores( HashMap<String,Double> bigramProbs, HashMap<String,Double> leftUnigramProbs, HashMap<String,Double> rightUnigramProbs ) {
	
		//create new scores hashmap
		HashMap<String,Double> scores= new HashMap<String,Double>();
		
		//copy hashmap keysets for bigram and unigrams into arraylists
		List<String> leftArr = new ArrayList<String>(leftUnigramProbs.keySet());
		List<String> rightArr = new ArrayList<String>(rightUnigramProbs.keySet());
		List<String> totalArr = new ArrayList<String>(bigramProbs.keySet());
		
		double probability = 0.0;
		int index = 0;
		
		//find out left unigram and right unigram from bigram
		for(int i=0; i<bigramProbs.size(); i++)
		{			
			char[] charArr = totalArr.get(i).toCharArray();
			
			for(int j=0; j<charArr.length; j++)
			{
				//converting string into array of chars
				if(Character.isWhitespace(charArr[j]))
				{	
					//store index of whitespace to split up left and right unigrams
					index = j;
				}
			}
			
			//after seperating left and right unigrams from given bigram, call probabilities from hash maps by passing the keys and calculate the probability
			probability=((bigramProbs.get(totalArr.get(i)))/(Math.sqrt(leftUnigramProbs.get(totalArr.get(i).substring(0,index)) * rightUnigramProbs.get(totalArr.get(i).substring(index+1)))));

			//add bigrams and their probabilities to the scores hash map
			scores.put(totalArr.get(i), probability);
		}
		
		return scores;
	}
	

    // getVocabulary
    // Preconditions:
    //    - data is a LinkedList representation of the data
    // Postconditions:
    //    - A new HashMap is created and returned that maps words
    //      to the number of times they appear in the data
	public static HashMap<String,Integer> getVocabulary( LinkedList<String> data )
	{
		
		//create new hash map
		HashMap<String,Integer> hm = new HashMap<String,Integer>();
		
		//loop through given linked list
		for(int i = 0; i<data.size(); i++)
		{
			//if element is not already in the hashmap, then count and add to the hash map
			if(!hm.containsKey(data.get(i)))
			{
				//add element and its count to the hash map
				hm.put(data.get(i), 1);
			}
			//if element is already in hashmap
			else if(hm.containsKey(data.get(i)))
			{
				//retrieve count and increment
				hm.put(data.get(i),(1+hm.get(data.get(i))));
			}
		}
		
		totalCount = data.size();
		
		return hm;
	}

    // loadDictionary
    // Preconditions:
    //    - dictionaryFilename is the name of a dictionary file
    // Postconditions:
    //    - A new HashSet is created and returned that contains
    //      all unique words appearing in the dictionary
	public static HashSet<String> loadDictionary( String dictionaryFilename ) {
		
		try
		{
			//setup input stream
			InputStream in = new FileInputStream(dictionaryFilename);
			
			//set chanel to read from input file
			Scanner sc = new Scanner(new File(dictionaryFilename));
			
			HashSet<String> hs = new HashSet<String>();
			
			//continues reading elements until none are left
			while (sc.hasNext()) 
			{
				String temp = sc.next();
				
				if(!hs.contains(temp))
				{
					//adds elements to hash set hs if they are not already in it
					hs.add(temp);
				}
			}
			
			//close input stream
			in.close();
			return hs;
		}
		//catch exception
		catch(IOException e)
		{
			//print error message to error display
			System.err.println("Error: Unable to open file "+dictionaryFilename);
			System.exit(1);
			return null;
		}
			
	}

    // incrementHashMap
    // Preconditions:
    //  - map is a non-null HashMap 
    //  - key is a key that may or may not be in map
    //  - amount is the amount that you would like to increment key's value by
    // Postconditions:
    //  - If key was already in map, map.get(key) returns amount more than it did before
    //  - If key was not in map, map.get(key) returns amount
    // Notes:
    //  - This method has been provided for you 
	private static void incrementHashMap(HashMap<String,Integer> map,String key,int amount) {
		if( map.containsKey(key) ) {
			map.put(key,map.get(key)+amount);
		} else {
			map.put(key,amount);
		}
		return;
	}

    // printNumWordsDiscovered
    // Preconditions:
    //    - vocab maps words to the number of times they appear in the data
    //    - dictionary contains the words in the dictionary
    // Postconditions:
    //    - Prints each word in vocab that is also in dictionary, in sorted order (alphabetical, ascending)
    //        Also prints the counts for how many times each such word occurs
    //    - Prints the number of unique words in vocab that are also in dictionary 
    //    - Prints the total of words in vocab (weighted by their count) that are also in dictionary 
	// Notes:
    //    - See example output for formatting
	public static void printNumWordsDiscovered( HashMap<String,Integer> vocab, HashSet<String> dictionary ) {
		
		//convert key set to array of string
		List<String> vocabArr = new ArrayList<String>(vocab.keySet());
		
		//alphabetically sort array of vocabArr
		Collections.sort(vocabArr);
		
		System.out.println("Word and frequency");
		
		int counter2 = 0;
		
		//loop through vocabArr
		for(int i=0; i<vocabArr.size(); i++)
		{
			//compare vocab Strings to dictionary Strings
			if(dictionary.contains(vocabArr.get(i)))
			{
				//count total tokens output for later use
				counter2+=vocab.get(vocabArr.get(i));
				
				System.out.println("Discovered "+vocabArr.get(i)+" (count "+vocab.get(vocabArr.get(i))+")");
			}
		}
		
		int counter = 0;
		
		//arraylist of unique words
		ArrayList<String> al = new ArrayList<String>();
		
		for(int j=0; j<vocabArr.size(); j++)
		{
			//add words to array list if they are not already in it
			if(!al.contains(vocabArr.get(j)))
			{
				al.add(vocabArr.get(j));
			}
		}
		
		//loop through vocabArr	
		for(int i=0; i<al.size(); i++)
		{
			//compare vocabArr Strings to dictionaryArr String
			if(dictionary.contains(al.get(i)))
			{
				//increment counter
				counter ++;
			}
		}
		
		System.out.println();
		
		//output unique word count
		System.out.println("Discovered "+counter+" (unique) words out of "+dictionary.size()+" dictionary words ("+String.format( "%.2f",100*((double)counter/(double)dictionary.size()))+"%)");
		
		//output total token count
		System.out.println("Discovered "+counter2+" actual word tokens out of "+totalCount+" total tokens ("+String.format( "%.2f",100*((double)counter2/(double)totalCount))+"%)");
		
		return;
	}
}
