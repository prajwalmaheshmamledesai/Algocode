package com.prajwal.bits;

public class bitManipulation {

	static boolean isUnique(int[] a) {

		int checker = 0;
		for (int i = 0; i < a.length; i++) {
			if ((checker & (1 << a[i])) != 0) {
				return false;

			}
			checker |= (1 << a[i]);
		}
		return true;
	}

	static int appearsOnce(int[] a) {
		int result = 0;
		for (int i = 0; i < a.length; i++) {
			result ^= a[i];
		}
		return result;
	}

	static int add(int x, int y) {
		while (y != 0) {
			int carry = x & y;
			x = x ^ y;
			y = carry << 1;
		}
		return x;
	}

	public static void main(String[] args) {
		// int a[] = { 1, 2, 3, 7, -7, -6 };
		// System.out.println(isUnique(a));
		// int[] arr = { 1, 1, 2, 2, 3, 4, 4 };
		// System.out.println(appearsOnce(arr));
		System.out.println(add(1, 3));
	}

}
