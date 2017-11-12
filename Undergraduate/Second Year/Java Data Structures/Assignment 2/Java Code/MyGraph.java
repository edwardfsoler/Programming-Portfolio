/* 
 * MyGraph.java
 *
 * A representation of a graph.
 * Assumes that we do not have negative cost edges in the graph.
 *
 * Students may only use functionality provided in the packages
 *     java.util 
 *     java.io
 * 
 * Use of any additional Java Class Library components is not permitted 
 * 
 * Edward Fleri Soler
 */

import java.util.*;
import java.io.*;


public class MyGraph implements Graph {

   	private Collection<Vertex> myVertices;	//the vertices in this graph
   	private Collection<Edge> myEdges;	//the edges in this graph
   
   	/**
   	 * Creates a MyGraph object with the given collection of vertices
   	 * and the given collection of edges.
   	 * @param v a collection of the vertices in this graph
   	 * @param e a collection of the edges in this graph
   	 */
   	public MyGraph(Collection<Vertex> v, Collection<Edge> e) {
   		//class constructor initialises myVertices and myEdges on class instanciation
   	    this.myVertices = v;
   	    this.myEdges = e;   
   	}
   
   
   	/** 
   	 * Return the collection of vertices of this graph
   	 * @return the vertices as a collection (which is anything iterable)
   	 */
   	public Collection<Vertex> loadVertices() {
   		
   		//simply returns collection of myVertices
   		return myVertices;
   
   	}
   
   	/**
   	 * Return the collection of edges of this graph
   	 * @return the edges as a collection
   	 */
   	public Collection<Edge> loadEdges() {
   
   	    //simply returns collection of myEdges
   		return myEdges;
   
   	}
   
   	/** 
   	 * Return a collection of vertices adjacent to a given vertex v.
   	 *   i.e., the set of all vertices w where edges v -> w exist in the graph.
   	 * @param v one of the vertices in the graph
   	 * @return a collection of vertices adjacent to v in the graph
   	 */
   	public Collection<Vertex> findAdjacentVertices(Vertex a) {
   		
   		//collection of adjacent vertices to be returned
   		Collection<Vertex> ll = new ArrayList<Vertex>();
   		
   		//copy edges and vertices into arraylists
   		ArrayList<Edge> edges = new ArrayList<Edge>(this.loadEdges());
   		ArrayList<Vertex> vertices = new ArrayList<Vertex>(this.loadVertices());
   		
   		//get number of edges in graph
   		int length = edges.size();
   		
   		//loop through each and every edge in the graph
   		for(int i = 0; i < length; i++)
   		{
   			//if currently searched edge has same 'from' as required vertex, then enter
   			if(edges.get(i).getEdgeFrom().equals(a))
   			{
   				//if adjacent edge is not in 'll', then add
   				if(!(ll.contains(edges.get(i).getEdgeTo())))
   				{
   					//add adjacent node to 'll'
   					ll.add(edges.get(i).getEdgeTo());	
   				}
   			}
   		}
   		
   		//return list of nodes adjacent to given node
   		return ll;
   
   	}
   
