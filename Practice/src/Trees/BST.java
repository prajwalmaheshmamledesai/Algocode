package Trees;

import java.io.IOException;

import org.codehaus.jackson.map.ObjectMapper;

public class BST {

	static ObjectMapper json = new ObjectMapper();

	public static class Node {
		public int value;
		public Node left;
		public Node right;

		public Node(int val) {
			this.value = val;
		}
	}

	Node head = null;

	Node insert(Node root, int val) {
		if (root == null)
			root = new Node(val);

		else if (root.value > val)
			root.left = insert(root.left, val);

		else if (root.value <= val)
			root.right = insert(root.right, val);

		return root;

	}

	Node delete(Node root, int val) {
		if (root == null)
			return null;
		else if (root.value > val)
			root.left = delete(root.left, val);
		else if (root.value < val)
			root.right = delete(root.right, val);
		else {
			if (root.left == null)
				return root.right;
			else if (root.right == null)
				return root.left;

			Node next = getNext(root);
			if (next == null)
				return null;
			root.value = next.value;

			root.left = delete(root.left, root.value);

		}
		return root;
	}

	private Node getNext(Node root) {
		if (root.left == null)
			return root.left;
		root = root.left;
		while (root.right != null) {
			root = root.right;
		}
		return root;
	}

	static int maxDepth(Node root) {
		if (root == null)
			return 0;

		return 1 + Math.max(maxDepth(root.left), maxDepth(root.right));
	}

	static int minDepth(Node root) {
		if (root == null)
			return 0;
		if (root.left == null)
			return minDepth(root.right) + 1;
		if (root.right == null)
			return minDepth(root.left) + 1;
		return 1 + Math.min(minDepth(root.left), minDepth(root.right));
	}

	Node p = null;
	int inversionCount = 0;
	int lowNodeCount = 0;

	Boolean inversion(Node r) {
		if (r == null)
			return true;
		if (inversion(r.left)) {
			if (p != null && r.value < p.value) {
				return false;
			}

			if (p != null) {
				lowNodeCount++;
				inversionCount += lowNodeCount;
			}
			p = r;
			inversion(r.right);
		}
		return false;
	}

	public static void main(String[] args) {
		BST bst = new BST();
		bst.head = bst.insert(null, 5);
		bst.insert(bst.head, 3);
		bst.insert(bst.head, 4);
		bst.insert(bst.head, 7);
		bst.insert(bst.head, 6);
		bst.insert(bst.head, 5);
		try {
			System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(bst.head));
			System.out.println("Deleting ------->");
			bst.delete(bst.head, 10);
			System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(bst.head));
			System.out.println("Max Depth ------->" + maxDepth(bst.head));
			System.out.println("Min Depth ------->" + minDepth(bst.head));

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
