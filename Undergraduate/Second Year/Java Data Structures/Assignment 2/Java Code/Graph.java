/**
 * Interface specifying a graph.
 * Assumes that we do not have negative cost edges in the graph.
 * DO NOT CHANGE THIS CODE
 */
 
 
import java.util.Collection;


public interface Graph {
  
     /** 
      * Return the collection of vertices of this graph
      * @return the vertices as a collection (which is anything iterable)
      */
     public Collection<Vertex> loadVertices();
   
     /** 
      * Return the collection of edges of this graph
      * @return the edges as a collection (which is anything iterable)
      */
     public Collection<Edge> loadEdges();
    
     /**
      * Return a collection of vertices adjacent to a given vertex v.
      *   i.e., the set of all vertices w where edges v -> w exist in the graph.
      * @param v one of the vertices in the graph
      * @return a collection of vertices adjacent to v in the graph
      */
     public Collection<Vertex> findAdjacentVertices(Vertex v);
     
     /**
      * Test whether vertex end_point is adjacent to vertex start_point (i.e. start_point -> end_point) in a directed graph.
      * @param start_point one vertex
      * @param end_point another vertex
      * @return an array which will contain distance, time_needed, and ticket_price of edge if there is a directed edge from start_point 
      * to end_point in the graph 
      * Return -1 otherwise.
      * (Including returning -1 if one of the two vertices does not exist.)
      * Assumes that we do not have negative cost edges in the graph.
      */
     public int[] checkIsAdjacent(Vertex start_point, Vertex end_point);
}
