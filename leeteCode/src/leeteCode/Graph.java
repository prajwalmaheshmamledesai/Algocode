package leeteCode;

import java.util.LinkedList;
import java.util.Stack;

public class Graph {

	static int nVertex = 0;
	static LinkedList<Integer>[] Edges = null;

	static Stack<Integer> topologicalSort = null;

	Graph(int nVertex) {

		this.nVertex = nVertex;
		Edges = new LinkedList[nVertex];

		for (int i = 0; i < Edges.length; i++) {
			Edges[i] = new LinkedList<Integer>();
		}

	}

	void addEdge(int v, int w) {
		Edges[v].add(w);
	}

	static void DFSutil(int v, Boolean[] visited) {
		visited[v] = true;
		System.out.println(" " + v);

		LinkedList<Integer> adjV = Edges[v];
		for (Integer vertex : adjV) {
			if (visited[vertex] == false) {
				DFSutil(vertex, visited);

			}

		}
		topologicalSort.push(v);

	}

	static void DFS(int v) {
		Boolean[] Visited = new Boolean[nVertex];
		for (int i = 0; i < Visited.length; i++) {
			Visited[i] = false;
		}
		// DFSutil(v, Visited);
		topologicalSort = new Stack<Integer>();

		for (int i = 0; i < Visited.length; i++) {
			if (Visited[i] == false)
				DFSutil(i, Visited);
		}
		// topologicalSort.push(0);

	}

	public static void main(String args[]) {
		Graph g = new Graph(6);
		g.addEdge(5, 2);
		g.addEdge(5, 0);
		g.addEdge(4, 0);
		g.addEdge(4, 1);
		g.addEdge(2, 3);
		g.addEdge(3, 1);

		System.out.println("Following is Depth First Traversal "
				+ "(starting from vertex 2)");

		g.DFS(1);

		while (topologicalSort.empty() == false) {
			System.out.print(topologicalSort.pop());
		}
	}

}
