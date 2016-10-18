package Trees;

import java.util.LinkedList;

public class Graph {

	static int nVertex = 0;
	static LinkedList<Integer>[] Edges = null;

	public Graph(int nVertex) {
		this.nVertex = nVertex;

		Edges = new LinkedList[nVertex];
		for (int i = 0; i < Edges.length; i++) {
			Edges[i] = new LinkedList<>();
		}

	}

	void addEdge(int v, int w) {
		Edges[v].add(w);
	}

	static void DFS(int v) {
		Boolean[] visited = new Boolean[nVertex];
		for (int i = 0; i < visited.length; i++) {
			visited[i] = false;
		}

		for (int i = 0; i < visited.length; i++) {
			if (visited[i] == false)
				DFSUtil(i, visited);
		}

	}

	private static void DFSUtil(int i, Boolean[] visited) {
		visited[i] = true;
		System.out.print(i);
		LinkedList<Integer> adjv = Edges[i];
		for (Integer vertex : adjv) {
			if (visited[vertex] == false)
				DFSUtil(vertex, visited);
		}

	}

	public static void main(String[] args) {
		Graph g = new Graph(6);
		g.addEdge(5, 2);
		g.addEdge(5, 0);
		g.addEdge(4, 0);
		g.addEdge(4, 1);
		g.addEdge(2, 3);
		g.addEdge(3, 1);

		System.out.println("Following is Depth First Traversal " + "(starting from vertex 2)");

		g.DFS(1);
	}

}
