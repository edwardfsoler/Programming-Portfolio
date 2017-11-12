import java.util.*;

public class NQueensTester
{
	static Scanner sc;
	
	public static void main(String[] args)
	{
		//initialise queens
		int[] queens = makeQueens();
		
		//perform random global search to act as starting point
		globalSearch(queens);
		
		//call Creeping Random Search function
		int count = CRS(queens);
		
		//if CRS failed, enter
		if(count < 0)
		{
			System.out.println("CRS Failed");
		}
		//CRS succeeded
		else
		{
			System.out.println("Solution found in "+count+" cycles");
			//print out solutions
			printBoard(queens);
		}
		
		//initialise queens
		int[] queens1 = makeQueens();
		
		//perform random global search to act as starting point
		globalSearch(queens1);
		
		//call Simulated Annealling function
		count = SA(queens1);
		
		//if SA failed, enter
		if(count < 0)
		{
			System.out.println("SA Failed");
		}
		//SA succeeded
		else
		{
			System.out.println("Solution found in "+count+" cycles");
			//print out solution
			printBoard(queens1);
		}
		
		//call Genetic Algorithm function
		int[] queens2 = GA();
		
		//test whether GA search failed
		if(queens2 == null)
		{
			System.out.println("GA Failed");
		}
		else
		{
			//print out solution
			printBoard(queens2);
		}
	}
	
	//arguments: none
	//return: solution to NQueens problem or null
	//function: performs Genetic Algorithm search and optimisation of NQueens board
	public static int[] GA()
	{
		sc = new Scanner(System.in);
		
		int n = 0;
		int psize = 0;
		int generations = 0;
		double coprob = 0.0;
		double mprob = 0.0;
		boolean repeat = true;
		
		//ask user to input size of the NQueens problem to solve
		System.out.println("Please enter the number of Queens you would like to include in this problem: ");
			
		do
		{	
			n = sc.nextInt();
			
			//test range of input integer
			if(n < 4 || n > 25)
			{
				System.out.println("Please enter a value between 4 and 25: ");
				repeat = true;
			}
			else
			{
				repeat = false;
			}
		
		//handle out of bounds value input
		}while(repeat);
		
		//ask user to input size of population in each generation
		System.out.print("Please enter the population size to maintain: ");
		
		do
		{
			psize = sc.nextInt();
			
			if(psize < 0)
			{
				System.out.println("Please enter a value greater than 0");
				repeat = true;
			}
			else
			{
				repeat = false;
			}
		}while(repeat);
		
		
		//ask user to input the cross over percentage
		System.out.print("Please enter the cross over percentage (1.0 - 99.0): ");
		
		do
		{
			coprob = sc.nextDouble();
			
			if(coprob < 1.0 || coprob > 99.0)
			{
				repeat = true;
				System.out.println("Please enter a value between 1.0 and 99.0");
			}
			else
			{
				repeat = false;
			}
		}while(repeat);
		
		//ask user to input the probability of mutation for each queen layout instance
		System.out.print("Please enter the mutation probability percentage (0.1 - 99.0): ");
		
		do
		{
			mprob = sc.nextDouble();
			
			if(mprob < 0.1 || mprob > 99.0)
			{
				repeat = true;
				System.out.println("Please enter a value between 0.1 and 99.0");
			}
			else
			{
				repeat = false;
			}	
		}while(repeat);
		
		//create an initial population to start the algorithm off with
		LinkedList<int[]> ll = GAInitialise(n, psize);
		
		//calculate the number of instances that must carry on from one generation to the next
		int keep = (int)(psize * ((100-coprob)/100));
		
		//prevent generation from being completely eradicated
		if(keep < 1)
		{
			keep = 1;
		}
		
		do
		{
			//create pool of elite elements
			LinkedList<int[]> pool = getElite(ll, keep);
			
			//increment number of generations
			generations++;
			
			//perform crossover on randomly selected elite members
			poolCrossOver(pool, psize);
			
			//perform mutation with probability mprob on any element in generation
			poolMutate(pool, mprob);
			
			//test whether a solution exists in the current pool
			for(int i = 0; i < pool.size(); i++)
			{
				//get errors for current queen layout instance
				int[] errors = queensTester(pool.get(i));
				
				//test whether current instance is a solution
				if(fitness(errors) == 0)
				{
					System.out.println("Completed in "+generations+" generations.");
					return pool.get(i);
				}
			}
		//set limit on loop
		}while(generations < 10000000);
		
		//solution not found
		System.out.println("Search for solution failed. Altering the input parameters may yield better results");
		return null;
	}
	
