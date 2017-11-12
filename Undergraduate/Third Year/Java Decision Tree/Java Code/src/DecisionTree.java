import java.io.FileInputStream;
import org.apache.commons.io.*;
import java.util.*;

public class DecisionTree {
	public static void main(String[] args)
	{
		//Store contents of data file as string
		String input = readFile("Dataset.txt");
		String testInput = readFile("TestData.txt");
		
		//Store input data in linked list
		LinkedList<LinkedList<String>> data = getData(input);
		LinkedList<LinkedList<String>> testData = getData(testInput);
		 
		LinkedList<String> attributes = new LinkedList<String>();
		//extract attributes from first row of data
		for(int i = 0; i < data.get(0).size()-1; i++)
		{
			attributes.add(data.get(0).get(i));
		}
		
		LinkedList<LinkedList<String>> uniqueVals = new LinkedList<LinkedList<String>>();
		//extract unique values for each column
		for(int i = 0; i < data.get(0).size(); i++)
		{
			LinkedList<String> uniqueVals2 = new LinkedList<String>();
			
			for(int j = 1; j < data.size(); j++)
			{
				if(uniqueVals2.size() == 0)
				{
					uniqueVals2.add(data.get(j).get(i));
				}
				else if(!uniqueVals2.contains(data.get(j).get(i)))
				{
					uniqueVals2.add(data.get(j).get(i));
				}
			}
			uniqueVals.add(uniqueVals2);
		}
		
		//construct decision tree
		Node root = ID3(data,uniqueVals,data.get(0).getLast(),attributes);
		DFSPrint(root,0);
		//test examples against decision tree to compute target outcome
		outputResults(testData, root, attributes, data.get(0).getLast());
	}
	
	/* Arguments: Root node and number of tabs
	 * Returns: void
	 * Performs depth first traversal of decision tree, outputting attribute values
	 */
	public static void DFSPrint(Node root, int tabs)
	{
		//if leaf node reached, output value
		if(root.getChildren().size() == 0 && (root.getName().toLowerCase().equals("yes") || root.getName().toLowerCase().equals("no")))
		{
			//print tabs to indent output 'tabs' times
			for(int i = 0; i < tabs; i++)
			{
				System.out.print("\t");
			}
			//output leaf node value
			System.out.println(root.getName().toUpperCase());
			return;
		}
		
		//loop through all children of current node
		for(int i = 0 ; i < root.getChildren().size(); i++)
		{
			//print 'tabs' tabs
			for(int j = 0; j < tabs; j++)
			{
				System.out.print("\t");
			}
			//output attribute name and value of current branch
			System.out.println("if "+root.getName()+" = "+root.getBranches().get(i));
			//recursively call DFS passing child node and incrementing the number of tabs
			DFSPrint(root.getChildren().get(i),tabs + 1);
		}
	}
	
	/* Arguments: Two double values
	 * Returns: Entropy value
	 * Computes entropy value given two input functions
	 */
	public static double entropy(double x, double y)
	{
		if(x == 0 || y == 0)
		{
			return 1;
		}
		double total = x + y;
		//entropy calculation
		return -(x/total)*(Math.log(x/total)/Math.log(2)) - (y/total)*(Math.log(y/total)/Math.log(2));
	}
	
	/* Arguments: Input file name
	 * Returns: String variable holding text contents of file
	 * Function reads contents of input data set text file and returns a String variable 
	 */
	public static String readFile(String filename)
	{
		String fileData = "";
		try
		{
			//create file input stream
			FileInputStream in = new FileInputStream(filename);
			//convert stream data to string
			fileData = IOUtils.toString(in);
		}
		//catch exceptions
		catch(Exception e)
		{
			System.err.println(e);
		}
		//return file data as string
		return fileData;
	}
	
	/* Arguments: String variable holding contents of input data file
	 * Returns: Linked List of Linked List of String acting as a table holding the contents of the input file
	 * Function segments and organises the string input, creating a table like data structure
	 */
	public static LinkedList<LinkedList<String>> getData(String input)
	{
		LinkedList<LinkedList<String>> examples = new LinkedList<LinkedList<String>>();
		//remove excess white spaces at beginning and end
		input = input.trim();
		//replace all occurences of ',' with a white space
		input = input.replace(',', ' ');
		int  i = 0, j = 0;
		
		//loop through input string
		while(i < input.length())
		{
			//create new 'row'
			LinkedList<String> values = new LinkedList<String>();
			
			//take note of new lines to create new 'row'
			while(i < input.length() && input.charAt(i) != '\n' && i < input.length())
			{
				//take note of white space to seperate input values
				while(i < input.length() && input.charAt(i) != ' ' && input.charAt(i) != '\r')
				{
					i++;
				}
				//create new column value
				values.add(input.substring(j,i));
				i = i + 2;
				j = i;
				
				//if end of current line has been reached, cycle to next line
				if(i < input.length() && input.charAt(i-2) == '\r')
				{
					break;
				}
			}
			//add current 'row' to 'table'
			examples.add(values);
		}
		//return table like structure with data
		return examples;
	}

