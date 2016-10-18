import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Set;

import org.codehaus.jackson.annotate.JsonMethod;
import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.annotate.JsonAutoDetect.Visibility;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

public class HortonWorksPrep {

	public static class BST {

		static public class Node {
			public int value;

			public Node l;
			public Node r;
			public Node next;
			public Node prev;

			public Node(int val) {
				value = val;
				r = null;
				l = null;
				next = null;
				prev = null;
			}

		}

		Node root = null;
		Node head = null;
		Node tail = null;

		Node insert(Node r, int val) {
			if (r == null)
				r = new Node(val);
			else if (r.value > val)
				r.l = insert(r.l, val);
			else
				r.r = insert(r.r, val);

			return r;

		}

		Node delete(Node r, int val) {
			if (r == null)
				return null;
			else if (r.value > val)
				r.l = delete(r.l, val);
			else if (r.value < val)
				r.r = delete(r, val);

			else {
				if (r.l == null)
					return r.r;
				else if (r.r == null)
					return r.l;

				int next = getNext(r.r);

				r.value = next;
				r.r = delete(r.r, r.value);

			}
			return r;
		}

		public int getNext(Node r) {
			int next = r.value;
			while (r.l != null) {
				next = r.l.value;
				r = r.l;
			}
			return next;
		}

		static void printByLevel(Node r) {

			Queue<Node> q = new LinkedList<>();
			Queue<Node> s = new LinkedList<>();
			q.add(r);
			while (!q.isEmpty()) {
				Node n = q.remove();

				System.out.print(n.value + " ");

				if (n.l != null)
					s.add(n.l);
				if (n.r != null)
					s.add(n.r);

				if (q.size() == 0) {

					System.out.println();
					q = s;
					s = new LinkedList<>();
				}

			}
		}

	}

	static void permString(LinkedList<String>[] grps, int grpNo, int noGrp, String result, String template, int index) {
		if (grpNo == noGrp - 1) {
			LinkedList<String> strings = grps[grpNo];
			Iterator<String> iterator = strings.iterator();
			while (iterator.hasNext()) {
				String string = (String) iterator.next();
				System.out.println(result + string);

			}

		} else {
			for (int i = grpNo; i < noGrp; i++) {
				LinkedList<String> strings = grps[grpNo];
				Iterator<String> iterator = strings.iterator();
				while (iterator.hasNext()) {
					String string = (String) iterator.next();
					int s = template.indexOf('}', index + 1);
					int e = template.indexOf('{', s + 1);
					String temp = template.substring(s + 1, e);
					permString(grps, grpNo + 1, noGrp, result + string + temp, template, e + 1);
				}

			}

		}

	}

	static LinkedList<String>[] returnGroups(String str) {
		int noOfGrp = 0;

		for (int i = 0; i < str.length(); i++) {
			if (str.charAt(i) == '{')
				noOfGrp += 1;
		}
		LinkedList<String>[] grps = new LinkedList[noOfGrp];

		int count = 0;
		String[] strings = str.split(" ");
		for (String string : strings) {
			if (string.contains("{")) {
				String[] strs = string.split("/");
				strs[0] = strs[0].substring(1, strs[0].length());
				strs[strs.length - 1] = strs[strs.length - 1].substring(0, strs[strs.length - 1].length() - 1);
				grps[count] = new LinkedList<>();
				for (String s : strs) {
					grps[count].add(s);
				}
				count++;
			}
		}
		permString(grps, 0, noOfGrp, "", str, 0);
		return grps;
	}

	static int oneEdit(String str1, String str2, int m, int n) {
		if (m == 0)
			return n;
		if (n == 0)
			return m;

		if (str1.charAt(m - 1) == str2.charAt(n - 1))
			return oneEdit(str1, str2, m - 1, n - 1);

		else
			return 1 + Math.min(oneEdit(str1, str2, m - 1, n),
					Math.min(oneEdit(str1, str2, m - 1, n - 1), oneEdit(str1, str2, m, n - 1)));

	}

	static List<String> list = new ArrayList<>();
	static HashSet<String> set;

	static void shortestChain(String start, String end) {
		if (start.compareTo(end) == 0)
			return;

		for (Iterator iterator = set.iterator(); iterator.hasNext();) {
			String string = (String) iterator.next();

			if (oneEdit(start, string, start.length(), end.length()) == 1) {
				set.remove(string);
				list.add(string);
				shortestChain(string, end);
			}
		}
	}

	static void swap(final String[] strings, final int pos1, final int pos2) {
		String temp = strings[pos1];
		strings[pos1] = strings[pos2];
		strings[pos2] = temp;
	}

	static void permString(String[] strings, int k, int n) {

		if (k == n) {
			StringBuffer buffer = new StringBuffer();
			for (String string : strings) {
				buffer.append(string);
			}
			set.add(buffer.toString());
			System.out.println(buffer.toString());

		} else {
			for (int i = k; i < n; i++) {
				swap(strings, i, k);
				permString(strings, k + 1, n);
				swap(strings, i, k);
			}
		}

	}
	
	public static Boolean isPermutate(String [] strings,String str){
		permString(strings, 0, strings.length);
		return set.contains(str);
	}

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		// json.setVisibility(JsonMethod.FIELD, Visibility.ANY);
		// String str = "{Oleg/Prajwal} loves to write code in {c/c++/java} "
		// + "but his boss wants hime to write in {python/go}";
		//
		// System.out.println(returnGroups(str));

		BST bst = new BST();

		bst.root = bst.insert(bst.root, 5);
		bst.root = bst.insert(bst.root, 3);
		bst.root = bst.insert(bst.root, 1);
		bst.root = bst.insert(bst.root, 4);
		bst.root = bst.insert(bst.root, 7);
		bst.root = bst.insert(bst.root, 6);
		bst.root = bst.insert(bst.root, 9);

		// BST.printByLevel(bst.root);

		// System.out.println(oneEdit("abc", "ac", 3, 2));

		String[] arr = {"bad", "actor", "good", "acting"};
		set = new HashSet<String>(Arrays.asList(arr));
		// shortestChain("TOON", "PLEA");

		// System.out.println(json.writeValueAsString(list));

		//permString(arr, 0, arr.length);
		
		System.out.println(isPermutate(arr, "badactorgoodacting"));
		
		

	}

}
