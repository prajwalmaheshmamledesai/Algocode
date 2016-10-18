package leeteCode;

import java.util.Stack;

import leeteCode.tries.trieNode;

public class TreeNode {
	int val;
	TreeNode left;
	TreeNode right;

	TreeNode(int x) {
		val = x;
	}

	public static void inorder(TreeNode root) {
		if (root == null)
			return;
		inorder(root.left);
		System.out.print(root.val + " ");
		inorder(root.right);

	}

	public static void postOder(TreeNode root) {
		Stack<TreeNode> s = new Stack<TreeNode>();
		Stack<TreeNode> o = new Stack<TreeNode>();
		s.push(root);
		while (s.empty() != true) {
			TreeNode current = s.peek();
			o.push(s.pop());
			if (current.left != null)
				s.push(current.left);
			if (current.right != null)
				s.push(current.right);
		}
		while (o.empty() != true)
			System.out.println(o.pop().val);

	}

	TreeNode prev = null;

	public boolean isValidBST(TreeNode root) {
		return inIncreasing(root);

	}

	private boolean inIncreasing(TreeNode root) {
		// TODO Auto-generated method stub
		if (root == null)
			return true;
		if (inIncreasing(root.left)) {
			if (prev != null && root.val <= prev.val)
				return false;
			prev = root;
			return inIncreasing(root.right);
		}
		

		return false;
	}

	public int minDepth(TreeNode root) {
		if (root == null)
			return 0;
		if (root.left != null && root.right != null)
			return Math.min(minDepth(root.left), minDepth(root.right)) + 1;
		else if (root.left == null)
			return minDepth(root.right) + 1;
		else
			return minDepth(root.left) + 1;

	}

	public int maxDepth(TreeNode root) {
		if (root == null)
			return 0;
		return Math.max(maxDepth(root.left), maxDepth(root.right)) + 1;

	}

	public boolean isBalanced(TreeNode root) {
		if (root == null)
			return true;

		return (Math.abs(maxDepth(root.right) - maxDepth(root.left)) <= 1)
				&& isBalanced(root.right) && isBalanced(root.left);

	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		// TreeNode node = new TreeNode(10);
		// node.left = new TreeNode(5);
		// node.right = new TreeNode(15);
		// postOder(node);

		TreeNode node = null;
		node = insert(node, 5);
		node = insert(node, 3);
		node = insert(node, 7);
		node = insert(node, 1);
		node = insert(node, 4);
		node = insert(node, 6);
		node = insert(node, 8);
		inorder(node);
		System.out.println();
		node = delete(node, 3);
		inorder(node);
		System.out.println();

		
		
	}

	static TreeNode insert(TreeNode root, int val) {

		if (root == null) {
			root = new TreeNode(val);
			return root;
		}
		if (root.val > val)
			root.left = TreeNode.insert(root.left, val);
		else
			root.right = TreeNode.insert(root.right, val);
		return root;
	}

	static TreeNode delete(TreeNode root, int val) {
		if (root == null)
			return root;

		if (root.val > val)
			root.left = delete(root.left, val);
		else if (root.val < val)
			root.right = delete(root.right, val);
		else {
			if (root.left == null)
				return root.right;
			else if (root.right == null)
				return root.left;
			else {
				root.val = minVal(root.right);
				root.right = delete(root.right, root.val);

			}
		}

		return root;

	}

	private static int minVal(TreeNode root) {
		int min = root.val;

		while (root.left != null) {
			min = root.left.val;
			root = root.left;
		}

		return min;

	}
}
