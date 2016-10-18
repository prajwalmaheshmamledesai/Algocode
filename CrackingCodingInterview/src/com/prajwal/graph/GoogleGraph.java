package com.prajwal.graph;

import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

public class GoogleGraph {

	static int nVertex = 0;

	static Stack<Integer> s = new Stack<Integer>();

	static void topologicalSort(LinkedList<Integer>[] adj) {
		boolean[] visited = new boolean[nVertex + 1];
		for (int i = 1; i <= nVertex; i++) {
			if (visited[i] == false)
				DFSUtil(adj, i, visited);
		}

	}

	static private void DFSUtil(LinkedList<Integer>[] adj, int vertex,
			boolean[] visited) {
		System.out.println(vertex);
		visited[vertex] = true;

		LinkedList<Integer> edges = adj[vertex];
		for (Integer e : edges) {
			if (visited[e] == false) {
				
				DFSUtil(adj, e, visited);
			}

		}
		s.push(vertex);
	}

	public static void main(String[] args) {
		LinkedList<Integer>[] adj = new LinkedList[6];
		nVertex = 5;
		for (int i = 0; i <= nVertex; i++) {
			adj[i] = new LinkedList<Integer>();
		}
		adj[1].add(2);
		adj[1].add(3);
		adj[1].add(4);
		adj[3].add(5);
		adj[2].add(5);
		adj[4].add(5);

		topologicalSort(adj);

		System.out.print(s);

	}

}