	//arguments: pool of queen layouts to be individually mutated with a probability of mprob
	//return: void
	//function: selects queen layouts with a probability of mprob to be mutated
	public static void poolMutate(LinkedList<int[]> pool, double mprob)
	{
		//divide probability by 100 to get decimal value
		mprob /= 100;
		
		//loop through pool of queen layouts
		for(int i = 0; i < pool.size(); i++)
		{
			//test whether to mutate
			if(Math.random() <= mprob)
			{
				//call mutate function on current queen layout
				mutate(pool.get(i));
			}
		}
	}
	
	//arguments: linked list of elite queen layouts and size of population
	//returns: void
	//function: randomly selects 2 elements in pool and performs crossover
	public static void poolCrossOver(LinkedList<int[]> pool, int psize)
	{
		//if pool only contains 1 element then return
		if(pool.size() == 1)
		{
			return;
		}
		
		Random random = new Random();
		
		int rand1, rand2;
		
		//store size of linked list on function entry so as to only perform cross over
		//on initial population and not on newly formed offspring
		int initialSize = pool.size();
		
		//generate offspring until population size is reached
		while(pool.size() < psize)
		{
			//randomly select one of the initial elements in the list
			rand1 = random.nextInt(initialSize);
			
			do
			{
				rand2 = random.nextInt(initialSize);
			//ensures that the two randomly selected queen layouts are not the same
			}while(rand1 == rand2);
			
			//perform crossover between the two selected queen layouts and add the offspring to the pool
			pool.add(crossover(pool.get(rand1), pool.get(rand2)));
		}
	}
	
	//arguments: Linked list of queens, number of elites to extract
	//returns: pool of elite queen layouts from given population
	//function: extracts elite queen layouts from the current pool of solutions
	public static LinkedList<int[]> getElite(LinkedList<int[]> ll, int keep)
	{
		//create linked list to store elite
		LinkedList<int[]> pool = new LinkedList<int[]>();
		//create linked list to store indices of queen elements
		LinkedList<Integer> tpool = new LinkedList<Integer>();
		
		//set minimum to highest possible error
		int min = ll.get(0).length + 1;
		int cur = 0;
		
		do
		{
			//loop through elements
			for(int i = 0; i < ll.size(); i++)
			{
				//extract error rating of current element
				int[] errors = queensTester(ll.get(i));
				//extract fitness rating of current element
				cur = fitness(errors);
				
				//if current element is new min, update min, clear tpool and add new element
				if(cur < min)
				{
					min = cur;
					tpool.clear();
					tpool.add(i);
				}
				//if current element  is equal to current min then add to tpool
				else if(cur == min)
				{
					tpool.add(i);
				}
			}
			
			//loop through tpool
			for(int i = 0; i < tpool.size(); i++)
			{
				//remove all elements that are in tpool from ll
				ll.remove(tpool.get(i));
				//add all elements in tpool to pool
				pool.add(ll.get(tpool.get(i)));
				
				//if number of elements to keep is reached, stop adding elements to pool
				if(pool.size() == keep)
				{
					break;
				}
			}
			
			//empty tpool for next round of elite skimming
			tpool.clear();
		
		//repeat until number of elements to keep is exceeded
		}while(pool.size() < keep);	
		
		return pool;
	}
	
	//arguments: 2 queen arrays to be crossed over
	//returns: offspring queen array of the 2 given queen arrays
	//function: performs crossover of two elements in the current generation
	public static int[] crossover(int[] q1, int[] q2)
	{
		Random random = new Random();
		
		//create copy of first queen array
		int[] offspring = new int[q1.length];
		offspring = q1.clone();
		
		//randomly select partition for crossover, partition 
		//must start after the first element and before the last element
		int rand = random.nextInt(q1.length-2) + 1;
		
		//loop through the first partition, performing crossover on each individual queen value
		for(int i = 0; i <= rand; i++)
		{
			//call switcharoo function to swap current queen instance in queen array
			switcharoo(offspring, q2[i], i);
		}
		
		//return newly created offspring
		return offspring;
	}
	
	//arguments: queens array to be manipulated, value to place, position to place it
	//returns: void
	//action: manipulates queens array to bring value v to position pos by performing a swap
	//		  so as not to break the queens rule
	public static void switcharoo(int[] queens, int v, int pos)
	{
		int temp;
		
		//if value is already in required position, exit
		if(queens[pos] == v)
		{
			return;
		}
		else
		{
			//store copy of current value
			temp = queens[pos];
			//update position with required value
			queens[pos] = v;
		}
		
		//loop through remainder of array searching for value v
		for(int i = pos+1; i < queens.length; i++)
		{
			//if value v is found, enter
			if(queens[i] == v)
			{
				//update position with value of old place temp
				queens[i] = temp;
				//exit
				return;
			}
		}
	}
	
