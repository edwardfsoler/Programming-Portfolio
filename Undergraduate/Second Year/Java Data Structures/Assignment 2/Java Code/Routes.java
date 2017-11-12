/*
 * Routes.java
 *
 * The driver program for CSCI 241's Programming Assignment 2
 *
 * Reads two text files and takes source, destination, and travel choice from the user
 *
 * Moushumi Sharmin
 * Nov 2015.
 *
 *
 * ----------------------------------------------------------------------------
 *
 * usage:
 *
 * java Routes vertexFilename edgeFilename
 *
 * where the arguments are
 *
 *   vertexFileName        a plaintext file from which your program will read vertices
 *   edgeFileName          a plaintext file from which your program will read edges and weights
 *
 *
 * After reading in the graph, it will prompt the user for source and destination and preferred optimization parameter
 * 1 = shortest route, 2 = cheapest route, 3 = fastest route, 4 = display all option
 *
 */



import java.util.*;
import java.io.*;
import java.util.Scanner;

public class Routes {

	public static void main(String[] args) {
		if(args.length != 2) {
			System.err.println("USAGE: java Routes <vertex_file> <edge_file>");
			System.exit(1);
		}

		MyGraph g = readGraph(args[0],args[1]);

		Collection<Vertex> v = g.loadVertices();
		System.out.println("Vertices in the graph are: "+v);
		System.out.println("Edges in the graph are: "+g.loadEdges());

		//take copy of vertices and edges
		ArrayList<Vertex> vertices = new ArrayList<Vertex>(g.loadVertices());
		ArrayList<Edge> edges = new ArrayList<Edge>(g.loadEdges());

      	//3 dimensional array to store values for adjacency matrices, based on cost, distance and time
	    int [][][] matrix = new int [vertices.size()][vertices.size()][3];
	    int [] val = new int [3];

		//loop through set of vertices vertically
		for(int i =0; i<vertices.size(); i++)
		{
			//loop through set of vertices horizontally
			for(int j =0; j<vertices.size(); j++)
			{
				//return weight of edge between two given vertices
				val = g.checkIsAdjacent(vertices.get(j), vertices.get(i));

				//if vertices are not adjacent, populate space with 0 (val = {-1} if vertices are not adjacent)
				if(val.length < 3)
				{
					matrix[j][i][0] = 0;
					matrix[j][i][1] = 0;
					matrix[j][i][2] = 0;
				}
				else
				{
					//store value for distance
					matrix[j][i][0] = val[0];
					//store value for cost
					matrix[j][i][1] = val[1];
					//store value for travel time
					matrix[j][i][2] = val[2];
				}
			}
		}

		//loop 3 times. 1 for each weight variable
		for(int choice = 0; choice<3; choice++)
		{
			//output adjacency matrix title
			switch(choice)
			{
				case 1:
					System.out.println("Adjacency Matrix with distance as edge's weight");
					System.out.println();
					break;
				case 2:
					System.out.println("Adjacency Matrix with travel cost as edge's weight");
					System.out.println();
					break;
				case 3:
					System.out.println("Adjacency Matrix with travel time as edge's weight");
					System.out.println();
					break;

			}

			//loop through vertices vertically, starting from -1 to output titles
			for(int i=-1; i<vertices.size(); i++)
			{
				//loop through vertices horizontally, starting from -1 to output titles
				for(int j=-1; j<vertices.size(); j++)
				{
					//leave space on upper-left corner of adjacency matrix
					if(i == -1 && j == -1)
					{
						System.out.printf("\t");
					}

					//output titles horizontally
					if(i == -1 && j > -1)
					{
						System.out.printf(vertices.get(j)+"\t");
					}

					//output titles vertically
					if(i > -1 && j == -1)
					{
						System.out.printf(vertices.get(i)+"\t");
					}

					//output data in specified column
					if(i > -1 && j > -1)
					{
						System.out.printf(matrix[i][j][choice]+"\t");
					}
				}

				//new line
				System.out.println();
			}

			//line break between matrices
			System.out.println();
			System.out.println();
			System.out.println();
		}

		Scanner console = new Scanner(System.in);
		for(;;) {
			System.out.print("Please specify the Start vertex: ");
			Vertex start_point = new Vertex(console.nextLine());

			if(!v.contains(start_point)) {
				System.out.println("No such vertex in the graph.");
				System.exit(0);
			}


			System.out.print("Please specify the Destination vertex: ");
			Vertex end_point = new Vertex(console.nextLine());

			if(!v.contains(end_point)) {
				System.out.println("No such vertex in the graph.");
				System.exit(1);
			}


			System.out.print("Optimization parameter? (1 = shortest route, 2 = cheapest route, 3 = fastest route, 4 = display all options) ");
			Scanner in = new Scanner(System.in);
			int choice = in.nextInt();
			if(choice < 1 && choice > 4) {
				System.out.println("Invalid option");
				System.exit(2);
			}

			List<Vertex> route = new ArrayList<Vertex>();
			int length;
			if (choice<4){
				length = g.findRoute(start_point, end_point, route, choice);

				//added code
					if(length == -1)
					{
						System.out.println("No route available");
					}
					else
					{
						switch(choice)
						{
							case 1:
								System.out.println("Shortest route between "+start_point.toString()+ " and "+ end_point.toString() +" is:\n");
								break;

							case 2:
								System.out.println("Cheapest route between "+start_point.toString()+ " and "+ end_point.toString() +" is:\n");
								break;

							case 3:
								System.out.println("Fastest route between "+start_point.toString()+ " and "+ end_point.toString() +" is:\n");
								break;
						}

						for(Vertex hop : route)
						{
							System.out.print(hop+" ");
						}

						System.out.println();

		   				switch(choice)
						{
							case 1:
								System.out.println("Shortest path cost: "+length);
								break;

							case 2:
								System.out.println("Cheapest path cost: "+length);
								break;

							case 3:
								System.out.println("Fastest path cost: "+length);
								break;
						}
					}
			}
			else {
				for (int i=1; i<=3; i++){
					length = g.findRoute(start_point, end_point, route, i/*changed 'choice' to i*/);

					//added code
					if(length == -1)
					{
						System.out.println("No route available");
					}
					else
					{
						switch(i)
						{
							case 1:
								System.out.println("Shortest route between "+start_point.toString()+ " and "+ end_point.toString() +" is:\n");
								break;

							case 2:
								System.out.println("Cheapest route between "+start_point.toString()+ " and "+ end_point.toString() +" is:\n");
								break;

							case 3:
								System.out.println("Fastest route between "+start_point.toString()+ " and "+ end_point.toString() +" is:\n");
								break;
						}

						for(Vertex hop : route)
						{
							System.out.print(hop+" ");
						}

						System.out.println();

		   				switch(i)
						{
							case 1:
								System.out.println("Shortest path cost: "+length);
								break;

							case 2:
								System.out.println("Cheapest path cost: "+length);
								break;

							case 3:
								System.out.println("Fastest path cost: "+length);
								break;
						}
					}
				}
			}
		}
	}


   /* Reads vertex and edge information
   */

	public static MyGraph readGraph(String f1, String f2) {
		Scanner s = null;
		try {
			s = new Scanner(new File(f1));
		} catch(FileNotFoundException e1) {
			System.err.println("FILE NOT FOUND: "+f1);
			System.exit(2);
		}

		Collection<Vertex> v = new ArrayList<Vertex>();
		while(s.hasNext())
			v.add(new Vertex(s.next()));

		try {
			s = new Scanner(new File(f2));
		} catch(FileNotFoundException e1) {
			System.err.println("FILE NOT FOUND: "+f2);
			System.exit(2);
		}

		Collection<Edge> e = new ArrayList<Edge>();
		while(s.hasNext()) {
			try {
				Vertex a = new Vertex(s.next());
				Vertex b = new Vertex(s.next());
				int distance = s.nextInt();
            int time_needed = s.nextInt();
            int ticket_price = s.nextInt();
				e.add(new Edge(a,b,distance,time_needed,ticket_price));
			} catch (NoSuchElementException e2) {
				System.err.println("EDGE FILE FORMAT INCORRECT");
				System.exit(3);
			}
		}

		return new MyGraph(v,e);
	}
}
