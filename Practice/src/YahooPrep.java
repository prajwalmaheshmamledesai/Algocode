import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Queue;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.annotate.JsonAutoDetect.Visibility;
import org.codehaus.jackson.annotate.JsonMethod;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

import java.util.Set;
import java.util.Stack;
import java.util.concurrent.atomic.AtomicInteger;

import javax.swing.text.DefaultEditorKit.InsertBreakAction;

public class YahooPrep {

	static void firstNonUniqueChar(String string) {
		LinkedHashMap<Character, Integer> map = new LinkedHashMap<>();

		for (int i = 0; i < string.length(); i++) {
			if (map.containsKey(string.charAt(i)) == false)
				map.put(string.charAt(i), 1);
			else {
				int count = map.get(string.charAt(i));
				map.put(string.charAt(i), count + 1);
			}
		}
		Set set = map.entrySet();
		Iterator i = set.iterator();

		while (i.hasNext()) {
			Map.Entry entry = (Map.Entry) i.next();
			if ((Integer) entry.getValue() == 1)
				System.out.println(entry.getKey());
		}
	}

	static Boolean patternMatch(String str, String pattern) {
		char[] patternArray = pattern.toCharArray();
		char[] strArray = str.toCharArray();

		int patternIndex = 0;

		for (int i = 0; i < strArray.length;) {
			if (strArray[i] == patternArray[patternIndex]) {
				++patternIndex;
				++i;
				continue;
			} else if (patternArray[patternIndex] == '.') {
				++i;
				++patternIndex;
				continue;
			} else if (patternArray[patternIndex] == '*') {

				++patternIndex;
				if (patternIndex < patternArray.length) {
					for (; i < strArray.length; i++) {
						if (strArray[i] == patternArray[patternIndex])
							break;
					}
				}
				if (patternIndex >= patternArray.length)
					break;
				continue;
			} else {
				patternIndex = 0;
				i++;
			}

		}
		if (patternIndex >= patternArray.length)
			return true;
		return false;

	}

	public static class TreeNode {
		public int value;
		public TreeNode left;
		public TreeNode right;

		public TreeNode(int value) {
			this.value = value;
			left = null;
			right = null;
		}

	}

	public static TreeNode insert(TreeNode node, int value) {
		if (node == null) {
			return new TreeNode(value);
		} else if (node.value > value)
			node.left = insert(node.left, value);
		else
			node.right = insert(node.right, value);
		return node;

	}

	public static TreeNode delete(TreeNode node, int value) {
		if (node == null)
			return null;
		if (node.value > value)
			node.left = delete(node.left, value);
		else if (node.value < value)
			node.right = delete(node.right, value);
		else {
			if (node.left == null)
				return node.right;
			else if (node.right == null)
				return node.left;

			int next = getNext(node.right);
			node.value = next;
			node.right = delete(node.right, value);

		}
		return node;
	}

	int[][] maze(int r, int c) {
		int[][] a = new int[r][c];
		return a;
	}

	static int[] spiralMatrix(int[][] matrix, int r, int c) {
		int layer = 0;
		int[] result = new int[r * c];
		int count = 0;
		while (layer <= r / 2) {
			for (int i = layer; i < c - layer - 1; i++) {
				result[count++] = matrix[layer][i];
			}
			for (int i = layer; i < r - layer - 1; i++) {
				result[count++] = matrix[i][c - layer - 1];
			}
			for (int i = c - layer - 1; i > layer; i--) {
				result[count++] = matrix[r - layer - 1][i];
			}
			for (int i = r - layer - 1; i > layer; i--) {
				result[count++] = matrix[i][layer];
			}
			layer++;

		}
		return result;
	}

	private static int getNext(TreeNode root) {
		int min = root.value;

		while (root.left != null) {
			min = root.left.value;
			root = root.left;
		}

		return min;
	}

	public static int deepestNode(TreeNode node) {
		Queue<TreeNode> q = new LinkedList<>();
		q.add(node);
		// int depth = 0;
		TreeNode treeNode = null;
		while (q.isEmpty() != true) {
			// ++depth;
			treeNode = q.remove();
			if (treeNode.left != null)
				q.add(treeNode.left);
			if (treeNode.right != null)
				q.add(treeNode.right);
		}
		System.out.println(treeNode.value);

		return treeNode.value;

	}

	static int sum = 0;

	public static void sumTree(TreeNode node) {
		if (node == null)
			return;

		sumTree(node.left);
		sum += node.value;
		node.value = sum;
		sumTree(node.right);

	}

	static void perm(Object[] c, int k) throws JsonGenerationException, JsonMappingException, IOException {

		if (k == c.length) {
			System.out.println(json.writeValueAsString(c));
		} else {
			for (int i = k; i < c.length; i++) {
				swap(c, i, k);
				perm(c, k + 1);
				swap(c, i, k);
			}

		}

	}

	static ArrayList<ArrayList<Integer>> listOLists = new ArrayList<ArrayList<Integer>>();
	static ArrayList<ArrayList<Integer>> result = new ArrayList<ArrayList<Integer>>();