	//arguments: single instance of queens layout
	//returns: void
	//function: randomly selects and swaps two columns of queens
	public static void mutate(int[] queens)
	{
		Random random = new Random();
		
		//select one random queen to swap
		int r1 = random.nextInt(queens.length);
		int r2;
		int temp;
		
		do
		{
			//select next random queen to swap
			r2 = random.nextInt(queens.length);
		//if the two queens chosen to swap are the same, repeat and change second queen	
		}while(r1 == r2);
		
		//perform swap
		temp = queens[r1];
		queens[r1] = queens[r2];
		queens[r2] = temp;		
	}
	
	//arguments: size of board and size of population
	//returns: population of size psize with n queens randomly laid out
	//function: randomly creates initial population to run the algorithm on
	public static LinkedList<int[]> GAInitialise(int n, int psize)
	{
		//create linked list to store population
		LinkedList<int[]> ll = new LinkedList<int[]>();
		
		//loop for psize times
		for(int i = 0; i < psize; i++)
		{
			//create empty integer array of size n
			int[] queens = new int[n];
			
			//loop through each index filling it with value unique to any other index
			for(int j = 0; j < queens.length; j++)
			{
				queens[j] = j;
			}
			
			//generate random order for queen layout
			globalSearch(queens);
			
			//add layout to population
			ll.add(queens);
		}
		
		return ll;
	}
	
	//arguments: initial queens layout to perform Simulated Annealing upon
	//return: count of number of cycles performed
	//function: performs simulated annealing optimised search for solution to NQueens problem
	public static int SA(int[] queens)
	{
		boolean repeat = true;
		sc = new Scanner(System.in);
		
		double temp = 0.0;
		int badc = 0;
		int goodc = 0;
		int moves = 0;
		double decRate;
		int temporary = 0;
		
		//ask user to input starting temperature and store value
		System.out.print("Please enter the starting temperature: ");
		
		do
		{
			temp = sc.nextDouble();
			
			if(temp < 1.0)
			{
				System.out.print("Please enter a value greater than 1: ");
				repeat = true;
			}
			else
			{
				repeat = false;
			}
		}while(repeat);
		
		//ask user to input decrease rate of temperature and store value
		System.out.print("Please enter the decrease rate of the temperature: ");
		
		do
		{
			decRate = sc.nextDouble();
			
			if(decRate > 1.0 | decRate < 0.0)
			{
				System.out.print("Please enter a decimal value between 0 and 1: ");
				repeat = true;
			}
			else
			{
				repeat = false;
			}
		}while(repeat);
		
		
		//loop until successful queen layout is found or until temperature reaches 1
		while(temp > 1)
		{
			//compute error of each queen
			int[] errors = queensTester(queens);
			
			//if solution is found, exit
			if(fitness(errors) == 0)
			{
				return moves;
			}
			
			//make copy of queens array
			int[] tqueens = queens;
			
			//retrieve bad and good candidates for swap
			badc = badCandidate(errors);
			goodc = goodCandidate(errors);
			
			//perform swap of candidate queens
			temporary = tqueens[badc];
			tqueens[badc] = tqueens[goodc];
			tqueens[goodc] = temporary;
			
			//if new layout is better, then keep it, otherwise discard
			if(fitness(tqueens) < fitness(queens))
			{
				queens = tqueens;
			}
			//if new layout is not better, test whether it should be kept
			else
			{
				//test whether suboptimal solution should still be kept
				if(keepTester(temp, fitness(queens), fitness(tqueens)))
				{
					queens = tqueens;
				}
			}
			
			//decrement temperature by given amount
			temp *= decRate;
			
			//increment number of moves
			moves++;
		}
		
		//if search fails, return
		System.out.println("Search failed. Consider increasing the starting temperature.");
		return -1;
	}
	
