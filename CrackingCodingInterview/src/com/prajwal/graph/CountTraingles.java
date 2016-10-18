package com.prajwal.graph;

public class CountTraingles {

	static int countTriangle(int[][] g, boolean isDirected) {
		int count = 0;
		for (int i = 0; i < g.length; i++) {
			for (int j = 0; j < g.length; j++) {
				for (int k = 0; k < g.length; k++) {
					if ((g[i][j] == 1) && (g[j][k] == 1) && (g[k][i] == 1)) {
						++count;
					}
				}
			}
		}
		count = isDirected ? count / 6 : count / 3;
		return count;
	}

	public static void main(String[] args) {
		int[][] graph = { { 0, 1, 1, 0 }, { 1, 0, 1, 1 }, { 1, 1, 0, 1 },
				{ 0, 1, 1, 0 } };

		// Create adjacency matrix of a directed graph
		int[][] digraph = { { 0, 0, 1, 0 }, { 1, 0, 0, 1 }, { 0, 1, 0, 0 },
				{ 0, 0, 1, 0 } };

		System.out.println(countTriangle(digraph, true));
	}
}
