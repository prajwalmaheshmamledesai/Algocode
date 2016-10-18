import org.codehaus.jackson.annotate.JsonMethod;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Stack;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.annotate.JsonAutoDetect.Visibility;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;



public class FitBitPrep {

	static public class BST {

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

		void convertToDoubleLL(Node h) {

			Stack<Node> s = new Stack();
			s.push(h);
			while (!s.isEmpty()) {
				Node node = s.peek();

				if (node.l != null) {
					s.push(node.l);
					node.l = null;
					continue;
				}
				s.pop();
				if (head == null) {
					head = node;
					tail = node;
				} else {
					tail.next = node;
					node.prev = tail;
					tail = node;
				}
				if (node.r != null) {
					s.push(node.r);
					node.r = null;
					continue;
				}

			}

			Node temp = head;
			while (temp != null) {
				System.out.println(temp.value + " ");
				temp = temp.next;
			}

		}

		static Node LCA(Node r, int p, int q) {
			if (r == null)
				return null;

			if (r.value == p || r.value == q)
				return r;

			Node left = LCA(r.l, p, q);
			Node right = LCA(r.r, p, q);

			if (left != null && right != null)
				return r;

			return left != null ? left : right;

		}

	}

	public static class DoubleLinkList {
		class DNode {
			int value;
			DNode next;
			DNode prev;

			public DNode(int val) {
				value = val;
				next = null;
				prev = null;
			}
		}

		DNode head = null;
		DNode tail = null;

		void insert(int val) {
			if (head == null) {
				head = new DNode(val);
				tail = head;
			} else {
				DNode node = new DNode(val);
				tail.next = node;
				node.prev = tail;
				tail = node;
			}

		}

	}

	public static class DynamicPrograms {

		static int longestCommonSubsequence(String str1, String str2, int m, int n) {
			if (m <= 0 || n <= 0)
				return 0;
			if (str1.charAt(m - 1) == str2.charAt(n - 1)) {

				return 1 + longestCommonSubsequence(str1, str2, m - 1, n - 1);
			} else
				return Math.max(longestCommonSubsequence(str1, str2, m - 1, n),
						longestCommonSubsequence(str1, str2, m, n - 1));

		}

		static int oneEdit(String str1, String str2, int m, int n) {
			if (m == 0)
				return n;
			else if (n == 0)
				return m;
			else if (str1.charAt(m - 1) == str2.charAt(n - 1)) {
				return oneEdit(str1, str2, m - 1, n - 1);
			} else {
				return 1 + Math.min(oneEdit(str1, str2, m, n - 1),
						Math.min(oneEdit(str1, str2, m - 1, n), oneEdit(str1, str2, m - 1, n - 1)));
			}
		}

	}

	public static class StringPractice {
		static void matchPattern(String[] dic, String pattern) {
			String hash = encode(pattern);

			for (String string : dic) {
				if (hash.contentEquals(encode(string)))
					System.out.println(string);
			}

		}

		private static String encode(String pattern) {
			StringBuffer res = new StringBuffer();
			HashMap<Character, Integer> map = new HashMap<>();

			for (int i = 0; i < pattern.length(); i++) {
				if (map.containsKey(pattern.charAt(i)) == false)
					map.put(pattern.charAt(i), i);

				res.append(map.get(pattern.charAt(i)));
			}

			return res.toString();
		}

		public static void medianTwoSortedArrays(int[] a1, int[] a2, int s1, int e1, int s2, int e2) {
			if (s1 > e1 || s2 > e2)
				return;

			int m1 = (s1 + e1) / 2;
			int m2 = (s2 + e2) / 2;

			if (a1[m1] == a2[m2])
				System.out.println(a1[m1]);

			else if (a1[m1] < a2[m2]) {
				s1 = m1 + 1;
				e2 = m2 - 1;

			} else {
				e1 = m1 - 1;
				s2 = m2 + 1;

			}

			medianTwoSortedArrays(a1, a2, s1, e1, s2, e2);

		}
	}

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		json.setVisibility(JsonMethod.FIELD, Visibility.ANY);
		BST bst = new BST();

		bst.root = bst.insert(bst.root, 5);
		bst.root = bst.insert(bst.root, 3);
		bst.root = bst.insert(bst.root, 1);
		bst.root = bst.insert(bst.root, 4);
		bst.root = bst.insert(bst.root, 7);
		bst.root = bst.insert(bst.root, 6);
		bst.root = bst.insert(bst.root, 9);
		//
		// // bst.head = bst.delete(bst.head, 5);
		// bst.convertToDoubleLL(bst.root);

		System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(BST.LCA(bst.root, 1, 9)));

		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(bst.root));
		// String str1 = "ef";
		// String str2 = "gh";
		// System.out.println(DynamicPrograms.longestCommonSubsequence(str1,
		// str2, str1.length(), str2.length()));
		// System.out.println(DynamicPrograms.oneEdit("abcd", "acdb", 4, 4));

		// String[] dic = { "abb", "abc", "xyz", "xyy" };
		// StringPractice.matchPattern(dic, "foo");

		// int[] a = { 1, 2, 3 };
		// int[] b = { 4, 5, 6 };

	}

}
