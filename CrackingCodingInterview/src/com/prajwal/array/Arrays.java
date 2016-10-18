package com.prajwal.array;

import java.util.Vector;

public class Arrays {

	/*
	 * Generate all the substrings of the string
	 * 
	 * "hello" ==> h,e,l,l,o,he,hel,hell,hello,el,ell,ello,ll,llo,lo
	 */
	static void subString(String str) {
		for (int i = 0; i < str.length(); i++) {
			for (int j = i; j < str.length(); j++) {
				for (int k = i; k <= j; k++) {
					System.out.print(str.charAt(k));
				}
				System.out.println();
			}
		}
	}

	static void subSequence(String str) {
		int no = (int) Math.pow(2, str.length());
		for (int i = 1; i < no; i++) {
			for (int j = 0; j < str.length(); j++) {
				int val = (i & (1 << j));
				if (val != 0)
					System.out.print(str.charAt(j));
			}
			System.out.print(" , ");
		}

	}

	// static Vector<Integer> v = new Vector<Integer>();

	static boolean isSubSet(int[] a, int n, int sum) {
		if (sum == 0) {
			// System.out.println(v);
			// v.clear();
			return true;
		} else if (n == 0 && sum != 0)
			return false;

		if (a[n - 1] > sum)
			isSubSet(a, n - 1, sum);

		// if (isSubSet(a, n - 1, sum))
		// return true;
		// else if (/* v.add(a[n - 1]) & */ isSubSet(a, n - 1, sum - a[n - 1]))
		// {
		// // if (v.size() > 0)
		// // v.removeElementAt(v.size() - 1);
		// return true;
		// }
		// // if (v.size() > 0)
		// // v.removeElementAt(v.size() - 1);
		// return false;
		return isSubSet(a, n - 1, sum) || isSubSet(a, n - 1, sum - a[n - 1]);

	}

	static int count = 0;

	static void countIncreasingSubarray(int[] a, int l, int h) {
		if (h <= l)
			return;
		int m = (l + h) / 2;
		countIncreasingSubarray(a, l, m);
		countIncreasingSubarray(a, m + 1, h);

		merge(a, l, m, h);
	}

	private static void merge(int[] array, int l, int m, int h) {

		int n1 = m - l + 1;
		int n2 = h - m;

		int[] left = new int[n1];
		int[] right = new int[n2];

		for (int i = 0; i < n1; i++) {
			left[i] = array[l + i];
		}

		for (int i = 0; i < n2; i++) {
			right[i] = array[m + 1 + i];
		}

		int i = 0;
		int j = 0;
		int k = l;
		while (i < n1 && j < n2) {
			if (left[i] <= right[j]) {

				array[k] = left[i];
				i++;
				k++;
			} else if (left[i] > right[j]) {
				if (n1 - i == 2)
					count += 2;
				array[k] = right[j];
				j++;
				k++;
			}

		}

		// If i < n1

		while (i < n1) {
			array[k] = left[i];
			i++;
			k++;
		}

		// If j < n2

		while (j < n2) {
			array[k] = right[j];
			j++;
			k++;
		}

	}

	public static void main(String[] args) {
		// subString("hello");
		subSequence("abc");
		int[] a = { 1, 2, 3, 4 };
		//System.out.println(isSubSet(a, a.length, 5));
		//
		// // int a[] = { 8, 4, 2, 1 };
		// countIncreasingSubarray(a, 0, a.length - 1);
		// System.out.println(count);

	}

}
