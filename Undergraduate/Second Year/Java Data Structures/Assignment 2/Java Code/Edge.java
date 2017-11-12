/**
 * Representation of a directed graph edge.
 */
 

public class Edge {
	public final Vertex from,to;
	public final int distance;
   public final int time_needed;
   public final int ticket_price;

	/**
	 * Construct a new edge
	 * @param from start vertex
	 * @param to end vertex
	 * @param distance distance between the vertices
    * @param time_needed required time to travel between the vertices
    * @param ticket_price required travel cost
	 */
    
	public Edge(Vertex from, Vertex to, int distance, int time_needed, int ticket_price) {
		if(from == null || to == null)
			throw new IllegalArgumentException("null");
		this.from = from;
		this.to = to;
		
		//if distance value is negative, output warning and convert it to positive
		if(distance < 0)
		{
			System.out.println("Negative distances are not allowed");
			distance *= -1;
		}
		
		this.distance = distance;
		
		//if time_needed value is negative, output warning and convert it to positive
		if(time_needed < 0)
		{
			System.out.println("Negative times are not allowed");
			time_needed *= -1;
		}
		
		this.time_needed = time_needed;
		
		//if cost value is negative, output warning and convert it to positive
		if(ticket_price < 0)
		{
			System.out.println("Negative prices are not allowed");
			ticket_price *= -1;
		}
		
		this.ticket_price = ticket_price;
	}
	
	//added function to return class data for specific instance of class
	public Vertex getEdgeFrom()
	{
		return from;
	}
	
	public Vertex getEdgeTo()
	{
		return to;
	}
	
	public int getEdgeDistance()
	{
		return distance;
	}
	
	public int getEdgeTime()
	{
		return time_needed;
	}
	
	public int getEdgeCost()
	{
		return ticket_price;
	}

	/**
	 * A string representation of this object
	 * @return A string of the form <from, to, distance, time_needed, ticket_price>
	 */
	public String toString() {
		return "<"+from+", "+to+", "+distance+" , "+time_needed+" +"+ticket_price+" >";
	}

	/**
   * Compares all fields of the edge
   */
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		final Edge other = (Edge) obj;
		if (from == null) {
			if (other.from != null)
				return false;
		} else if (!from.equals(other.from))
			return false;
		if (to == null) {
			if (other.to != null)
				return false;
		} else if (!to.equals(other.to))
			return false;
		if (distance != other.distance)
			return false;
      else if (time_needed != other.time_needed)
			return false;
      else if (ticket_price != other.ticket_price)
			return false;
         
      
		return true;
	}
}
