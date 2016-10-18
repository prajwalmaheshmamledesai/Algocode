package com.prajwal.graph;

import java.util.LinkedList;
import java.util.Vector;

public class Graph {

	public static class node {
		public int data;
		node[] children;

	}

	int nVertev = 0;
	LinkedList<Integer>[] edges = null;

	public Graph(int nVertex) {
		this.nVertev = nVertex;

		edges = new LinkedList[nVertex];
		for (int i = 0; i < nVertex; i++) {
			edges[i] = new LinkedList<Integer>();
		}

	}

	void addEdge(int u, int v) {
		edges[u].add(v);
	}

	void DFS(int s) {

		boolean[] visited = new boolean[nVertev];
		for (int i = 0; i < visited.length; i++) {
			visited[i] = false;
		}
		DFSUtil(s, visited);

	}

	Vector<Integer> vistedNodes = new Vector<Integer>();

	void DFSUtil(int s, boolean[] visited) {
		System.out.println(s);
		visited[s] = true;
		vistedNodes.addElement(s);
		LinkedList<Integer> adjList = edges[s];
		for (Integer e : adjList) {
			if (!visited[e]) {
				DFSUtil(e, visited);
			}

		}
	}

	boolean Search(int s, int d) {
		DFS(s);
		return vistedNodes.contains(d);
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

		// g.DFS(5);

		System.out.println(g.Search(5, 4));

	}

}
