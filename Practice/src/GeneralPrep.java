import java.io.IOException;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

public class GeneralPrep {

	public static int medianRotatedArray(int[] a, int l, int h, int target) {
		if (l > h)
			return -1;

		int m = (l + h) / 2;

		if (a[m] == target)
			return m;

		if (a[l] <= a[m]) {
			if (target >= a[l] && target < a[m])
				return medianRotatedArray(a, l, m - 1, target);

			return medianRotatedArray(a, m + 1, h, target);
		}

		if (target > a[m] && target <= a[h])
			return medianRotatedArray(a, m + 1, h, target);

		return medianRotatedArray(a, l, m - 1, target);
	}

	public static void swap(final char[] c, final int pos1, final int pos2) {
		char temp = c[pos1];
		c[pos1] = c[pos2];
		c[pos2] = temp;

	}

	static String reverseWord(String word) {
		char[] c = word.toCharArray();
		int i = 0;
		int j = c.length - 1;

		while (i <= j) {
			swap(c, i, j);
			i++;
			j--;
		}
		return new String(c, 0, c.length);

	}

	static String reverseLine(String line) {
		String reversedLine = reverseWord(line);
		int prev = 0;
		StringBuffer result = new StringBuffer();
		for (int i = 0; i < reversedLine.length(); i++) {
			if (reversedLine.charAt(i) == ' ') {
				String subString = reversedLine.substring(prev, i);
				subString = reverseWord(subString);
				result.append(subString);
				result.append(" ");
				prev = i;
			}
		}
		String subString = reversedLine.substring(prev, reversedLine.length());
		subString = reverseWord(subString);
		result.append(subString);

		return result.toString();
	}

	static void perms(char[] c, int k, int n) {

		if (k == n) {
			System.out.println(c);

		} else {
			for (int i = k; i < n; i++) {
				swap(c, i, k);
				perms(c, k + 1, n);
				swap(c, i, k);
			}
		}

	}

	static void printAllKLength(char[] c, String prefix, int n, int k) {
		if (k == 0)
			System.out.println(prefix);
		else {
			for (int i = 0; i < n; i++) {
				String newPrefix = prefix + c[i];
				printAllKLength(c, newPrefix, n, k - 1);

			}
		}
	}

	static Boolean isUnique(String str) {

		int checker = 0;

		for (int i = 0; i < str.length(); i++) {
			if ((checker & (1 << str.charAt(i) - 'a')) > 0)
				return false;
			checker |= (1 << str.charAt(i) - 'a');

		}
		return true;

	}

	static int[][] matrixRotation(int[][] m, int n) {
		int[][] a = new int[n][n];
		for (int i = 0; i < n; i++) {
			for (int j = n - 1; j >= 0; j--) {
				a[i][n - j - 1] = m[j][i];
			}
		}

		return a;

	}

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		// int a[] = { 7, 8, 9, 10, 1, 2, 3, 4, 5, 6 };
		//
		// System.out.println(medianRotatedArray(a, 0, a.length - 1, 7));

		// System.out.println(json.writeValueAsString(reverseWord("hello")));

		// System.out.println(reverseLine("india love I"));
		char[] c = { 'a', 'b', 'c' };
		// perms(c, 0, 2);

		 printAllKLength(c, "", 3, 3);
		// System.out.println(isUnique("abzsdfgs"));

		int[][] m = { { 1, 2, 3 },
				      { 4, 5, 6 }, 
				      { 7, 8, 9 } };

		//System.out.println(json.writeValueAsString(matrixRotation(m, 3)));
	}

}
