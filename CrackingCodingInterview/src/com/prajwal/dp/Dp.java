package com.prajwal.dp;

import java.util.Hashtable;
import java.util.Vector;

public class Dp {

	static int OneEdit(String str1, String str2) {

		int m = str1.length();
		int n = str2.length();

		int[][] a = new int[m + 1][n + 1];

		for (int i = 0; i <= m; i++) {
			for (int j = 0; j <= n; j++) {

				if (i == 0)
					a[i][j] = j;

				else if (j == 0)
					a[i][j] = i;
				else if (str1.charAt(i - 1) == str2.charAt(j - 1))
					a[i][j] = a[i - 1][j - 1];
				else
					a[i][j] = 1 + Math.min(a[i][j - 1], Math.min(a[i - 1][j], a[i - 1][j - 1]));

			}

		}
		return a[m][n];

	}

	/*
	 * 
	 * Longest Common Subsequence
	 */

	static int Lis(int[] a) {

		int[] lis = new int[a.length];


		for (int i = 0; i < a.length; i++)
			lis[i] = 1;

		for (int i = 1; i < a.length; i++) {
			for (int j = 0; j < i; j++) {
				if (a[i] > a[j] && lis[i] < lis[j] + 1)
					lis[i] = lis[j] + 1;

			}

		}
		int max = Integer.MIN_VALUE;
		for (int i = 0; i < lis.length; i++) {
			if (max < lis[i])
				max = lis[i];

		}

		return max;
	}

	static int bionomialCoeff(int n, int k) {
		if (k == 0 || k == n)
			return 1;
		System.out.println(n + " " + k);
		return bionomialCoeff(n - 1, k - 1) + bionomialCoeff(n - 1, k);
	}

	/*
	 * 11 2 121 3 1331 4 14641 5 1 5 10 10 5 1 6
	 */

	static void printPascal(int n) {
		int a[][] = new int[n + 1][100];

		for (int i = 1; i < n + 1; i++) {
			for (int j = 0; j < i + 1; j++) {
				if (i == 1) {
					a[i][0] = 1;
					a[i][i] = 1;
				} else if (j == 0 || j == i) {
					a[i][j] = 1;
				} else {
					a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
				}

			}
		}
		for (int i = 1; i < n + 1; i++) {
			for (int j = 0; j < 100; j++) {
				System.out.print(a[i][j] + " ");
			}
			System.out.println();
		}

	}

	public static void main(String[] args) {
		// System.out.println(OneEdit("gfg", "gf"));
		int arr[] = { 10, 22, 9, 10, 33, 21, 50, 29, 30 };
		//System.out.println(Lis(arr));
		// System.out.println(bionomialCoeff(2, 2));
	 printPascal(3);
	}

}