	/* Arguments:
	 * - data: Linked List of Linked List of Strings holding all the input data including the first row
	 * - uniqueVals: Linked List of Linked List of Strings holding all unique column values
	 * - targetAttribute: String name of target attribute
	 * - attributes: Set of all attributes
	 * Returns: Root node of decision sub-tree
	 * Function recursively creates decision tree structure by comparing information gain of each attribute and creating nodes.
	*/
	public static Node ID3(LinkedList<LinkedList<String>> data, LinkedList<LinkedList<String>> uniqueVals, String targetAttribute, LinkedList<String> attributes)
	{
		if(data.size() < 2)
		{
			System.err.println("Something went wrong here");
			return null;
		}
		
		int totyes = 0, totno = 0;
		
		//calculate total number of yes/no example outcomes for table
		for(int i = 1; i< data.size(); i++)
		{
			if(data.get(i).getLast().toLowerCase().equals("yes"))
			{
				totyes++;
			}
			
			if(data.get(i).getLast().toLowerCase().equals("no"))
			{
				totno++;
			}
		}
		
		//if all example values are yes, set root value to yes
		if(totyes == data.size()-1)
		{
			Node root = new Node("Yes");
			return root;
		}
		
		//if all example values are no, set root value to no
		if(totno == data.size()-1)
		{
			Node root = new Node("No");
			return root;
		}
		
		//if no attributes have been passed, set root node to majority outcome
		if(attributes.size() == 0)
		{
			if(totyes >= totno)
			{
				Node root = new Node("Yes");
				return root;
			}
			else
			{
				Node root = new Node("No");
				return root;
			}
		}
		
		int attributeIndex = 0;
		double max = - 10000.0;
		int maxInd = 0;
		//calculate current entropy
		double prevInf = entropy(totyes,totno);
		double val = 0.0;
		int yes = 0, no = 0;
		
		//loop through all attributes
		for(int i = 0; i < attributes.size(); i++)
		{
			//find out the index of the attribute we're dealing with at the moment
			for(int k = 0; k < data.get(0).size(); k++)
			{
				if(data.get(0).get(k).toLowerCase().equals(attributes.get(i).toLowerCase()))
				{
					attributeIndex = k;
					break;
				}
			}
			//intialise value to current entropy at start of each iteration
			val = prevInf;
			
			//loop through all the possible values for the current attribute
			for(int j = 0; j < uniqueVals.get(attributeIndex).size(); j++)
			{
				//initialise yes/no example outcomes
				yes = no = 0;
				//loop through examples
				for(int k = 1; k < data.size(); k++)
				{
					//if the value of the attribute in question is equal to that of the current iteration, increment yes/no count
					if(data.get(k).get(attributeIndex).toLowerCase().equals(uniqueVals.get(attributeIndex).get(j).toLowerCase()))
					{
						if(data.get(k).getLast().toLowerCase().equals("yes"))
						{
							yes++;
						}
						else
						{
							no++;
						}
					}
				}
				//calculate information gain of current category
				val -= ((double)(yes+no)/(double)(data.size()-1))*entropy(yes,no);
			}
			
			System.out.println("Information Gain for "+attributes.get(i)+" is "+val);
			//store attribute with greatest information gain
			if(val > max)
			{
				max = val;
				maxInd = i;
			}
		}
		
		System.out.println(attributes.get(maxInd)+" selected as sub-tree root.");
		System.out.println(attributes.get(maxInd)+" node created.");
		//create new node for attribute with greatest information gain
		Node root = new Node(attributes.get(maxInd));
		
		//remove attribute from attributes list
		LinkedList<String> attributesCopy = new LinkedList<String>();
		attributesCopy = (LinkedList<String>) attributes.clone();
		attributesCopy.remove(maxInd);
		
		//find out the index of the attribute we're dealing with at the moment
		for(int k = 0; k < data.get(0).size(); k++)
		{
			if(data.get(0).get(k).toLowerCase().equals(attributes.get(maxInd).toLowerCase()))
			{
				attributeIndex = k;
				break;
			}
		}
		
		//loop through all possible values for the attribute selected
		for(int i = 0; i < uniqueVals.get(attributeIndex).size(); i++)
		{
			//create copy of data to pass on to recursive call
			LinkedList<LinkedList<String>> dataCopy = new LinkedList<LinkedList<String>>();
			dataCopy = (LinkedList<LinkedList<String>>) data.clone();
			
			//loop through rows
			for(int j = 1; j < dataCopy.size(); j++)
			{
				//if example value for selected attribute is not equal to that of the current iteration, remove it
				if(!dataCopy.get(j).get(attributeIndex).toLowerCase().equals(uniqueVals.get(attributeIndex).get(i).toLowerCase()))
				{
					//decrement j as there is now one less list in dataCopy
					dataCopy.remove(j--);
				}
			}
			
			System.out.println("Example set reduced to those with "+attributes.get(maxInd)+" set to "+ uniqueVals.get(attributeIndex).get(i));
			
			//if no examples are left, set value to majority of all examples
			if(dataCopy.size() <= 1)
			{
				if(totyes >= totno)
				{
					Node rootChild = new Node("Yes");
					root.addChild(rootChild, uniqueVals.get(attributeIndex).get(i));
					System.out.println("Child of "+attributes.get(maxInd)+" node's "+ uniqueVals.get(attributeIndex).get(i)+" branch set to Yes\n");
				}
				else
				{
					Node rootChild = new Node("no");
					root.addChild(rootChild, uniqueVals.get(attributeIndex).get(i));
					System.out.println("Child of "+attributes.get(maxInd)+" node's "+ uniqueVals.get(attributeIndex).get(i)+" branch set to No\n");
				}
			}
			//otherwise recursively call the function to find the current nodes decision sub-tree
			else
			{
				Node rootChild = ID3(dataCopy, uniqueVals, targetAttribute, attributesCopy);
				root.addChild(rootChild, uniqueVals.get(attributeIndex).get(i));
				System.out.println("Child of "+root.getName()+" node's "+ uniqueVals.get(attributeIndex).get(i)+" branch set to "+rootChild.getName()+"\n");
			}
		}
		//return root node of current recursive level.
		return root;
	}
	
