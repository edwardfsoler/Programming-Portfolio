import java.io.File;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.*;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import com.google.common.collect.Lists;
import org.graphstream.graph.*;
import org.graphstream.graph.implementations.*;

public class KnowledgeExtraction2 {

	public static void main(String[] args) throws JSONException, IOException, ParserConfigurationException, SAXException
	{
		//set of main entities to include in graph search
		String [] entity_names = {"Joseph%20Muscat", "Simon%20Busuttil"};//, "Edward%20Scicluna", "Louis%20Grech", "Konrad%20Mizzi", "Tonio%20Fenech", "Lawrence%20Gonzi", "Austin%20Gatt"};
		String [] entity_names2 = {"Joseph Muscat", "Simon Busuttil"};//, "Edward Scicluna", "Louis Grech", "Konrad Mizzi", "Tonio Fenech", "Lawrence Gonzi", "Austin Gatt"};
		
		//set step distance for each node in graph
		int step = 5;
		//de/activate moralization on graph
		boolean moral = false;
		
		//de/activate community detection
		boolean community = true;
		
		LinkedList<LinkedList<String>> token_list = new LinkedList<LinkedList<String>>();
		LinkedList<LinkedList<String>> NER_tags = new LinkedList<LinkedList<String>>();
		LinkedList<LinkedList<Integer>> sentence_count = new LinkedList<LinkedList<Integer>>();
		LinkedList<LinkedList<LinkedList<Integer>>> coreference_indices = new LinkedList<LinkedList<LinkedList<Integer>>>();
		LinkedList<LinkedList<String>> coreference_title = new LinkedList<LinkedList<String>>();
		LinkedList<LinkedList<LinkedList<LinkedList<Integer>>>> relations = new LinkedList<LinkedList<LinkedList<LinkedList<Integer>>>>();
		LinkedList<LinkedList<LinkedList<String>>> infoBox_relations = new LinkedList<LinkedList<LinkedList<String>>>();
		LinkedList<LinkedList<String>> page_links = new LinkedList<LinkedList<String>>();
		LinkedList<LinkedList<String>> page_links_here = new LinkedList<LinkedList<String>>();
		LinkedList<String> file_names = new LinkedList<String>();
		
		//loop through list of main entities
		for(int i = 0; i < entity_names.length; i++)
		{
			//send raw text, ingoing and outgoing link requests to wikipedia
			file_names.add(getRawText(entity_names[i], entity_names2[i]));
			page_links.add(getLinks(entity_names[i]));
			page_links_here.add(getLinksHere(entity_names[i]));
			
			//send request and extract wikipedia info box relations
			infoBox_relations.add(getInfoBoxRels(entity_names[i], entity_names2[i], i, page_links.get(i), true));
			
			token_list.add(new LinkedList<String>());
			NER_tags.add(new LinkedList<String>());
			sentence_count.add(new LinkedList<Integer>());
			coreference_indices.add(new LinkedList<LinkedList<Integer>>());
			coreference_title.add(new LinkedList<String>());
			
			//process the raw text, generating files containing relation, coreference and NER data
			processRawText(file_names.get(i));
			//extract meaningful data from above mentioned files
			extractInfo(file_names.get(i), token_list.get(i), NER_tags.get(i), sentence_count.get(i), coreference_indices.get(i), coreference_title.get(i));
			//extract relations by means of the Reverb algorithm
			relations.add(extractRels(file_names.get(i), 0.5, sentence_count.get(i)));
			//perform coreference resolution
			coreferenceResolution(relations.get(i), NER_tags.get(i),coreference_indices.get(i));
			//trim relations, storing meaningful relations only
			trimRelations(relations.get(i), coreference_indices.get(i), coreference_title.get(i), entity_names2[i], i, token_list.get(i), NER_tags.get(i));
		}
		//plot relations on graph and organise graph
		Graph g = plotGraph(relations, infoBox_relations, token_list, entity_names2, step);
		//plot copy graph for moralization
		Graph g2 = plotGraph(relations, infoBox_relations, token_list, entity_names2, step);

		//perform moralization
		if(moral)
		{
			moralization(g2);
		}
		
		//perform community detection
		if(community)
		{
			if(!moral)
			{
				moralization(g2);
			}	
			communityDetection(g2);
		}
	}
	
	//arguments: set of maximal cliques r, set of vertices with potential to join a clique p, set of already processed vertices x, arraylist of cliques and graph g
	//returns: void
	//function performs Bron-Kerbosch community detection in recursive fashion
	public static void BK(Set<Node> r, Set<Node> p, Set<Node> x, ArrayList<Set<Node>> communities, Graph g)
	{
		//if p and x are empty then r is a maximal clique
		if(p.size() == 0 && x.size() == 0)
		{
			communities.add(r);
			//exit recursion
			return;
		}
		
		Set<Node> p_iter_copy = new HashSet<Node>(p);
		
		Iterator<Node> iter = p_iter_copy.iterator();
		
		//loop through all nodes in set p
		while(iter.hasNext())
		{ 
			Node v = iter.next();
			
			Set<Node> r_copy = new HashSet<Node>(r);
			Set<Node> p_copy = new HashSet<Node>(p);
			Set<Node> x_copy = new HashSet<Node>(x);
			
			//add v to set r
			r_copy.add(v);
			//intersect p with neighbours of v
			p_copy.retainAll(neighbour(v, g));
			//intersect x with neighbours of v
			x_copy.retainAll(neighbour(v, g));
			//recursively call Bron-Kerbosch with updated sets
			BK(r_copy, p_copy, x_copy, communities, g);
			
			//remove v from set p
			p.remove(v);
			//add v to set x
			x.add(v);
		}
	}
	
	//arguments: graph g
	//returns: void
	//function initialised variables for calling og Bron-Kerbosch algorithm
	public static void communityDetection(Graph g)
	{
		Set<Node> p = new HashSet<Node>();
		Set<Node> r = new HashSet<Node>();
		Set<Node> x = new HashSet<Node>();
		ArrayList<Set<Node>> communities = new ArrayList<Set<Node>>();
		
		for(Node n:g.getEachNode())
		{
			p.add(n);
		}
		
		//extract communities through Bron-Kerbosch algorithm
		BK(r, p, x, communities, g);
		
		//loop through communities, visualising the community
		for(int i = 0; i < communities.size(); i++)
		{
			community_visualisation(communities.get(i), g, i);
		}
	}
	
