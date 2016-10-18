package com.prajwal.trees;

import java.util.Vector;

import com.prajwal.trees.BST.node;

public class GoogleTreeQuestions {

	static node prev = null;
	static int min = Integer.MAX_VALUE;

	static boolean findMinValue(node r) {
		if (r == null)
			return true;
		if (findMinValue(r.left)) {
			if (prev != null && r.data < prev.data) {
				return false;
			}
			if (prev != null && (min > (Math.abs(r.data - prev.data))))
				min = (Math.abs(r.data - prev.data));
			prev = r;

			return findMinValue(r.right);
		}
		return false;
	}

	static int maxSum = Integer.MIN_VALUE;

	static class sum {
		int sum = 0;
	}

	static void maxSum(node r, int s, Vector<Integer> v) {
		if (r == null) {
			if (s > maxSum) {
				maxSum = s;
				for (Integer ve : v) {
					System.out.print(ve + " ");
				}
				System.out.println();
			}
			return;
		}

		s += r.data;
		v.add(r.data);
		maxSum(r.left, s, v);

		maxSum(r.right, s, v);
		v.remove(v.size() - 1);
	}

	public static void main(String[] args) {
		BST tree = new BST();
		tree.root = tree.insert(tree.root, 10);
		tree.insert(tree.root, 5);
		tree.insert(tree.root, 16);
		tree.insert(tree.root, 12);
		tree.insert(tree.root, 20);
		tree.insert(tree.root, 25);
		tree.insert(tree.root, 13);
		tree.insert(tree.root, 14);
		tree.insert(tree.root, 27);

		// findMinValue(tree.root);
		// System.out.println(min);
		Vector<Integer> v = new Vector<Integer>();
		maxSum(tree.root, 0, v);
		
		System.out.println(Diameter(tree.root));

	}

	static int Diameter(node r) {
		if (r == null)
			return 0;

		int lheight = height(r.left);
		int rheight = height(r.right);

		int lDiameter = Diameter(r.left);
		int rDiameter = Diameter(r.right);

		return Math.max(lheight + rheight + 1, Math.max(lDiameter, rDiameter));

	}

	static int height(node r) {
		if (r == null)
			return 0;

		return Math.max(height(r.left), height(r.right)) + 1;

	}

}
