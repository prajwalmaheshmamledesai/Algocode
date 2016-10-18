package com.prajwal.graph;

import java.util.ArrayList;
import java.util.List;

public class SamContact {

	static public class contact {
		public contact(String string, String string2, String string3) {
			field1 = string;
			field2 = string2;
			field3 = string3;
		}

		public String field1, field2, field3 = null;

	}

	int[][] graph;

	void buildGraph(contact[] contacts) {
		int n = contacts.length;
		graph = new int[n][n];

		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				if (contacts[i].field1.equals(contacts[j].field1)
						|| contacts[i].field1.equals(contacts[j].field2)
						|| contacts[i].field1.equals(contacts[j].field3)
						|| contacts[i].field2.equals(contacts[j].field1)
						|| contacts[i].field2.equals(contacts[j].field2)
						|| contacts[i].field2.equals(contacts[j].field3)
						|| contacts[i].field3.equals(contacts[j].field1)
						|| contacts[i].field3.equals(contacts[j].field2)
						|| contacts[i].field3.equals(contacts[j].field3)) {
					graph[i][j] = 1;
					break;
				}

			}
		}

	}

	List<Integer> topologicalSort = new ArrayList<Integer>();

	void DFS(int i, Boolean[] visited) {
		visited[i] = true;
		System.out.println("Visited : " + i);
		topologicalSort.add(i);

		for (int j = 0; j < visited.length; j++) {
			if (graph[i][j] == 1 && visited[j] == false)
				DFS(j, visited);
		}
	}

	void findSameContacts(contact[] contacts) {
		int n = contacts.length;
		buildGraph(contacts);

		Boolean[] visited = new Boolean[n];
		for (int i = 0; i < visited.length; i++) {
			visited[i] = false;
		}

		for (int i = 0; i < n; i++) {
			if (visited[i] == false) {
				DFS(i, visited);
				topologicalSort.add(-1);
			}
		}
		for (int i = 0; i < topologicalSort.size(); i++) {
			if (topologicalSort.get(i) == -1)
				System.out.println();
			System.out.print(topologicalSort.get(i) + " ");
		}
	}

	public static void main(String[] args) {
		contact[] arr = new contact[6];
		contact c = new contact("Gaurav", "gaurav@gmail.com",
				"gaurav@gfgQA.com");
		arr[0] = c;
		c = new contact("Lucky", "lucky@gmail.com", "+1234567");
		arr[1] = c;
		c = new contact("gaurav123", "+5412312", "gaurav123@skype.com");
		arr[2] = c;
		c = new contact("gaurav1993", "+5412312", "gaurav@gfgQA.com");
		arr[3] = c;
		c = new contact("raja", "+2231210", "raja@gfg.com");
		arr[4] = c;
		c = new contact("bahubali", "+878312", "raja");
		arr[5] = c;
		
		SamContact s = new SamContact();
		s.findSameContacts(arr);
	}

}