	//argument: set of nodes forming a community, graph, and integer identifier to be used on edges
	//returns: void
	//function visualises communities
	public static void community_visualisation(Set<Node> community, Graph g, int id)
	{
		/*Random rand = new Random();
		int c1, c2, c3;
		generate three random numbers corresponding to the R,G & B values for the colour of the community
		c1 = rand.nextInt(255);
		c2 = rand.nextInt(255);
		c3 = rand.nextInt(255);*/
		
		//map set of nodes onto array
		Node[] nodes = community.toArray(new Node[community.size()]);
		
		//create new graph for each community
		Graph g2 = new MultiGraph("");
		
		String styleSheet = "node {text-alignment: under; text-background-mode: rounded-box; text-background-color: rgb(200,200,200);}";
		g2.setAutoCreate(true);
        g2.setStrict(false);
        g2.addAttribute("ui.stylesheet", styleSheet);
		
        //add all nodes in community
		for(int i = 0; i < nodes.length; i++)
		{
			Node n = g2.addNode(nodes[i].getId());
			n.addAttribute("ui.label", n.getId());
		}
		
		//add edge between all nodes in community
		for(int i = 0; i < nodes.length-1; i++)
		{
			for(int j = i+1; j < nodes.length; j++)
			{
				g2.addEdge(nodes[i].getId()+nodes[j].getId(), nodes[i], nodes[j]);
			}
		}
		
		g2.display();
			
		//loop through all nodes in the community, creating edges between them
		/*for(int i = 0; i < nodes.length-1; i++)
		{
			for(int j = i+1; j < nodes.length; j++)
			{						
				//create new edge between nodes
				Edge e = g.addEdge("comm"+id+nodes[i].getId()+nodes[j].getId(), nodes[i], nodes[j]);
				//set colour style to new colour
				e.addAttribute("ui.style", "fill-color: rgb("+c1+","+c2+","+c3+"); size: 3;");
			}
		}*/
		
		System.out.print("Community "+(id+1)+" of "+community.size()+" nodes: ");
		for(int i = 0; i < nodes.length; i++)
		{
			if(i < nodes.length - 1)
			{
				System.out.print(nodes[i]+" - ");
			}
			else
			{
				System.out.println(nodes[i]);
			}
		}
	}
	
	
	
	//arguments: node to find neighbours of, and graph
	//returns: set of nodes neighbouring n
	//function finds all nodes neighbouring the given node n in graph g
	public static Set<Node> neighbour(Node n, Graph g)
	{
		Set<Node> neighbours = new HashSet<Node>();
		
		//loop through every edge
		for(Edge e: g.getEachEdge())
		{
			Node temp;
			//if current edge connects n to another node, add other node to the set
			if((temp = e.getOpposite(n)) != null)
			{
				neighbours.add(temp);
			}
		}
		//return the set of neighbouring nodes
		return neighbours;
	}
	
	//arguments: Graph of relations
	//returns: void
	//function performs moralization on the provided graph, converting it into an undirected, unlabelled moral graph
    public static void moralization(Graph graph)
    {
        ArrayList<Node> list = new ArrayList<Node>();
        
        //add new stylesheet to graph
        graph.addAttribute("ui.stylesheet","edge.important { fill-color: red; }");
        
        //add every node in the graph to the list
        for(Node node:graph)
        {
            list.add(node);
        }
        
        //first parent node to be compared with rest of parent nodes        
        for(int i = 0; i < list.size(); i++) 
        {
            Node node1 = list.get(i);
            
            //loop through edges leaving node 1
            for(Edge node1LeavingEdge : node1.getEachLeavingEdge())
            {                   
            	//rest of parent nodes
            	for(int j = 0; j < list.size(); j++)
                {
                    Node node2 = list.get(j);
                    
                    //loop through edges leaving node 2
                    for(Edge node2LeavingEdge : node2.getEachLeavingEdge())
                    {
                    	//if source nodes are different, and source nodes both target the same node, then enter
	                    if (!node1.equals(node2) && node2LeavingEdge.getTargetNode().equals(node1LeavingEdge.getTargetNode()))
	                    {
	                        //if there isn't an edge between the two source nodes, add one and set it to be red
	                        if (!node2.hasEdgeBetween(node1))
	                        {
	                            Edge e = graph.addEdge(node1.getId() + node2.getId(), node1, node2);
	                            e.addAttribute("ui.class", "important");  
	                        }
	                    }
                    }
                }               
            }                   
        }
   
        //loop through all edges, removing direction
        for(Edge edge:graph.getEachEdge())
        {
            if (edge.isDirected())
            {
                String edgeId = edge.getId();
                Node sourceNode = edge.getSourceNode();
                Node targetNode = edge.getTargetNode();
                graph.removeEdge(edge);
                graph.addEdge(edgeId, sourceNode, targetNode);
                edge.removeAttribute("ui.label");
            }
        }
        
        //loop through all edges, removing any repeated, undirected edges from the graph
        for(Edge e: graph.getEachEdge())
        {
        	for(Edge e2: graph.getEachEdge())
        	{
        		//if e connexts the same two nodes as e2, then delete e2
        		if(e != e2 && ((e.getSourceNode() == e2.getTargetNode() && e.getTargetNode() == e2.getSourceNode()) || (e.getSourceNode() == e2.getSourceNode() && e.getTargetNode() == e2.getTargetNode())))
        		{
        			graph.removeEdge(e2);
        		}
        	}
        }
    }
	
	//arguments: linked list of relations for specific main entity, coreference indices and titles, entity name and number, list of tokens and NERs
	//returns: void
	//function trims down relations to ones which are relevant
	public static void trimRelations(LinkedList<LinkedList<LinkedList<Integer>>> relations, LinkedList<LinkedList<Integer>> coreference_indices, LinkedList<String> coreference_title, String entity_name, int entity_num, LinkedList<String> token_list, LinkedList<String> NER_tags)
	{
		Boolean entered = false;
		String part1, part2;
		//split entity name and surname
		part1 = entity_name.split("\\s+")[0];
		part2 = entity_name.split("\\s+")[1];
		
		int ind1 = 0, ind2 = 0;
		
		//find tokens corresponding to current entity name
		for(int i = 0; i < token_list.size()-1; i++)
		{
			if(token_list.get(i).equals(part1) && token_list.get(i+1).equals(part2))
			{
				ind1 = i;
				break;
			}
		}
		
		//find coreference chain corresponding to coreference of curren entity name
		ind2 = getCorefInd(coreference_indices,ind1);
		
		//loop through all arg1s of relations
		for(int i = 0; i < relations.get(0).size(); i++)
		{
			//if current arg1 relates to current entity
			if(relations.get(0).get(i).get(0) == ind2)
			{
				//set arg1 to hold full entity name
				relations.get(0).get(i).clear();
				relations.get(0).get(i).add(ind1);
				relations.get(0).get(i).add(ind1+1);
			}
			else
			{
				//otherwise remove relation as it does not relate current entity to anything
				relations.get(0).remove(i);
				relations.get(1).remove(i);
				relations.get(2).remove(i--);
			}
		}
		
		//loop through all arg2s in relations
		for(int i = 0; i < relations.get(2).size(); i++)
		{
			//loop through all tokens forming arg2
			for(int j = 0; j < relations.get(2).get(i).size(); j++)
			{
				//if current token is not an NER then remove it
				if(NER_tags.get(relations.get(2).get(i).get(j)).equals("O"))
				{
					//if we have already read an NER, then remove
					if(entered)
					{
						relations.get(2).get(i).remove(j--);
					}
					//otherwise move token to relator between arg1 and arg2
					else
					{
						relations.get(1).get(i).add(relations.get(2).get(i).get(j));
						relations.get(2).get(i).remove(j--);
					}
				}
				else
				{
					entered = true;
				}
			}
			entered = false;
			
			//if arg2 is now empty, delete relation
			if(relations.get(2).get(i).size() < 1){
				relations.get(0).remove(i);
				relations.get(1).remove(i);
				relations.get(2).remove(i--);
			}
		}
	}
	
