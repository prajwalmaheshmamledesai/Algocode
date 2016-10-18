package leeteCode;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class graphPractice1 {
	
	static int V = 0;
	static LinkedList<Integer> [] adj ;
	
	public graphPractice1(int v) {
		V = v;
		adj = new LinkedList[v];
		for (int i = 0; i < v; i++) {
			adj[i] = new LinkedList<Integer>();
		}
	}
	
	void addEdge (int u ,int v){
		adj[u].add(v);
	}
	
	static void DFSUtils(int v , boolean [] visited){
		visited[v] = true;
		System.out.print(v);
		
		LinkedList<Integer> edges  = adj[v];
		for (Integer e : edges) {
			if (visited[e] == false)
				DFSUtils(e, visited);
		}
		
	}
	
	static void DFS(int ver){
		boolean [] visited = new boolean[V];
		for (int i = 0; i < visited.length; i++) {
			visited[i] = false;
		}
		DFSUtils(ver, visited);
		
	}
	
	public static void main(String[] args) {
		graphPractice1 g = new graphPractice1(6);
		g.addEdge(0, 1);
        g.addEdge(0, 2);
        g.addEdge(1, 2);
        g.addEdge(2, 0);
        g.addEdge(2, 3);
        g.addEdge(3, 3);
 
        System.out.println("Following is Depth First Traversal "+
                           "(starting from vertex 2)");
 
        g.DFS(2);	}

}
