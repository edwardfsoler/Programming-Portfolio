
public class Tree {
	
	public Node root;
	public int size;
	
    public Tree(Node root) {
    	this.root = root;
    	this.size = 0;
    }
    
    public Node getRoot() {
    	return this.root;
    }
    
    public void setRoot(Node root) {
    	this.root = root;
    	return;
    }
    
    
    public void add(Node root, Node newNode)
    {
    	if(newNode.count > root.count)
    	{
    		if(root.right == null)
    		{
    			root.right = newNode;
    			newNode.parent = root;
    			this.size++;
    			return;
    		}
    		//else
    		add(root.right, newNode);
    	}
    	else
    	{
    		if(root.left == null)
    		{
    			root.left = newNode;
    			newNode.parent = root;
    			this.size++;
    			return;
    		}
    		//else
    		add(root.left, newNode);
    	}
    }
}