	//arguments: linked list of relations, NER tags and coreference indices
	//returns: void
	//function performs coreference resolution of tokens involves in relations
	@SuppressWarnings("unchecked")
	public static void coreferenceResolution(LinkedList<LinkedList<LinkedList<Integer>>> relations, LinkedList<String> NER_tags, LinkedList<LinkedList<Integer>> coreference_indices)
	{
		int index = 0;
		LinkedList<Integer> indices = new LinkedList<Integer>();
		
		//loop through relations
		for(int  i = 0; i < relations.get(0).size(); i++)
		{
			//loop through indices of current attribute
			for(int j = 0; j < relations.get(0).get(i).size(); j++)
			{
				//find out whether current token corefers
				index = getCorefInd(coreference_indices, relations.get(0).get(i).get(j));
				
				//if not, store previous value
				if(index == -1)
				{
					indices.add(relations.get(0).get(i).get(j));
				}
				//if so, update index to point to head of coreference
				else
				{
					for(int k = 0; k < coreference_indices.get(index).size()-1; k++)
					{
						indices.add(coreference_indices.get(index).get(k));
						if(coreference_indices.get(index).get(k)+1 != coreference_indices.get(index).get(k+1))
						{
							break;
						}
					}
				}
			}
			
			//loop through relation attribute
			for(int j = 0; j < indices.size()-1; j++)
			{
				//if token is repeated, remove it
				if(indices.get(j) == indices.get(j+1))
				{
					indices.remove(j+1);
				}
			}
			//update relations
			relations.get(0).set(i, (LinkedList<Integer>)indices.clone());
			
			indices.clear();
		}
	}
	
	//arguments: list of coreferencing indices and index to search for
	//returns: index of coreference row current index belongs to
	//function searches through corefering tokens, finding which set of corefering tokens the given index belongs to
	public static int getCorefInd(LinkedList<LinkedList<Integer>> coreference_indices, int index)
	{
		//loop through corefering rows
		for(int i = 0; i < coreference_indices.size(); i++)
		{
			//loop through corefering indices
			for(int j = 0; j < coreference_indices.get(i).size(); j++)
			{
				if(index == coreference_indices.get(i).get(j))
				{
					return i;
				}
			}
		}
		return -1;
	}
		
