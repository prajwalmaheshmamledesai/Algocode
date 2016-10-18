package com.prajwal.dp;

public class GoogleDp {

	static int MaxSeq(int[] a) {
		int res[] = new int[a.length];
		res[0] = a[0];
		int max = Integer.MIN_VALUE;
		for (int i = 1; i < a.length; i++) {
			res[i] = Math.max(res[i - 1] + a[i], a[i]);
			if (max < res[i])
				max = res[i];
		}

		return max;
	}

	public static void main(String[] args) {
		int[] a = { -2, 5, -1, 7, -3 };
		System.out.println(MaxSeq(a));
	}

}
