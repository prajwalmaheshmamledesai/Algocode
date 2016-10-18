package com.prajwal.trees;

import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Queue;

/**
 * @author prajwalmaheshmamledesai
 *
 */
public class BST {
	public static class node {
		public int data;
		node left;
		node right;

		public node(int val) {
			data = val;
			left = null;
			right = null;
		}

	}

	static node p = null;
	static double inversionCount = 0;
	static int lowNodeCount = 0;

	static boolean inversions(node r) {
		if (r == null)
			return true;
		if (inversions(r.left)) {
			if (p != null && r.data < p.data)
				return false;

			if (p != null) {
				lowNodeCount++;
				inversionCount += lowNodeCount;
			}
			p = r;
			return inversions(r.right);
		}
		return false;

	}

	public static node root = null;

	void creatMinimalBST(int[] a) {
		Arrays.sort(a);

		root = creatMinimalBST(a, 0, a.length - 1);

	}

	static node insert(node r, int val) {
		if (r == null)
			return new node(val);
		else if (r.data <= val)
			r.right = insert(r.right, val);
		else
			r.left = insert(r.left, val);

		return r;

	}

	public node creatMinimalBST(int[] a, int start, int end) {
		if (start > end)
			return null;
		int m = (start + end) / 2;
		node n = new node(a[m]);
		n.left = creatMinimalBST(a, start, m - 1);
		n.right = creatMinimalBST(a, m + 1, end);
		return n;
	}

	int count = 0;

	void inOrder(node r) {
		if (r == null)
			return;
		inOrder(r.left);
		System.out.println(count++ + " " + r.data);
		inOrder(r.right);
	}

	void revInOrder(node r) {
		if (r == null)
			return;
		revInOrder(r.right);
		System.out.print(r.data);
		revInOrder(r.left);
	}

	/*
	 * BST Greater sum tree
	 */

	static int sum = 0;

	void maxSumTree(node r) {
		if (r == null)
			return;
		maxSumTree(r.right);
		int temp = r.data;
		r.data = sum;
		sum += temp;
		maxSumTree(r.left);

	}

	/*
	 * Sum of 2 no
	 */

	static int low = 0;
	static int high = 0;

	int sumPair(node r, int target) {
		if (r == null)
			return 0;
		low = sumPair(r.left, target);
		high = sumPair(r.right, target);
		if (low + high == target)
			System.out.println(low + " " + high);
		else if (low + high > target)
			low = sumPair(r.right, target);
		else
			high = sumPair(r.left, target);
		return r.data;
	}

	static class count {
		public int count = 0;
	}

	static int KthLagestNum = Integer.MAX_VALUE;

	void KthLargest(node r, count c) {
		if (r == null)
			return;
		KthLargest(r.right, c);
		// System.out.print(r.data);
		c.count--;
		if (c.count == 0)
			KthLagestNum = r.data;

		KthLargest(r.left, c);
		return;
	}

	int maxDepth(node r) {
		if (r == null)
			return 0;
		return Math.max(maxDepth(r.left), maxDepth(r.right)) + 1;

	}

	void BFSLevelPrint(node r) {

		Queue<node> q = new LinkedList<node>();

		q.add(r);
		while (true) {
			int n = q.size();
			if (n == 0)
				break;
			for (int i = 0; i < n; i++) {
				node temp = q.remove();
				System.out.print(temp.data + " ");
				if (temp.left != null)
					q.add(temp.left);
				if (temp.right != null)
					q.add(temp.right);
			}
			System.out.println();

		}

	}

	int minDepth(node r) {
		if (r == null)
			return 0;
		if (r.left != null && r.right != null)
			return Math.min(minDepth(r.left), minDepth(r.right)) + 1;
		else if (r.left == null)
			return minDepth(r.right) + 1;
		else
			return minDepth(r.left) + 1;
	}

	boolean isBalanced(node r) {
		if (r == null)
			return true;
		return ((Math.abs(maxDepth(r) - minDepth(r)) <= 1) && isBalanced(r.left) && isBalanced(r.right));
	}

	node prev = null;

	boolean isValid(node r) {

		if (r == null)
			return true;
		if (isValid(r.left)) {
			if (prev != null && prev.data > r.data)
				return false;
			prev = r;
			return isValid(r.right);
		}
		return false;

	}

	void preOrder(node r) {
		if (r == null)
			return;
		System.out.print(r.data);
		preOrder(r.left);
		preOrder(r.right);

	}

	public static void main(String[] args) {

		BST tree = new BST();
		// /int[] a = { 1, 2, 3, 4, 5, 6 };
		int[] a = { 8, 4, 2, 1, 0 };
		tree.creatMinimalBST(a);
		/*
		 * 3 / \ 1 5 / \ / \ 0 2 4 6
		 */
		// tree.insert(tree.root, 0);
		// tree.inOrder(tree.root);
		// System.out.println(tree.maxDepth(tree.root));
		// tree.BFSLevelPrint(tree.root);

		// System.out.println(tree.isBalanced(tree.root));
		// System.out.println(tree.isValid(tree.root))
		// getCountUtil(tree.root, 1, 6);
		// System.out.println(countTree);
		// // tree.revInOrder(tree.root);
		// count c = new count();
		// c.count = 3;
		// tree.KthLargest(tree.root, c);
		// System.out.println(KthLagestNum);

		// findPredSuc(tree.root, 6);
		//
		// System.out.println("pred : "+pred+" succ : "+succ);

		// tree.maxSumTree(tree.root);
		// tree.BFSLevelPrint(tree.root);

		tree.sumPair(tree.root, 10);
	    tree.preOrder(tree.root);
		// inversions(tree.root);
		// System.out.println(inversionCount);
	}

	/*
	 * Predecessor and Successor
	 */
	static int pred = 0;
	static int succ = 0;

	/**
	 * @param r
	 * @param val
	 */
	static void findPredSuc(node r, int val) {
		if (r == null)
			return;
		if (r.data == val) {
			// Find Pred
			if (r.left != null) {
				node temp = r.left;
				while (temp.right != null)
					temp = temp.right;
				pred = temp.data;
			}

			// Find succ
			if (r.right != null) {
				node temp = r.right;
				while (temp.left != null)
					temp = temp.left;
				succ = temp.data;
			}
			return;
		}

		if (r.data > val) {
			succ = r.data;
			findPredSuc(r.left, val);
		} else {
			pred = r.data;
			findPredSuc(r.right, val);
		}
	}

	static int countTree = 0;

	static boolean getCountUtil(node r, int start, int end) {
		if (r == null)
			return true;

		boolean left = r.left != null ? getCountUtil(r.left, start, end) : true;
		;
		boolean righth = r.right != null ? getCountUtil(r.right, start, end) : true;

		if (left && righth & (r.data >= start && r.data <= end)) {
			countTree++;
			return true;
		}
		return false;

	}
}