	//arguments: Nested Linked List of relations, Linked list of tokens, name of entity and number of steps to allow from all main entities and any other node
	//returns: Graph of relations
	//function performs graph visualisation of relations 
	public static Graph plotGraph(LinkedList<LinkedList<LinkedList<LinkedList<Integer>>>> relations, LinkedList<LinkedList<LinkedList<String>>> infoBox_rels, LinkedList<LinkedList<String>> token_list, String[] entity_names2, int step)
	{
		//create new graph entity
		Graph graph = new MultiGraph("Entity Relator");
		//Set graph rendering properties
		System.setProperty("org.graphstream.ui.renderer", "org.graphstream.ui.j2dviewer.J2DGraphRenderer");
		graph.addAttribute("ui.quality");

		//set main entities to be red
		String styleSheet = "node.mainEntity { fill-color: red;} node { text-alignment: under; text-background-mode: rounded-box; text-background-color: rgb(200,200,200);}";
		graph.setAutoCreate(true);
        graph.setStrict(false);
        graph.addAttribute("ui.stylesheet", styleSheet);
        
        //display the graph
        graph.display();
        
        LinkedList<LinkedList<Node>> arg1_nodes = new LinkedList<LinkedList<Node>>();
        LinkedList<LinkedList<Node>> arg2_nodes = new LinkedList<LinkedList<Node>>();
        LinkedList<LinkedList<Edge>> edges = new LinkedList<LinkedList<Edge>>();
        
        String arg1 = "", arg2 = "", rel = "";
        int tot = 0;
        boolean okay = false, dont = false;
        
        //loop through each relation
        for(int i = 0; i < relations.size(); i++)
        {
        	arg1_nodes.add(new LinkedList<Node>());
           	arg2_nodes.add(new LinkedList<Node>());
           	edges.add(new LinkedList<Edge>());
        	
        	for(int j = 0; j < relations.get(i).get(0).size(); j++)
        	{
        		arg1 = rel = arg2 = "";
        		//Concatenate words for each part of the relation
        		for(int k = 0; k < relations.get(i).get(0).get(j).size(); k++)
        		{
        			arg1 += token_list.get(i).get(relations.get(i).get(0).get(j).get(k))+ " ";
        		}
        		
        		for(int k = 0; k < relations.get(i).get(1).get(j).size(); k++)
        		{
        			rel += token_list.get(i).get(relations.get(i).get(1).get(j).get(k))+ " ";
        		}
        		
        		for(int k = 0; k < relations.get(i).get(2).get(j).size(); k++)
        		{
        			arg2 += token_list.get(i).get(relations.get(i).get(2).get(j).get(k))+ " ";
        		}
        		
        		arg1 = arg1.trim();
        		rel = rel.trim();
        		arg2 = arg2.trim();
        		
        		System.out.println(arg1+"|"+rel+"|"+arg2);
        		
        		Node temp_node1, temp_node2;

        		//create arg1 and arg2 nodes
    			temp_node1 = graph.addNode(arg1);
        		arg1_nodes.get(i).add(temp_node1);


        		//mark first argument in relation as main entity if it relates to one of the main entites
        		for(int k = 0; k < entity_names2.length; k++)
        		{
        			if(entity_names2[k].equals(arg1))
        			{
                		temp_node1.setAttribute("ui.class", "mainEntity");
                		temp_node1.setAttribute("xyz", 30*i+5, 300-(i-relations.size()/2)*30, 0);
                		break;
        			}
        		}

        		//create arg1 and arg2 nodes
    			temp_node2 = graph.addNode(arg2);
        		arg2_nodes.get(i).add(temp_node2);
        		
        		//create new edge
        		edges.get(i).add(graph.addEdge(rel+i+j, temp_node1, temp_node2, true));
        		
        		//display argument labels
        		temp_node1.addAttribute("ui.label", arg1);
        		temp_node2.addAttribute("ui.label", arg2);
        		
        		//display edge labels
        		edges.get(i).get(j).addAttribute("ui.label", rel);
        		
        		arg1 = rel = arg2 = "";
        	}
        	
        	//store last index of linked list for traversal purposes
        	tot = relations.get(i).get(0).size();
        	
        	for(int j = 0; j < infoBox_rels.get(i).size(); j++)
        	{
        		arg1 = infoBox_rels.get(i).get(j).get(0).trim();
        		rel = infoBox_rels.get(i).get(j).get(1).trim();
        		arg2 = infoBox_rels.get(i).get(j).get(2).trim();
        		
        		System.out.println(arg1+"|"+rel+"|"+arg2);
        		
        		Node temp_node1, temp_node2;
        	
        		//create arg1 and arg2 nodes
    			temp_node1 = graph.addNode(arg1);
        		arg1_nodes.get(i).add(temp_node1);

        		//mark first argument in relation as main entity if it relates to one of the main entites
        		for(int k = 0; k < entity_names2.length; k++)
        		{
        			if(entity_names2[k].equals(arg1))
        			{
                		temp_node1.setAttribute("ui.class", "mainEntity");
                		break;
        			}
        		}

        		//create arg1 and arg2 nodes
    			temp_node2 = graph.addNode(arg2);
        		arg2_nodes.get(i).add(temp_node2);
        		
        		//create new edge
        		edges.get(i).add(graph.addEdge(rel+i+j, temp_node1, temp_node2, true));
        		
        		//display argument labels
        		temp_node1.addAttribute("ui.label", arg1);
        		temp_node2.addAttribute("ui.label", arg2);
        		
        		//display edge labels
        		edges.get(i).get(j+tot).addAttribute("ui.label", rel);
        	}
        }
        
        //loop through different entity rel lists
        for(int i = 0; i < relations.size(); i++)
        {
        	//loop through all nodes, removing any node with only one edge
        	for(int j = 0; j < arg2_nodes.get(i).size(); j++)
        	{
        		System.out.println(arg1_nodes.get(i).get(j).getId()+"|"+edges.get(i).get(j).getId()+"|"+arg2_nodes.get(i).get(j).getId());
         		okay = false;
        		
        		//each node must be connected to atleast 1 main entity
    			for(Edge e: arg2_nodes.get(i).get(j).getEachEdge())
    			{
    				dont = false;
    				for(int k = 0; k < entity_names2.length; k++)
    				{
    					//if current node is a main entity, then enter
    					if(entity_names2[k].equals(arg2_nodes.get(i).get(j).getId()))
    					{
    						dont = true;
    						break;
    					}
    				}
    				
    				//if current node has an edge connecting it to a main entity or is a main entity itself then it is fine
    				if(dont || e.getOpposite(arg2_nodes.get(i).get(j)).hasAttribute("ui.class"))
    				{
    					okay = true;
    					break;
    				}
    			}
    			
    			//otherwise delete it
    			if(!okay)
    			{
    				for(Edge e:arg2_nodes.get(i).get(j).getEachEdge())
        			{
        				graph.removeEdge(e);
        			}
    				edges.get(i).remove(j);
    				if(graph.getNode(arg2_nodes.get(i).get(j).getId()) != null)
    				{
    					graph.removeNode(arg2_nodes.get(i).get(j));
    				}
        			arg2_nodes.get(i).remove(j);
        			arg1_nodes.get(i).remove(j);
        			j--;
        			continue;
    			}
    			else if(dont)
    			{
    				continue;
    			}
        		
        		//if any node has a degree less than 2, remove it
        		if(arg2_nodes.get(i).get(j).getDegree() < 2)
        		{
        			for(Edge e:arg2_nodes.get(i).get(j).getEachEdge())
        			{
        				graph.removeEdge(e);
        			}
        			edges.get(i).remove(j);
        			if(graph.getNode(arg2_nodes.get(i).get(j).getId()) != null)
    				{
        				graph.removeNode(arg2_nodes.get(i).get(j));
    				}
        			arg2_nodes.get(i).remove(j);
        			arg1_nodes.get(i).remove(j);
        			j--;
        			continue;
        		}
        		//if node has a degree greater than 1, check whether degree exists with same node
        		else
        		{
        			int count = 1;
        			//loop through each edge
        			for(Edge e: arg2_nodes.get(i).get(j).getEachEdge())
        			{
        				for(Edge e2: arg2_nodes.get(i).get(j).getEachEdge())
        				{
        					if(e.getOpposite(arg2_nodes.get(i).get(j)) != e2.getOpposite(arg2_nodes.get(i).get(j)))
							{
        						//count number of different nodes having an edge to the current node
        						count++;
							}
        				}
        				break;
        			}
        			
        			//if node is connected to less than two different nodes, delete it
        			if(count < 2)
        			{
        				for(Edge e:arg2_nodes.get(i).get(j).getEachEdge())
            			{
            				graph.removeEdge(e);
            			}
        				edges.get(i).remove(j);
        				if(graph.getNode(arg2_nodes.get(i).get(j).getId()) != null)
        				{
        					graph.removeNode(arg2_nodes.get(i).get(j));
        				}
            			arg2_nodes.get(i).remove(j);
            			arg1_nodes.get(i).remove(j);
            			j--;
        			}
        			count = 1;
        			okay = false;
        		}
        	}
        	
        	//loop through all nodes, removing any node with only one edge
        	for(int j = 0; j < arg1_nodes.get(i).size(); j++)
        	{
        		okay = false;
        		//each node must be connected to atleast 1 main entity
    			for(Edge e: arg1_nodes.get(i).get(j).getEachEdge())
    			{
    				dont = false;
    				for(int k = 0; k < entity_names2.length; k++)
    				{
    					//if current node is a main entity node, enter
    					if(entity_names2[k].equals(arg1_nodes.get(i).get(j).getId()))
    					{
    						dont = true;
    						break;
    					}
    				}
    				
    				//if current node has an edge connecting it to a main entity or it is the main entity then it is fine
    				if(dont || e.getOpposite(arg1_nodes.get(i).get(j)).hasAttribute("ui.class"))
    				{
    					okay = true;
    					break;
    				}
    			}
    			
    			//otherwise delete it
    			if(!okay)
    			{
    				for(Edge e:arg1_nodes.get(i).get(j).getEachEdge())
        			{
        				graph.removeEdge(e);
        			}
    				edges.get(i).remove(j);
    				if(graph.getNode(arg1_nodes.get(i).get(j).getId()) != null)
    				{
    					graph.removeNode(arg1_nodes.get(i).get(j));
    				}
        			arg1_nodes.get(i).remove(j);
        			arg2_nodes.get(i).remove(j);
        			j--;
        			continue;
    			}
        		
        		//if any node has a degree less than 2, remove it
        		if(arg1_nodes.get(i).get(j).getDegree() < 2)
        		{
        			for(Edge e:arg1_nodes.get(i).get(j).getEachEdge())
        			{
        				graph.removeEdge(e);
        			}
        			edges.get(i).remove(j);
        			if(graph.getNode(arg1_nodes.get(i).get(j).getId()) != null)
    				{
        				graph.removeNode(arg1_nodes.get(i).get(j));
    				}
        			arg1_nodes.get(i).remove(j);
        			arg2_nodes.get(i).remove(j);
        			j--;
        			continue;
        		}
        		//if node has a degree greater than 1, check whether degree exists with same node
        		else
        		{
        			int count = 1;
        			for(Edge e: arg1_nodes.get(i).get(j).getEachEdge())
        			{
        				for(Edge e2: arg1_nodes.get(i).get(j).getEachEdge())
        				{
        					if(e.getOpposite(arg1_nodes.get(i).get(j)) != e2.getOpposite(arg1_nodes.get(i).get(j)))
							{
        						//count number of different nodes having an edge to the current node
        						count++;
							}
        				}
        				break;
        			}
        			
        			//if node is connected to less than two different nodes, delete it
        			if(count < 2)
        			{
        				for(Edge e:arg1_nodes.get(i).get(j).getEachEdge())
            			{
            				graph.removeEdge(e);
            			}
        				edges.get(i).remove(j);
        				if(graph.getNode(arg1_nodes.get(i).get(j).getId()) != null)
        				{
        					graph.removeNode(arg1_nodes.get(i).get(j));
        				}
            			arg1_nodes.get(i).remove(j);
            			arg2_nodes.get(i).remove(j);
            			j--;
        			}
        			count = 1;
        			okay = false;
        		}
        	}
        }
        
      //loop through different entity rel lists
        for(int i = 0; i < relations.size(); i++)
        {
        	//loop through all nodes, removing any node with only one edge
        	for(int j = 0; j < arg2_nodes.get(i).size(); j++)
        	{
         		okay = false;
        		
        		//if any node has a degree less than 2, remove it
        		if(arg2_nodes.get(i).get(j).getDegree() < 2)
        		{
        			for(Edge e:arg2_nodes.get(i).get(j).getEachEdge())
        			{
        				graph.removeEdge(e);
        			}
        			edges.get(i).remove(j);
        			if(graph.getNode(arg2_nodes.get(i).get(j).getId()) != null)
    				{
        				graph.removeNode(arg2_nodes.get(i).get(j));
    				}
        			arg2_nodes.get(i).remove(j);
        			arg1_nodes.get(i).remove(j);
        			j--;
        			continue;
        		}
        		//if node has a degree greater than 1, check whether degree exists with same node
        		else
        		{
        			int count = 1;
        			//loop through each edge
        			for(Edge e: arg2_nodes.get(i).get(j).getEachEdge())
        			{
        				for(Edge e2: arg2_nodes.get(i).get(j).getEachEdge())
        				{
        					if(e.getOpposite(arg2_nodes.get(i).get(j)) != e2.getOpposite(arg2_nodes.get(i).get(j)))
							{
        						//count number of different nodes having an edge to the current node
        						count++;
							}
        				}
        				break;
        			}
        			
        			//if node is connected to less than two different nodes, delete it
        			if(count < 2)
        			{
        				for(Edge e:arg2_nodes.get(i).get(j).getEachEdge())
            			{
            				graph.removeEdge(e);
            			}
        				edges.get(i).remove(j);
        				if(graph.getNode(arg2_nodes.get(i).get(j).getId()) != null)
        				{
        					graph.removeNode(arg2_nodes.get(i).get(j));
        				}
            			arg2_nodes.get(i).remove(j);
            			arg1_nodes.get(i).remove(j);
            			j--;
        			}
        			count = 1;
        			okay = false;
        		}
        	}
        	
        	//loop through all nodes, removing any node with only one edge
        	for(int j = 0; j < arg1_nodes.get(i).size(); j++)
        	{
        		okay = false;
        		
        		//if any node has a degree less than 2, remove it
        		if(arg1_nodes.get(i).get(j).getDegree() < 2)
        		{
        			for(Edge e:arg1_nodes.get(i).get(j).getEachEdge())
        			{
        				graph.removeEdge(e);
        			}
        			edges.get(i).remove(j);
        			if(graph.getNode(arg1_nodes.get(i).get(j).getId()) != null)
    				{
        				graph.removeNode(arg1_nodes.get(i).get(j));
    				}
        			arg1_nodes.get(i).remove(j);
        			arg2_nodes.get(i).remove(j);
        			j--;
        			continue;
        		}
        		//if node has a degree greater than 1, check whether degree exists with same node
        		else
        		{
        			int count = 1;
        			for(Edge e: arg1_nodes.get(i).get(j).getEachEdge())
        			{
        				for(Edge e2: arg1_nodes.get(i).get(j).getEachEdge())
        				{
        					if(e.getOpposite(arg1_nodes.get(i).get(j)) != e2.getOpposite(arg1_nodes.get(i).get(j)))
							{
        						//count number of different nodes having an edge to the current node
        						count++;
							}
        				}
        				break;
        			}
        			
        			//if node is connected to less than two different nodes, delete it
        			if(count < 2)
        			{
        				for(Edge e:arg1_nodes.get(i).get(j).getEachEdge())
            			{
            				graph.removeEdge(e);
            			}
        				edges.get(i).remove(j);
        				if(graph.getNode(arg1_nodes.get(i).get(j).getId()) != null)
        				{
        					graph.removeNode(arg1_nodes.get(i).get(j));
        				}
            			arg1_nodes.get(i).remove(j);
            			arg2_nodes.get(i).remove(j);
            			j--;
        			}
        			count = 1;
        			okay = false;
        		}
        	}
        }
        
        //remove any edge which is a repetition
        for(Edge e: graph.getEachEdge())
    	{
    		for(Edge e2: graph.getEachEdge())
    		{
    			//if edge with same title between same two nodes exists, delete
    			if(!e.getId().equals(e2.getId()) && e.getAttribute("ui.label").equals(e2.getAttribute("ui.label")) && e.getSourceNode().getId().equals(e2.getSourceNode().getId()) && e.getTargetNode().getId().equals(e2.getTargetNode().getId()))
    			{
    				graph.removeEdge(e2);
    			}
    		}
    	}
        
		LinkedList<Node> queue = new LinkedList<Node>();
		LinkedList<Integer> depth = new LinkedList<Integer>();
        
		//delete nodes which are not reachable by all main entities in the specified number of steps
        for(Node n: graph.getEachNode())
        {
        	//if source node is a main entity, enter
    		if(n.hasAttribute("ui.class") && n.getAttribute("ui.class").equals("mainEntity"))
    		{	
    			//create array list of all nodes in the graph
    			List<Node> nodes = Lists.newArrayList(graph.getEachNode().iterator());
    			
    			//loop through all nodes  in the graph
		    	for(int i = 0; i < nodes.size(); i++)
		    	{
		    		System.out.println(n.getId()+"||"+nodes.get(i).getId());
        		
        			queue.clear();
        			depth.clear();
        			queue.add(n);
        			depth.add(0);
        			
        			//if target node is not a main entity and cannot be reached from the source main entity in the specified number of steps, delete
        			if((!nodes.get(i).hasAttribute("ui.class") || !nodes.get(i).getAttribute("ui.class").equals("mainEntity")) && !reachable(nodes.get(i), queue, depth, step))
        			{
        				graph.removeNode(nodes.get(i));
        				nodes.remove(i--);
        			}
        		}
        	}
        }
        return graph;
	}
	
