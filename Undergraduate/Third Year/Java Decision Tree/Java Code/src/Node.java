import java.util.*;

public class Node {
	private String name;
	private LinkedList<Node> children;
	private LinkedList<String> branchLabels;
	
	public Node(String a){
		name = a;
		children = new LinkedList<Node>();
		branchLabels = new LinkedList<String>();
	}
	
	public String getName(){
		return name;
	}
	
	public void setName(String n){
		this.name = n;
	}
	
	public LinkedList<Node> getChildren(){
		return children;
	}
	
	public LinkedList<String> getBranches(){
		return branchLabels;
	}
	
	public void addChild(Node child, String branchName){
		this.children.add(child);
		this.branchLabels.add(branchName);
	}
	
	public void addBranch(String branchName)
	{
		this.branchLabels.add(branchName);
	}
	
	public Boolean deleteChildren(){
		this.children.clear();
		
		if(this.children.size() != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	public Boolean deleteBranches(){
		this.branchLabels.clear();
		
		if(this.branchLabels.size() != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}
