import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.LinkedList;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;



public class Graph {

	static int nVertex = 0;

	static LinkedList<Integer>[] adjList;

	public Graph(int nVertex) {
		this.nVertex = nVertex;
		adjList = new LinkedList[nVertex];

		for (int i = 0; i < adjList.length; i++) {
			adjList[i] = new LinkedList<>();
		}
	}

	static void addEdge(int v, int u) {
		adjList[v].add(u);
	}

	static int time = -1;

	static void dfsUtil() throws JsonGenerationException, JsonMappingException, IOException {
		Boolean[] visited = new Boolean[nVertex];
		int[] toplogicalSort = new int[nVertex];
		for (int i = 0; i < visited.length; i++) {
			toplogicalSort[i] = -1;
			visited[i] = false;
		}

		for (int i = 0; i < nVertex; i++) {
			if (visited[i] == false) {
				DFS(visited, i, toplogicalSort);
			}
		}
		System.out.println("Topological sort : " + json.writeValueAsString(toplogicalSort));
	}

	private static void DFS(final Boolean[] visited, int v, int[] toplogicalSort)
			throws JsonGenerationException, JsonMappingException, IOException {
		visited[v] = true;
		System.out.println(v);
		LinkedList<Integer> list = adjList[v];
		toplogicalSort[++time] = v;
		for (Integer u : list) {
			if (visited[u] == false) {
				DFS(visited, u, toplogicalSort);
			}
		}

	}

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		Graph g = new Graph(6);
		g.addEdge(5, 2);
		g.addEdge(5, 0);
		g.addEdge(4, 0);
		g.addEdge(4, 1);
		g.addEdge(2, 3);
		g.addEdge(3, 1);

		dfsUtil();

	}

}