	//arguments: Node to find, queue with start node, queue with start depth (0), number of steps to consider 
	//returns: Boolean valu signifying whether path from start to end nodes with edge jumps <= steps
	//function performs BFS starting from the start node and returning value based on search result
	public static boolean reachable(Node end, LinkedList<Node> queue, LinkedList<Integer> depth, int steps)
	{
		//increment steps by 1 due to implementation
		steps++;
		
		//perform BFS for end node
		for(int i = 0; i < steps*10; i++)
		{
			//if current node is target node and we have completed less steps than required, return true
			if(queue.get(0) == end && depth.get(0) < steps)
			{
				return true;
			}
			else
			{
				//find every edge emanating from the current node
				for(Edge e: queue.get(0).getEachLeavingEdge())
				{
					//add every adjacent nodes to queue and update depths
					queue.add(e.getTargetNode());
					depth.add(depth.get(0)+1);
				}
				//remove head of both queues
				queue.remove(0);
				depth.remove(0);
			}
		}
		//search failed
		return false;
	}
	
	@SuppressWarnings("unchecked")
	//arguments: file name, double rating for threshold of relation confidence, linked list of sentence counts
	//returns: Triple nested linked list of relations in the form of <arg1, rel, arg2>
	//function runs reverb relation extraction upon entity raw text, extracting relations and organising them into a linked list
	public static LinkedList<LinkedList<LinkedList<Integer>>> extractRels(String filename, double threshold, LinkedList<Integer> sentence_count) throws IOException
	{
		//Run reverb relation extraction
        ProcessBuilder builder2 = new ProcessBuilder("cmd.exe", "/c", "cd \"\\NLP\" && java -Xmx512m -jar reverb.jar \""+filename+".txt\"");
        builder2.redirectErrorStream(true);
        Process p = builder2.start();
        //divert command line output to store in String variable
        BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
        int kk = 0, start = 0, end = 0, sent = 0, tokenIndex = 0;
        String line = "";
        String[] splitData = new String[18];
        LinkedList<String[]> relationData = new LinkedList<String[]>();
        LinkedList<LinkedList<LinkedList<Integer>>> token_relations = new LinkedList<LinkedList<LinkedList<Integer>>>(); 
        
        while (true)
        {
            line = r.readLine();
            if (line == null || line.equals("Done with extraction.")) { break; }
            
            if(kk < 5)
            {
	            System.err.println(line);
	            kk++;
            }
            else
            {
            	//split command line data at each tab point (column in table)
	            splitData = line.split("\t");
	            relationData.add(splitData.clone());
            }
        }
        
        for(int j = 0; j < relationData.size(); j++)
        {
        	if(Double.parseDouble(relationData.get(j)[11]) < threshold)
        	{
        		relationData.remove(j--);
        	}
        }

        //create string array to store relational data for each token
        LinkedList<LinkedList<Integer>> tokenRelationsArg1 = new LinkedList<LinkedList<Integer>>();
        LinkedList<LinkedList<Integer>> tokenRelationsRel = new LinkedList<LinkedList<Integer>>();
        LinkedList<LinkedList<Integer>> tokenRelationsArg2 = new LinkedList<LinkedList<Integer>>();
        LinkedList<Integer> tempList = new LinkedList<Integer>();
        
        //loop through each relation for current entity
        for(int j = 0; j < relationData.size(); j++)
        {
        	//store sentence number
        	sent = Integer.parseInt(relationData.get(j)[1]);
        	
        	//store start and end token indices of first argument
        	start = Integer.parseInt(relationData.get(j)[5]);
        	end = Integer.parseInt(relationData.get(j)[6]);
        	
        	tokenIndex = 0;
        	
        	for(int k = 0; k < sent; k++)
        	{
        		tokenIndex += sentence_count.get(k);
        	}
        	
        	//update relational array to mark tokens which form part of the first argument for the jth relationship
        	for(int k = 0; k < (end-start); k++)
        	{
        		tempList.add(tokenIndex + start + k);
        	}
        	
        	tokenRelationsArg1.add((LinkedList<Integer>)tempList.clone());
        	tempList.clear();
        	
        	//store start and end token indices of relation attribute
        	start = Integer.parseInt(relationData.get(j)[7]);
        	end = Integer.parseInt(relationData.get(j)[8]);
        	
        	//update relational array to mark tokens which form part of the relational operator for the jth relationship
        	for(int k = 0; k < (end-start); k++)
        	{
        		tempList.add(tokenIndex + start + k);
        	}
        	
        	tokenRelationsRel.add((LinkedList<Integer>)tempList.clone());
        	tempList.clear();
        	
        	//store start and end token indices of second argument
        	start = Integer.parseInt(relationData.get(j)[9]);
        	end = Integer.parseInt(relationData.get(j)[10]);
        	
        	//update relational array to mark tokens which form part of the second argument for the jth relationship
        	for(int k = 0; k < (end-start); k++)
        	{
        		tempList.add(tokenIndex + start + k);
        	}
        	
        	tokenRelationsArg2.add((LinkedList<Integer>)tempList.clone());
        	tempList.clear();
        	
        	//output relationship together with confidence rating
        	//System.out.println(relationDataAll.get(i).get(j)[15]+" | "+relationDataAll.get(i).get(j)[16]+" | "+relationDataAll.get(i).get(j)[17]+" | "+relationDataAll.get(i).get(j)[11]);
        }
        token_relations.add((LinkedList<LinkedList<Integer>>)tokenRelationsArg1.clone());
        token_relations.add((LinkedList<LinkedList<Integer>>)tokenRelationsRel.clone());
        token_relations.add((LinkedList<LinkedList<Integer>>)tokenRelationsArg2.clone());
        
        return token_relations;
	}
	
