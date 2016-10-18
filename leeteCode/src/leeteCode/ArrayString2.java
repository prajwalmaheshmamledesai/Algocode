package leeteCode;

import java.io.IOException;
import java.lang.reflect.Array;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

public class ArrayString2 {

	static void mergeSort(int[] array, int l, int h) {
		if (h <= l)
			return;
		int m = (l + h) / 2;
		mergeSort(array, l, m);
		mergeSort(array, m + 1, h);
		merge(array, l, m, h);
	}

	static void merge(int[] array, int l, int m, int h) {
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
			if (left[i] < right[j]) {
				array[k] = left[i];
				i++;
				k++;
			} else {
				array[k] = right[j];
				j++;
				k++;
			}
		}
		while (i < n1) {
			array[k] = left[i];
			i++;
			k++;
		}
		while (j < n2) {
			array[k] = right[j];
			j++;
			k++;
		}

	}

	static int partition(int[] a, int l, int h) {
		int pivot = a[h];
		int i = l;
		for (int j = l; j <= h - 1; j++) {
			if (a[j] <= pivot) {
				int temp = a[j];
				a[j] = a[i];
				a[i] = temp;
				i++;
			}
		}
		int temp = a[i];
		a[i] = pivot;
		a[h] = temp;
		return i;
	}

	static int kthSmallest(int[] array, int k) {

		k = k - 1;
		int j = 0;
		int low = 0;
		int up = array.length - 1;
		while (true) {
			j = partition(array, low, up);

			if (j == k)
				return array[j];
			else if (j < k) {
				low = j + 1;

			} else
				up = j - 1;
		}

	}

	static ObjectMapper json = new ObjectMapper();

	static void combination(char[] c, int[] a, int k, int n)
			throws JsonGenerationException, JsonMappingException, IOException {
		if (k == n) {
			a[n] = 0;
			print(c, a);
			// System.out.println(json.writeValueAsString(a));
			a[n] = 1;
			// System.out.println(json.writeValueAsString(a));
			print(c, a);
			// return;
		} else {
			a[k] = 0;
			combination(c, a, k + 1, n);
			a[k] = 1;
			combination(c, a, k + 1, n);
		}

	}

	private static void print(char[] c, int[] a) {
		// TODO Auto-generated method stub
		String str = "";
		for (int i = 0; i < a.length; i++) {
			if (a[i] == 1)
				str += c[i];
		}
		System.out.println(str);
	}

	public static void interLeaving(char[] str1, char[] str2, String res, int s1, int s2) {
		if (s1 == str1.length && s2 == str2.length) {
			System.out.println(res);
			//
		}
		if (s1 < str1.length) {
			String prev = res;
			res += str1[s1];
			interLeaving(str1, str2, res, s1 + 1, s2);
			res = prev;

		}
		if (s2 < str2.length) {
			String prev = res;
			res += str2[s2];
			interLeaving(str1, str2, res, s1, s2 + 1);
			res = prev;
		}

	}

	static boolean isUnique(String str) {
		int checker = 0;
		for (int i = 0; i < str.length(); i++) {
			int c = str.charAt(i) - 'a';
			if ((checker & (1 << c)) > 0)
				return false;
			checker |= (1 << c);

		}
		return true;
	}

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		// TODO Auto-generated method stub
		int a[] = { 1, 5, 4, 2, 1 };
		// mergeSort(a, 0, a.length - 1);
		// for (int i : a) {
		// System.out.print(i);
		// }
		// System.out.println(partition(a, 3, 0, a.length - 1));

		// System.out.println(kthSmallest(a, 3));
		// System.out.println();
		char c[] = { 'a', 'b', 'c', 'd' };
		int[] array = new int[c.length];
		combination(c, array, 0, c.length - 1);

		// char c[] = { 'a', 'b', 'c' };
		// char c1[] = { 'd', 'e', 'f' };
		// interLeaving(c, c1, "", 0, 0);

		// System.out.println(isUnique("abckl"));

	}

	static public void matrixRotation(int[][] matrix, int n) {

		for (int layer = 0; layer < n / 2; layer++) {
			int first = layer;
			int last = n - 1 - first;
			for (int i = first; i < last; i++) {
				int offset = i - first;
				int top = matrix[first][i];
				matrix[first][i] = matrix[last - offset][last];

			}

		}

	}

}
