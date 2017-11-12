import java.io.*;
import java.util.*;

public class FlightUpdater {
	public static void main(String []args) throws IOException
	{
		try
		{
			LinkedList<String> ll = new LinkedList();
			FileReader fr = new FileReader("Flight Cache 2.txt");
			PrintWriter pw = new PrintWriter("Flight Arrivals.txt");
			BufferedReader br = new BufferedReader(fr);
			String line = null;
			String source = "";
			
			int tcount = 0;
			int trcount = 0;
			int nexttd = 0;
			int endtd = 0;
			int start = 0;
			
			while((line = br.readLine()) != null)
			{
				if(line.contains("class=\"arrive-depart-table\""))
				{
					tcount++;
					start = line.indexOf("class=\"arrive-depart-table\"");
				}
				
				while((start = line.indexOf("<tr>",start)) != -1 && tcount == 3)
				{
					trcount++;
					start += 4;
					if(trcount != 1)
					{
						while((nexttd = line.indexOf("<td>",start)) != -1)
						{
							nexttd += 4;
							endtd = line.indexOf("</td>",nexttd);
							source = line.substring(nexttd, endtd);
							
							if(source.length() < 50)
							{
								if(source.length() < 1)
								{
									source = "N/A";
								}
								ll.add(source);
							}
							start = endtd;
						}
					}
				}
			}
			
			for(int i =0; i< ll.size(); i += 5)
			{
				pw.println("arrive('"+ll.get(i)+"','"+ll.get(i+1)+"','"+ll.get(i+2)+"','"+ll.get(i+3)+"',"+ll.get(i+4).toLowerCase()+").");
			}
 
			br.close();
			pw.close();
		}
		catch(FileNotFoundException e)
		{
			System.out.println("File not found: "+e);
		}
		
		try
		{
			LinkedList<String> ll = new LinkedList();
			FileReader fr = new FileReader("Flight Cache.txt");
			PrintWriter pw = new PrintWriter("Flight Departures.txt");
			BufferedReader br = new BufferedReader(fr);
			String line = null;
			String source = "";
			
			int tcount = 0;
			int trcount = 0;
			int nexttd = 0;
			int endtd = 0;
			int start = 0;
			
			while((line = br.readLine()) != null)
			{
				if(line.contains("class=\"arrive-depart-table\""))
				{
					tcount++;
					start = line.indexOf("class=\"arrive-depart-table\"");
				}
				
				while((start = line.indexOf("<tr>",start)) != -1 && tcount == 3)
				{
					trcount++;
					start += 4;
					if(trcount != 1)
					{
						while((nexttd = line.indexOf("<td>",start)) != -1)
						{
							nexttd += 4;
							endtd = line.indexOf("</td>",nexttd);
							source = line.substring(nexttd, endtd);
							
							if(source.length() < 50)
							{
								if(source.length() < 1)
								{
									source = "N/A";
								}
								ll.add(source);
							}
							start = endtd;
						}
					}
				}
			}
			
			for(int i =0; i< ll.size(); i += 5)
			{
				pw.println("depart('"+ll.get(i)+"','"+ll.get(i+1)+"','"+ll.get(i+2)+"','"+ll.get(i+3)+"',"+ll.get(i+4).toLowerCase()+").");
			}
 
			br.close();
			pw.close();
		}
		catch(FileNotFoundException e)
		{
			System.out.println("File not found: "+e);
		}
	}
}