	@SuppressWarnings("unchecked")
	//arguments: file name, empty linked lists to store tokens, NER tags, integer counts for each sentence, indices of corefering tokens, titles of corefering tokens
	//returns: void
	//function parses JSON data stored in .txt.xml file produced by processRawText
	public static void extractInfo(String filename, LinkedList<String> token_list, LinkedList<String> NER_tags, LinkedList<Integer> sentence_count, LinkedList<LinkedList<Integer>> coreference_indices, LinkedList<String> coreference_title) throws ParserConfigurationException, SAXException, IOException
	{
		LinkedList<Integer> coreference = new LinkedList<Integer>();
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        
        File file = new File(filename+".txt.xml");
        Document doc = builder.parse(file);
        NodeList nl = doc.getElementsByTagName("*");
        
        Element el;
        Boolean entered = false, rep = false;
        int tokenCount = 0, sent = 0, start = 0, end = 0, total = 0;
        
        //processing of xml file to extract data
        for (int ii = 0; ii < nl.getLength(); ii++)
        {
        	el = (Element)nl.item(ii);
        	
        	//add each token to linked list
        	if(el.getNodeName().equals("word"))
        	{
        		token_list.add(el.getTextContent());
        	}
        	
        	//save NER value of each token
        	if(el.getNodeName().equals("NER"))
        	{
        		NER_tags.add(el.getTextContent());
        	}
        	
        	//increment on each token encounter
        	if(el.getNodeName().equals("token"))
        	{
        		tokenCount++;
        	}
        	
        	//count number of tokens in each sentence and save
        	if(el.getNodeName().equals("sentence") && el.hasAttribute("id"))
        	{
        		sentence_count.add(tokenCount);
        		tokenCount = 0;
        	}
        	
        	if(el.getNodeName().equals("mention"))
        	{
        		//once coreference section is reached, all sentences have been passed so we may add the final sentence token count which was skipped
        		if(!entered)
        		{
        			entered = true;
                    sentence_count.add(tokenCount);
        		}
        		
        		//new coreference entity
        		if(el.hasAttribute("representative"))
        		{
        			rep = true;
        			coreference_indices.add((LinkedList<Integer>)coreference.clone());
        			coreference.clear();
        		}
        			
        		//store copy of sentence, start and end values to compute token index
    			sent = Integer.parseInt(nl.item(++ii).getTextContent());
    			start = Integer.parseInt(nl.item(++ii).getTextContent());
    			end = Integer.parseInt(nl.item(++ii).getTextContent());
    			
    			if(rep)
    			{
    				rep = false;
    				++ii;
    				coreference_title.add(nl.item(++ii).getTextContent());
    			}
    			
    			//computation for token number of current coreference example
    			for(int j = 0; j < sent; j++)
    			{
    				total += sentence_count.get(j);
    			}
    			
    			for(int j = start; j < end; j++)
    			{
    				coreference.add(total+j - 1);
    			}
    			total = 0;
        	}
        }
        //remove first as it is empty
        coreference_indices.removeFirst();
        //add last as it was never added
        coreference_indices.add(coreference);  
	}
	