	/* Arguments:
	 * - example: Linked List of example values
	 * - root: Current sub-decision tree root node
	 * - attributes: Linked List of table attributes
	 * Returns: Boolean value for target attribute
	 * Function recursively traverses the tree in a depth-first fashion, until the target attribute value is found
	 */
	public static boolean DFS(LinkedList<String> example, Node root, LinkedList<String> attributes)
	{
		if(root.getName().toLowerCase().equals("yes") && root.getChildren().size() == 0)
		{
			return true;
		}
		
		if(root.getName().toLowerCase().equals("no") && root.getChildren().size() == 0)
		{
			return false;
		}
		
		//node not leaf node
		
		int index = -1;
		//find out what attribute current node tests
		for(int i = 0; i < attributes.size(); i++)
		{
			if(root.getName().toLowerCase().equals(attributes.get(i).toLowerCase()))
			{
				index = i;
				break;
			}
		}

		int childIndex = -1;
		//find out which branch we should follow
		for(int i = 0; i < root.getChildren().size(); i++)
		{
			if(root.getBranches().get(i).toLowerCase().equals(example.get(index).toLowerCase()))
			{
				childIndex = i;
				break;
			}
		}
		
		if(childIndex == -1)
		{
			System.err.println("Error in test case");
			return false;
		}
		
		//recursively call function, passing the appropriate child node
		return DFS(example, root.getChildren().get(childIndex), attributes);
	}

	/* Arguments:
	 * - testData: Table-like data structure holding test cases
	 * - root: Root node of decision tree
	 * - attributes: Linked List of decision example attributes
	 * - targetAttribute: String value of attribute to learn
	 * Returns: void
	 * Function performs DFS of decision tree for each test example, outputting the results on-screen
	 */
	public static void outputResults(LinkedList<LinkedList<String>> testData, Node root, LinkedList<String> attributes, String targetAttribute)
	{
		//loop through each test example
		for(int i = 1; i < testData.size(); i++)
		{
			if(i == 1)
			{
				System.out.println("\t\t\t\t Example \t\t\t\t\t\t Result\n");
			}
			
			System.out.print(i+") ");
			
			//loop through test example values
			for(int j = 0; j < attributes.size(); j++)
			{
				System.out.print(testData.get(i).get(j)+"\t\t");
			}
			
			//output target attribute outcome
			System.out.println("\t\t"+DFS(testData.get(i),root,attributes));
		}
	}
}