	static void matrixRotation(int a[][], int n) throws JsonGenerationException, JsonMappingException, IOException {
		int[][] res = new int[n][n];

		for (int i = 0; i < n; i++) {
			for (int j = n - 1; j >= 0; j--) {
				res[i][n - j - 1] = a[j][i];
			}
		}

		System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(res));
	}

	public static class LinkList implements Serializable {

		public static class Node {
			int value;
			Node next;

			public Node(int value) {
				this.value = value;
				next = null;
			}
		}

		static Node head = null;

		static void insert(int value) {
			head = insert(head, value);
		}

		static Node insert(Node head, int value) {

			if (head == null)
				head = new Node(value);
			else {
				head.next = insert(head.next, value);
			}
			return head;
		}

	}

	public static <k> void swap(final k[] arr, final int pos1, final int pos2) {
		final k temp = arr[pos1];
		arr[pos1] = arr[pos2];
		arr[pos2] = temp;
	}

	static int partition(Object[] a, int l, int n) {
		int i = l;
		int pivot = (int) a[n];
		for (int j = l; j <= n - 1; j++) {
			if ((int) a[j] <= pivot) {
				swap(a, i, j);
				i++;
			}

		}
		swap(a, i, n);
		return i;
	}

	static int kthSmallest(Object[] a, int k) {
		k = k - 1;
		int low = 0;
		int up = a.length - 1;
		while (true) {
			int j = partition(a, low, up);
			if (j == k)
				return (int) a[j];
			else if (j < k)
				low = j + 1;
			else
				up = j - 1;
		}
	}

	public static void hanoi(int n, String start, String auxiliary, String end) {
		if (n == 1) {
			System.out.println(start + " -> " + end);
		} else {
			hanoi(n - 1, start, end, auxiliary);
			System.out.println(start + " -> " + end);
			hanoi(n - 1, auxiliary, start, end);
		}
	}

	static String movePlane(String str) {
		str = str.toUpperCase();
		StringBuffer temp = new StringBuffer();
		HashMap<Character, Integer> map = new HashMap<>();
		map.put('U', 1);
		map.put('L', -1);
		map.put('D', -1);
		map.put('R', 1);
		int x = 0;
		int y = 0;
		Stack<Integer> xStack = new Stack<>();
		Stack<Integer> yStack = new Stack<>();

		for (int i = 0; i < str.length(); i++) {
			char c = str.charAt(i);
			// System.out.println(( c + " "+ ((int) c);
			if ((int) c >= (int) '0' && (int) c <= (int) '9') {
				temp.append(c);
				continue;
			} else if (c == 'U' || c == 'D' || c == 'L' || c == 'R' || c == 'X') {
				int mul = 1;
				if (temp.length() > 0) {
					mul = Integer.parseInt(temp.toString());
					temp = new StringBuffer();
				}
				if (c == 'U' || c == 'D') {
					xStack.push(x);
					yStack.push(y);
					y += (mul * map.get(c));
				} else if (c == 'L' || c == 'R') {
					xStack.push(x);
					yStack.push(y);
					x += (mul * map.get(c));
				} else {
					if (xStack.isEmpty() || xStack.isEmpty()) {
						x = 0;
						y = 0;

					} else {
						x = xStack.pop();
						y = yStack.pop();

					}
				}
			} else {
				return "(999,999)";

			}
		}
		StringBuffer result = new StringBuffer();
		result.append("(").append(x).append(",").append(y).append(")");
		return result.toString();

	}

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		json.setVisibility(JsonMethod.FIELD, Visibility.ANY);
		// firstNonUniqueChar("abbdcd");

		// System.out.println(patternMatch("abcbcd", "a.*c*d"));

		// TreeNode head = insert(null, 5);
		// insert(head, 3);
		// insert(head, 1);
		// insert(head, 4);
		// insert(head, 8);
		// insert(head, 9);
		// insert(head, 7);
		// insert(head, 0);
		//
		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(head));
		//
		// deepestNode(head);
		// delete(head, 3);
		// System.out.println("====================================================");

		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(head));

		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(head));
		// System.out.println("====================================================");
		//
		// sumTree(head);
		//
		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(head));
		// Object[] c = { 'a', 'b', 'c' };
		// perm(c, 0);

		// ArrayList<Integer> arrayList1 = new ArrayList<>();
		// arrayList1.add(1);
		// arrayList1.add(2);
		// arrayList1.add(3);
		// result.add(arrayList1);
		// listOLists.add(arrayList1);
		//
		// ArrayList<Integer> arrayList2 = new ArrayList<>();
		// arrayList2.add(4);
		// arrayList2.add(5);
		// arrayList2.add(6);
		// result.add(arrayList2);
		// listOLists.add(arrayList2);
		//
		// ArrayList<Integer> arrayList3 = new ArrayList<>();
		// arrayList3.add(7);
		// arrayList3.add(8);
		// arrayList3.add(9);
		// result.add(arrayList3);
		// listOLists.add(arrayList3);
		//
		int a[][] = new int[3][3];
		a[0][0] = 1;
		a[0][1] = 2;
		a[0][2] = 3;

		a[1][0] = 4;
		a[1][1] = 5;
		a[1][2] = 6;

		a[2][0] = 7;
		a[2][1] = 8;
		a[2][2] = 9;
		//
		//// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(a));
		////
		//// matrixRotation(a);

		// LinkList linkList = new LinkList();

		// linkList.insert(1);
		// linkList.insert(2);
		// linkList.insert(3);
		// linkList.insert(4);
		// linkList.insert(5);
		//
		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(linkList.head));

		// hanoi(3, "start", "auxiliary", "end");

		// System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(spiralMatrix(a,
		// 3, 3)));

		System.out.println(movePlane("4DXX2R"));

	}

}