	//arguments: Name of file to process
	//returns: void
	//function processes raw text for NER, coreference and other data
	public static void processRawText(String filename) throws IOException
	{
		String line;
		File f = new File(filename+".txt.xml");
		//see if file has already been processed by Stanford CoreNLP
		if(!f.exists())
		{
			//run command line statement to process page text for each entity
			ProcessBuilder builder = new ProcessBuilder("cmd.exe", "/c", " java -cp \"*\" -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file \""+filename+".txt\"");
	        builder.redirectErrorStream(true);
	        Process p = builder.start();
	        //output command line data to console for logging purposes
	        BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
	        while (true)
	        {
	            line = r.readLine();
	            if (line == null) { break; }
	            System.out.println(line);
	        }
		}            
	}
	
	//arguments: name of file to retrieve data
	//returns: file name
	//function queries wikipedia for the raw text of the wikipedia page for the given entity
	public static String getRawText(String entity_name, String entity_name2) throws JSONException, MalformedURLException
	{
		File f = new File(entity_name2+".txt");
		if(!f.exists())
		{
			String url, raw_text = "";
			String[] pageData = new String[2];
	    	String tempData = "", inputLine = "", sourceCode = "";
	
	    	//wikipedia request
			url = "https://en.wikipedia.org/w/api.php?format=json&action=query&prop=extracts&explaintext&titles=" + entity_name;
			System.err.println("Requesting data for "+entity_name);
			
			//Send Request to Wikipedia
	        try {
	            URL ur = new URL(url);
	            HttpURLConnection yc = (HttpURLConnection) ur.openConnection();
	            BufferedReader in = new BufferedReader(new InputStreamReader(yc.getInputStream()));
	            
	            //read request response into String variable
	            while ((inputLine = in.readLine()) != null) {
	                sourceCode = sourceCode + "\n" + inputLine;
	            }
	            in.close();
	            
	            //traverse JSON response to extract required data
	            JSONObject response = new JSONObject(sourceCode);
	            JSONObject queryVal = response.getJSONObject("query");
	            tempData = queryVal.getJSONObject("pages").toString();
	            
	            pageData[1] = tempData.substring(2, tempData.indexOf("\":"));
	            raw_text = queryVal.getJSONObject("pages").getJSONObject(pageData[1]).get("extract").toString();
	            pageData[0] = queryVal.getJSONObject("pages").getJSONObject(pageData[1]).get("title").toString();
	
	            
	        } catch (IOException e) {
	            System.err.println("Error in link: " + url);
	        }
	        //save response in text file
	        try(PrintWriter out = new PrintWriter(pageData[0]+".txt"))
	        {
	        	//remove all brackets and data inside them (due to relation extraction code)
	        	raw_text = raw_text.replaceAll("\\(.*?\\) ?", "");
	        	//remove all page headers
	        	raw_text = raw_text.replaceAll("\\===.*?\\=== ?", "");
	        	//remove all page headers
	        	raw_text = raw_text.replaceAll("\\==.*?\\== ?", "");
	        	out.println(raw_text);
	        }
	        catch(IOException e)
	        {
	        	System.err.println(e);
	        }

	        return pageData[0];
		}
		return entity_name2;
	}
	