   	 /**
      * Test whether vertex end_point is adjacent to vertex start_point (i.e. start_point -> end_point) in a directed graph.
      * @param start_point, one vertex
      * @param end_point, another vertex
      * @return an array which will contain distance, time_needed, and ticket_price of edge if there is a directed edge from start_point 
      * to end_point in the graph 
      * Return -1 otherwise.
      * (Including returning -1 if one of the two vertices does not exist.)
      * Assumes that we do not have negative cost edges in the graph.
      */
   	public int[] checkIsAdjacent(Vertex a, Vertex b) {
   
   		//copy list of vertices into array list
   		ArrayList<Vertex> vertices = new ArrayList<Vertex>(this.loadVertices());
   		
   		//if start or end vertices are not in the collection of vertices then return -1
   		if(!(vertices.contains(a)) || !(vertices.contains(b)))
   		{
   			//must return array of ints as returning int is a violation
   			int [] ret = {-1};
   			
   			return ret;
   		}
   		
   		//if given vertices are valid, compute list of vertices adjacent to a
   		ArrayList<Vertex> adjacentVertices = new ArrayList<Vertex>(this.findAdjacentVertices(a));
   		
   		//load list of edges
   		ArrayList<Edge> edges = new ArrayList<Edge>(this.loadEdges());
   		
   		//if the ending vertex is adjacent to the starting vertex then enter
   		if(adjacentVertices.contains(b))
   		{
   			//loop through all edges
   			for(int i = 0; i < edges.size(); i++)
   			{
   				//search for specific edge required
   				if((edges.get(i).getEdgeFrom().equals(a)) && (edges.get(i).getEdgeTo().equals(b)))
   				{
   					//create array of ints and populate with edge values
   					int [] values = {edges.get(i).getEdgeDistance(), edges.get(i).getEdgeTime(), edges.get(i).getEdgeCost()};
   					
   					//break from loop and return int array
   					return values;
   				}
   			}
   		}
   		
   		//else
   		int [] ret = {-1};
   			
   		//ending vertex is not adjacent to starting vertex
   		return ret;
   	}
   	/**
   	 * Returns the shortest route from start_point to end_point in the graph.  
   	 * Assumes positive edge weights.
   	 * @param start_point the starting vertex
   	 * @param end_point the destination vertex
   	 * @param route a list in which the route will be stored, in order, the first
   	 * being the start vertex and the last being the destination vertex.  the
   	 * list will be empty if no such route exists.  
   	 * @param choice (1 = shortest route, 2 = cheapest route, 3 = fastest route)
   	 * NOTE: the list will be cleared of any previous data.
   	 * @return the length of the shortest route from start_point to end_point, -1 if no such path
   	 * exists.
   	 */
   	public int findRoute(Vertex start_point, Vertex end_point, List<Vertex> route, int choice)
   	{
   		//function implements version of Dijkstra's Algorithm to find shortest path between two nodes
   		
   		//clear route list
   		route.clear();
   		
   		//create hashmap to store vertices and current cost
   		HashMap<Vertex, Integer> vertices = new HashMap<Vertex, Integer>();
   		
   		//create hashmap to store vertices and their parent node
   		HashMap<Vertex,Vertex> verticesPrev = new HashMap<Vertex, Vertex>();
   		
   		//copy list of edges and vertices into array lists
   		ArrayList<Edge> edges = new ArrayList<Edge>(this.loadEdges());
   		ArrayList<Vertex> v = new ArrayList<Vertex>(this.loadVertices());
   		
   		//create array list of type vertex to serve as a priority queue
   		ArrayList<Vertex> pq = new ArrayList<Vertex>();
   		
   		//set start index to have distance 0 and previous equal to null
   		vertices.put(start_point,0);
   		verticesPrev.put(start_point,null);
   		
   		//add start index to the priority queue
   		pq.add(start_point);
   		
   		//loop through list of vertices
   		for(int i = 0; i < v.size(); i++)
   		{
   			//required to prevent duplicate starting points being added
   			if(!(v.get(i).equals(start_point)))
   			{
	   			//initialise each vertex to have max distance and null previous
	   			vertices.put(v.get(i), Integer.MAX_VALUE);
	   			verticesPrev.put(v.get(i),null);
   			}
   		}
   		
   		//once we are done using v, use it as visited arraylist
   		v.clear();
   		
   		Vertex temp;
   		int tempd = 0;
   		int [] values = new int[3];
   		
   		//while priority queue is not empty
   		while(pq.size() > 0)
   		{
   			//copy unvisited vertex with least distance to temp
   			temp = getMin(pq,vertices);
   			
   			//remove chosen vertex from priority queue as it is now being visited
   			pq.remove(temp);
   			
   			//add chosen vertex to the visited list
   			v.add(temp);
   			
   			//if next node to test is the final state, then return its distance and exit
   			if(temp.equals(end_point))
   			{
   				//populate route list
   				getRoute(verticesPrev, temp, route);
   				
   				//add end_point to route list
   				route.add(temp);
   				
   				//return final distance of path from source node to destination
   				return vertices.get(temp);
   			}
   			
   			//get a list of all vertices adjacent to temp
   			ArrayList<Vertex> adj = new ArrayList<Vertex>(findAdjacentVertices(temp));
   			
   			//loop through adjacent nodes
   			for(int j = 0; j < adj.size(); j++)
   			{
   				//if adjacent vertex has already been visited, skip
   				if(v.contains(adj.get(j)))
   				{
   					continue;
   				}
   				
   				//return distances between vertices
   				values = checkIsAdjacent(temp, adj.get(j));
   				
   				//calculate temp distance to reach current vertex, based on choice
   				tempd = vertices.get(temp) + values[choice-1];
   				
   				//done solely due to error in vertices recognition of adj.get() vertex
   				ArrayList<Vertex> tester = new ArrayList<Vertex>(vertices.keySet());
   				
   				Vertex t = null;
   				
   				//required for bug fix
   				for(int k = 0; k<tester.size(); k++)
   				{
   					//Vertex HashMap 'vertices' was not recognising vertices passed from adj ArrayList, but was recognising vertices from its own keyset
   					//therefore these simple lines of code were added to overcome this problem
   					
   					//find vertex in tester list which is equal to current adjacent node
   					if(tester.get(k).equals(adj.get(j)))
   					{
   						//copy vertex to variable t
   						t = tester.get(k);
   					}
   				}
   				
   				//if new distance is less than previous distance, enter
   				if(tempd < vertices.get(t))
   				{
   					//update new distance for vertex
   					vertices.put(adj.get(j),tempd);
   					
   					//update previous vertex for current vertex
   					verticesPrev.put(adj.get(j), temp);
   					
   					//add current vertex to priority queue
   					pq.add(adj.get(j));
   				}
   			}
   		}
   		//else
   		
   		return -1;
   	}
   	
   	//function to recursively move backwards through graph to form route list
   	public void getRoute(HashMap<Vertex,Vertex> verticesPrev, Vertex node, List<Vertex> route)
   	{
   		//end of recursion
   		if(verticesPrev.get(node) == null)
   		{
   			return;
   		}
   		//else
   		
   		//copy parent node of current node
   		Vertex temp = verticesPrev.get(node);
   		
   		//recursively call function passing parent node
   		getRoute(verticesPrev,temp,route);
   		
   		//add current node to route list
   		route.add(temp);
   		return;
   	}
   	
   	//returns element in prioirtiy queue with smallest distance
   	public Vertex getMin(ArrayList<Vertex> pq, HashMap<Vertex, Integer> hm)
   	{
   		//set first element in the queue to the minimum
   		Vertex min = pq.get(0);
   		
   		//search through elements which are in the priority queue by accessing their distances through the hashmap hm
   		for(int i = 0; i < pq.size(); i++)
   		{
   			//if current vertex has distance less than previous min, then change min
   			if(hm.get(pq.get(i)) < hm.get(min))
   			{
   				//copy vertex with least weight to new min
   				min = pq.get(i);
   			}
   		}
   		
   		//return vertex with least weight in the queue
   		return min;
   	}
}