	//arguments: current temperature, previous solution fitness, current sub-optimal solution fitness
	//returns: true if sub-optimal solution should be kept, false otherwise
	//function: takes temperature into account to decide whether to accept sub-optimal solution
	public static boolean keepTester(double temp, int f1, int f2)
	{		
		//calculate probability of accepting sub-optimal solution
		//based upon difference in fitness between solutions and current temperature
		double prob = Math.exp(-(f1 - f2)/(temp));
		
		//perform random probability test
		if(Math.random() <= prob)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//arguments: initial queens layout to run Creeping Random Search upon
	//returns: counter of cycles performed
	//function: performs Creeping Random search upon an initial random queen layout
	public static int CRS(int[] queens)
	{		
		int moves = 0;
		boolean repeat = true;
		int local = 0;
		int badc = 0;
		int goodc = 0;
		int temp = 0;
		boolean locals = true;
		
		//compute error rating for current queen layout
		int[] errors = queensTester(queens);

		sc = new Scanner(System.in);
		
		//ask user to enter the ratio of local:global searches
		System.out.println("Please enter the percentage of local searches (whole number): ");
		
		do
		{
			local = sc.nextInt();
			
			if(local < 1 || local > 99)
			{
				System.out.println("Please enter a value between 1 and 99: ");
				repeat = true;
			}
			else
			{
				repeat = false;
			}
			
		}while(repeat);
		
		//loops until solution found
		while(true)
		{
			//return errors for new test solution
			errors = queensTester(queens);
			
			//test whether solution to problem has been found
			if(fitness(errors) == 0)
			{
				return moves; 
			}
			else
			{
				//increment number of moves (counter)
				moves++;
				
				//test whether ratio of local:global was surpassed
				if(moves % 100 < local)
				{
					locals = true;
				}
				else
				{
					//implement global search if ratio was surpassed
					locals = false;
				}
				
				//perform local search
				if(locals)
				{
					//make copy of queens array
					int[] tqueens = queens;
					
					//retrieve bad and good candidates for swap (search neighbourhood)
					badc = badCandidate(errors);
					goodc = goodCandidate(errors);
					
					//perform swap of candidate queens
					temp = tqueens[badc];
					tqueens[badc] = tqueens[goodc];
					tqueens[goodc] = temp;
					
					//if new layout is better, then keep it, otherwise discard
					if(fitness(tqueens) < fitness(queens))
					{
						queens = tqueens;
					}
				}
				//global search
				else
				{
					//make copy of queens array
					int[] tqueens = queens;
					
					//perform global search
					globalSearch(tqueens);
					
					//if new layout is better, then keep it, otherwise discard
					if(fitness(tqueens) < fitness(queens))
					{
						queens = tqueens;
					}
				}
			}
		}
	}
	
	//arguments: array of errors for each queen
	//return: queen with worst error rating (random queen of pool if multiple queens have the same worst rating)
	//function: selects worst queen in current queen layout, to be moved
	public static int badCandidate(int[] errors)
	{
		Random random = new Random();
		LinkedList<Integer> ll = new LinkedList<Integer>();
		int max = 0;
		
		//loop through array of errors
		for(int i=0; i < errors.length; i++)
		{
			//if current index contains value higher than maximum,
			//then update maximum and store copy in ll.
			if(errors[i] > max)
			{
				max = errors[i];
				ll.clear();
				ll.add(i);
			}
			//if current index shares maximum error value then add to pool of candidates
			//can be adjusted to widen/narrow hypersphere
			else if(errors[i] == max)
			{
				ll.add(i);
			}
		}
		
		//return random candidate from pool
		return ll.get(random.nextInt(ll.size()));
	}
	
	//arguments: array of errors for each queen
	//return: queen with best error rating (random queen of pool if multiple queens have the same best rating)
	//function: selects best queen in current queen layout to be moved
	public static int goodCandidate(int[] errors)
	{
		Random random = new Random();
		LinkedList<Integer> ll = new LinkedList<Integer>();
		
		//highest possible number of errors + 1
		int min = errors.length + 1;
		
		//loop through array of errors
		for(int i=0; i < errors.length; i++)
		{
			//if current index contains value lower than minimum,
			//then update minimum and store copy in ll.
			if(errors[i] < min)
			{
				min = errors[i];
				ll.clear();
				ll.add(i);
			}
			//if current index shares minimum error value then add to pool of candidates
			else if(errors[i] == min)
			{
				ll.add(i);
			}
		}
		
		//return random candidate from pool of candidates
		return ll.get(random.nextInt(ll.size()));
	}
	
	//argument: array containing errors of each queen
	//return: number of queens which conflict
	//function: counts the number of queens in a given queen layout which violate a rule
	public static int fitness(int[] errors)
	{
		int count = 0;
		
		//loop through array incrementing the count each time a queen with an error is encountered
		for(int i = 0; i < errors.length; i++)
		{
			//if current queen violates atleast 1 rule, increment counter
			if(errors[i] != 0)
			{
				count++;
			}
		}
		
		return count;
	}
	
	//arguments: array of queens
	//return: array of queens, randomly sorted
	//function: generates random row value for each queen in each column
	public static void globalSearch(int[] queens)
	{	
		Random random = new Random();
		
		int j = 0;
		int temp = 0;
		
		//loop through list of queens
		for(int i = 0; i< queens.length; i++)
		{
			//select random index of queen to swap row with
			j = random.nextInt(queens.length);
			
			//perform swap
			temp = queens[i];
			queens[i] = queens[j];
			queens[j] = temp;
		}
	}
	
	//arguments: array of queens
	//returns: integer array with errors for each queen
	//function: computes number of time a rule is violated by each queen
	public static int[] queensTester(int[] queens)
	{
		//find diagonal value of positive gradient
		int[] added = adder(queens);
		
		//find diagonal value of negative gradient
		int[] subtracted = subtractor(queens);
				
		//count number of errors for each queen and return results
		return countOcc(added, subtracted, queens);
	}
	
	//arguments: row array values and diagonal array values (both positive and negative gradient) together with queen layout
	//returns: array with count of errors for each individual queen
	//function: calculates number of times a specific queen violates a rule
	public static int[] countOcc(int[] add, int[] sub, int[] queens)
	{
		//create int array to store error scores for each queen
		int[] ans = new int[queens.length];
		
		int t1, t2, t3;
		
		//loop through all but last queen in list (as last queen would have already been tested with all other queens
		for(int i = 0; i < queens.length-1; i++)
		{
			//store temporary value of each array at index i for later use
			t1 = add[i];
			t2 = sub[i];
			t3 = queens[i];
			
			//loop through the remainder of each array to perform comparisons
			for(int j = i+1; j < queens.length; j++)
			{
				//if value is repeated, increment error count for both queens
				//this same test is performed on each of the three arrays
				if(t1 == add[j])
				{
					ans[i]++;
					ans[j]++;
				}
				
				if(t2 == sub[j])
				{
					ans[i]++;
					ans[j]++;
				}
				
				if(t3 == queens[j])
				{
					ans[i]++;
					ans[j]++;
				}
			}
		}
		
		//return error bearing array
		return ans;
	}
	
	//arguments: array of queens
	//returns: addition of queen row with queen column
	//function: adds queen row and column values to test for diagonal violation
	public static int[] adder(int[] queens)
	{
		//create array to store addition values
		int[] temp = new int[queens.length];
		
		//loop through array performing addition operation
		for(int i=0; i < queens.length; i++)
		{
			//add row with column and store in temp array
			temp[i] = i + queens[i];
		}
		
		return temp;
	}
	
	//arguments: array of queens
	//returns: subtraction of queen column from queen row
	//function: subtracts queen column value from row value to test for diagonal violation
	public static int[] subtractor(int[] queens)
	{
		//create array to store subtraction values
		int [] temp = new int[queens.length];
		
		//loop through array performing subtraction operation
		for(int i=0; i < queens.length; i++)
		{
			//subtract queen column from queen row and store in temp array
			temp[i] = queens[i] - i;
		}
		
		return temp;
	}
	
	//argument: void
	//return: blank integer array of size n
	//function: asks for user input on deciding the size of the NQueens problem and creates blank queens layout for this size
	public static int[] makeQueens()
	{
		boolean repeat = true;
		int n = 0;
		
		sc = new Scanner(System.in);
		
		//ask user to input size of the NQueens problem to solve
		System.out.println("Please enter the number of Queens you would like to include in this problem: ");
		
		do
		{
			n = sc.nextInt();
			
			//test range of input integer
			if(n < 4 || n > 25)
			{
				System.out.println("Please enter a value between 4 and 25: ");
				repeat = true;
			}
			else
			{
				repeat = false;
			}
		
		//handle out of bounds value input
		}while(repeat);
		
		//create empty integer array of size n
		int[] queens = new int[n];
		
		//populate array with unique values for each index
		for(int i=0; i < queens.length; i++)
		{
			queens[i] = i;
		}
		
		return queens;
	}
	
	//arguments: queens layout solution
	//return: void
	//function: prints board layout for given queens solution
	public static void printBoard(int[] queens)
	{
		//loop through queens array vertically
		for(int i=0; i < queens.length; i++)
		{
			//loop through queens array horizontally
			for(int j=0; j < queens.length; j++)
			{
				//if current height is equal to the index value, then print a queen
				if(queens[i] == j)
				{
					System.out.print(" Q");
				}
				//otherwise print an empty space
				else
				{
					System.out.print(" -");
				}
			}
			//move to new line for next row in board
			System.out.print("\n");
		}
	}
}