	//arguments: name of entity to retrieve info-box data for
	//returns: Nested linked list of relationships for current entity
	//function retrieves and extract relations from the info box of the current entity
	@SuppressWarnings("unchecked")
	public static LinkedList<LinkedList<String>> getInfoBoxRels(String entity_name, String entity_name2, int entity_num, LinkedList<String> page_links, Boolean recursive) throws JSONException, MalformedURLException
	{
		String sourceCode = "", inputLine = "", url, tempData = "", pageData = "", revision_data = "", tempString = "";
		LinkedList<String> sents = new LinkedList<String>();
		LinkedList<LinkedList<String>> rels = new LinkedList<LinkedList<String>>();
		LinkedList<LinkedList<String>> temp_rels2 = new LinkedList<LinkedList<String>>();
		
        //wikipedia request for page info box
        url = "https://en.wikipedia.org/w/api.php?action=query&prop=revisions&rvprop=content&format=json&rvsection=0&titles=" + entity_name;
        
        try {
            URL ur = new URL(url);
            HttpURLConnection yc = (HttpURLConnection) ur.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(yc.getInputStream()));
            
            //read in response from API request
            while ((inputLine = in.readLine()) != null) {
                sourceCode = sourceCode + "\n" + inputLine;
            }
            in.close();
            
            //traverse JSON response
            JSONObject response = new JSONObject(sourceCode);
            JSONObject queryVal = response.getJSONObject("query");
            tempData = queryVal.getJSONObject("pages").toString();
            
            pageData = tempData.substring(2, tempData.indexOf("\":"));
            if(!queryVal.getJSONObject("pages").getJSONObject(pageData).has("revisions"))
        	{
            	return null;
        	}
            
            JSONArray revision = queryVal.getJSONObject("pages").getJSONObject(pageData).getJSONArray("revisions");//.getJSONObject("rev");

        	JSONObject temp_data = revision.getJSONObject(0);
        	revision_data = temp_data.getString("*");
        	
        	//System.out.println(revision_data);
        	
        	int j = 0;
        	String temp;
        	
        	//Organise and extract lines holding useful data
        	for (int pos = revision_data.indexOf("\n"); pos != -1; pos = revision_data.indexOf("\n", pos + 1)) 
        	{
        		//seperate response by new line chars
        		temp = revision_data.substring(j, pos);
        		j = pos+1;
        		//if current line holds useful info, enter
        		if(temp.length() > 0 && temp.charAt(0) == '|')
        		{
        			//ignore first c='|' char
        			sents.add(temp.substring(1));
        		}
    		}
        	
        	int k = 0, tempi = 0;
        	j = 0;
        	String rel = "", arg2 = "";
        	LinkedList<String> temp_rels = new LinkedList<String>();
        	
        	//loop through each line
        	for(int i = 0; i < sents.size(); i++)
        	{
        		j = k = 0;
        		//extract relation descriptor
        		j = sents.get(i).indexOf(" ", 1);
        		if(j != -1)
        		{
        			rel = sents.get(i).substring(0, j);
        		}
        		
        		if(rel.contains("="))
        		{
        			tempi = rel.indexOf("=");
        			rel = rel.substring(0, tempi);
        		}
        		
        		//search for each instance of [[
        		while((j = sents.get(i).indexOf("[[",j)) != -1)
        		{
        			//find index of next ]] instance and extract data inbetween
         			k = sents.get(i).indexOf("]]",j);
         			arg2 = sents.get(i).substring(j+2, k);
         			j = k;
         			
         			if((k = arg2.indexOf("|")) != -1)
         			{
         				arg2 = arg2.substring(0, k);
         			}
         			
         			//create new relation entry
         			temp_rels.add(entity_name2.trim());
         			temp_rels.add(rel.trim());
         			temp_rels.add(arg2.trim());
            		rels.add((LinkedList<String>)temp_rels.clone());
            		temp_rels.clear();
        		}
        	}
        	
        	boolean repeat = false;
        	
        	//if we are in the first recursive level, get data for arg2 nodes
        	if(recursive)
        	{
        		//loop through all arg2 nodes
        		for(int i = 0; i < rels.size(); i++)
        		{
        			repeat = false;
    				tempString = rels.get(i).get(2);
    				
    				//test whether current entity has already been recursively search for
    				for(int l = 0; l < rels.size(); l++)
    				{
    					if(rels.get(i).get(0).equals(tempString))
    					{
    						repeat = true;
    						break;
    					}
    				}
    				
    				//if so, skip to next iteration
    				if(repeat)
    				{
    					continue;
    				}
    				
    				//test whether arg2 node has a link to external page
    				for(int l = 0; l < page_links.size(); l++)
    				{
    					//if so, recursively call function to extract data of arg2 entity
    					if(page_links.get(l).equals(tempString))
    					{
    						System.err.println("Entering new recursive search for "+tempString);
    						//concatinate linked list of second recursive call with current temporary linked list
    						concat(temp_rels2, getInfoBoxRels(tempString.replaceAll(" ", "%20"), tempString, entity_num, null, false));
    						break;
    					}
    				}
        		}
        		//concatinate temporary linked list with actual rels linked list
        		concat(rels, temp_rels2);
        	}
        } catch (IOException e) {
            System.err.println("Error in link: " + url);
        }
        return rels;
	}
	
	//arguments: Two linked lists of type string to be concatinated
	//returms: void
	//function to concatinate two linked lists together
	public static void concat(LinkedList<LinkedList<String>> head, LinkedList<LinkedList<String>> tail)
	{
		if(tail == null)
		{
			return;
		}
		
		for(int i = 0; i < tail.size(); i++)
		{
			head.add(tail.get(i));
		}
	}
	
	//arguments: name of entity to retrieve links for
	//returns: Linked List of links within entities wikipedia page
	//function retrieves a list of links within the wikipedia page of the given entity
	public static LinkedList<String> getLinks(String entity_name) throws JSONException, MalformedURLException
	{
		String sourceCode = "", inputLine = "", url, tempData = "";
		String[] pageData = new String[2];
		
		LinkedList<String> tempLinks = new LinkedList<String>();
		
        //wikipedia request for page links
        url = "https://en.wikipedia.org/w/api.php?format=json&action=query&prop=links&pllimit=max&titles=" + entity_name;
        
        try {
            URL ur = new URL(url);
            HttpURLConnection yc = (HttpURLConnection) ur.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(yc.getInputStream()));
            
            while ((inputLine = in.readLine()) != null) {
                sourceCode = sourceCode + "\n" + inputLine;
            }
            in.close();
            
            JSONObject response = new JSONObject(sourceCode);
            JSONObject queryVal = response.getJSONObject("query");
            tempData = queryVal.getJSONObject("pages").toString();
            
            pageData[1] = tempData.substring(2, tempData.indexOf("\":"));
            JSONArray links = queryVal.getJSONObject("pages").getJSONObject(pageData[1]).getJSONArray("links");
            
            for(int j = 0; j < links.length(); j++)
            {
            	JSONObject linkData = links.getJSONObject(j);
            	tempLinks.add(linkData.getString("title"));
            }
            
        } catch (IOException e) {
            System.err.println("Error in link: " + url);
        }
        return tempLinks;
	}
	
	//arguments: name of entity to retrieve links for
	//returns: Linked List of wikipedia pages which link to the current page
	//function retrieves a list of pages which link to the current wikipedia page
	public static LinkedList<String> getLinksHere(String entity_name) throws MalformedURLException, JSONException
	{
		String sourceCode = "", inputLine = "", url, tempData = "";
		String[] pageData = new String[2];
		
		LinkedList<String> tempLinks = new LinkedList<String>();
		
        //wikipedia request for page links
		url = "https://en.wikipedia.org/w/api.php?format=json&action=query&prop=linkshere&lhlimit=max&titles=" + entity_name;
        
        try {
            URL ur = new URL(url);
            HttpURLConnection yc = (HttpURLConnection) ur.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(yc.getInputStream()));
            
            while ((inputLine = in.readLine()) != null) {
                sourceCode = sourceCode + "\n" + inputLine;
            }
            in.close();
            
            JSONObject response = new JSONObject(sourceCode);
            JSONObject queryVal = response.getJSONObject("query");
            tempData = queryVal.getJSONObject("pages").toString();
            
            pageData[1] = tempData.substring(2, tempData.indexOf("\":"));
            JSONArray links = queryVal.getJSONObject("pages").getJSONObject(pageData[1]).getJSONArray("linkshere");
            
            for(int j = 0; j < links.length(); j++)
            {
            	JSONObject linkData = links.getJSONObject(j);
            	tempLinks.add(linkData.getString("title"));
            }
            
        } catch (IOException e) {
            System.err.println("Error in link: " + url);
        }
        return tempLinks;
	}
}
