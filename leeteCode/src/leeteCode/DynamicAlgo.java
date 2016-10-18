package leeteCode;

import java.lang.ref.Reference;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class DynamicAlgo {

	static void LCS(String str1, String str2) {
		int maxLength = 0;
		int[][] a = new int[(str1.length() + 1)][(str2.length() + 1)];

		for (int i = 0; i < str1.length(); i++) {
			for (int j = 0; j < str2.length(); j++) {

				if (i == 0 || j == 0) {
					a[i][j] = 0;
				}

				else if (str1.charAt(i) == str2.charAt(j)) {
					a[i][j] = 1 + a[i - 1][j - 1];
				} else {
					a[i][j] = Math.max(a[i - 1][j], a[i][j - 1]);
				}

			}
		}

		for (int i = 0; i <= str1.length(); i++) {
			for (int j = 0; j <= str2.length(); j++) {
				System.out.print(a[i][j] + " ");
			}
			System.out.print("\n");
		}

	}

	public static int MaxSubSeqSum(int[] a) {

		List<Integer> res = new ArrayList<Integer>();
		res.add(a[0]);
		int max = a[0];
		for (int i = 1; i < a.length; i++) {
			int temp = Math.max(res.get(i - 1) + a[i], a[i]);
			res.add(temp);
			if (temp > max)
				max = temp;
		}
		String str;
		// str.comp

		return max;

	}

	static String res = "";

	// static String buildLowestNumberRec(String str, int n,Reference<String>
	// str1)
	// {
	// // If there are 0 characters to remove from str,
	// // append everything to result
	// if (n == 0)
	// {
	// res += (str);
	// //System.out.println(res);
	// return res;
	// }
	//
	// int len = str.length();
	//
	// // If there are more characters to remove than string
	// // length, then append nothing to result
	// if (len <= n)
	// return "";
	//
	// // Find the smallest character among first (n+1) characters
	// // of str.
	// int minIndex = 0;
	// for (int i = 1; i<=n ; i++)
	// if (str.charAt(i) < str.charAt(i))
	// minIndex = i;
	//
	// // Append the smallest character to result
	// String prev = res;
	// res+=(str.charAt(minIndex));
	//
	// // substring starting from minIndex+1 to str.length() - 1.
	// String new_str = str.substring(minIndex+1, len-minIndex);
	//
	// // Recur for the above substring and n equals to n-minIndex
	// buildLowestNumberRec(new_str, n-minIndex);
	// System.out.println(res + " "+prev);
	//
	// return res;
	// }
	//
	// // A wrapper over buildLowestNumberRec()
	// static String buildLowestNumber(String str, int n)
	// {
	// String res = "";
	//
	// // Note that result is passed by reference
	// buildLowestNumberRec(str, n);
	//
	// return res;
	// }

	static int oneEdit(String str1, String str2) {

		int m = str1.length();
		int n = str2.length();

		int dp[][] = new int[m + 1][n + 1];

		for (int i = 0; i <= m; i++) {
			for (int j = 0; j <= n; j++) {
				if (i == 0)
					dp[i][j] = j;
				else if (j == 0)
					dp[i][j] = i;

				else if (str1.charAt(i - 1) == str2.charAt(j - 1))
					dp[i][j] = dp[i - 1][j - 1];

				else
					dp[i][j] = 1 + min(dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1]);

			}

		}

		for (int i = 0; i <= str1.length(); i++) {
			for (int j = 0; j <= str2.length(); j++) {
				System.out.print(dp[i][j] + " ");
			}
			System.out.print("\n");
		}

		return dp[str1.length() - 1][str2.length() - 1];

	}

	static int oneEdit1(String str1, String str2) {

		int m = str1.length();
		int n = str2.length();
		int dp[][] = new int[m + 1][n + 1];

		// Fill d[][] in bottom up manner
		for (int i = 0; i <= m; i++) {
			for (int j = 0; j <= n; j++) {
				// If first string is empty, only option is to
				// isnert all characters of second string
				if (i == 0)
					dp[i][j] = j; // Min. operations = j

				// If second string is empty, only option is to
				// remove all characters of second string
				else if (j == 0)
					dp[i][j] = i; // Min. operations = i

				// If last characters are same, ignore last char
				// and recur for remaining string
				else if (str1.charAt(i - 1) == str2.charAt(j - 1))
					dp[i][j] = dp[i - 1][j - 1];

				// If last character are different, consider all
				// possibilities and find minimum
				else
					dp[i][j] = 1 + min(dp[i][j - 1], // Insert
							dp[i - 1][j], // Remove
							dp[i - 1][j - 1]); // Replace
			}
		}
		// for (int i = 0; i <= str1.length(); i++) {
		// for (int j = 0; j <= str2.length(); j++) {
		// System.out.print(dp[i][j] + " ");
		// }
		// System.out.print("\n");
		// }

		return dp[str1.length() - 1][str2.length() - 1];

	}

	static int min(int x, int y, int z) {
		return Math.min(Math.min(x, y), z);
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		// LCS("ABACEB", "ADBAVCEB");
		int[] a = { -2, -3, 4, -1, -2, 1, 5, -3 };
		System.out.println(MaxSubSeqSum(a));
		// String s ="";
		// System.out.println(s);

		// System.out.println(buildLowestNumber("121198", 2));

		// String str1 = "sunday";
		// String str2 = "saturday";
		// // System.out.println(oneEdit("a", "b"));
		// System.out.println(oneEdit1(str1, str2));

		buildLowestNumberRec("121198", 2);
		// System.out.println(result);

		Set<String> dic = new HashSet<String>();
		dic.add("leet");
		dic.add("code");

		System.out.println(wordBreak("leetcode", dic));

		// System.out.println(pascal(4, 1));

	}

	static StringBuffer result = new StringBuffer();

	static void buildLowestNumberRec(String str, int n) {
		if (n == 0) {
			result.append(str);
		}
		if (str.length() <= n)
			return;
		// if (str.length() == n) {
		// result = new StringBuffer(str);
		// return;
		//
		// }
		StringBuilder s = new StringBuilder(str);
		int min = 0;
		for (int i = 1; i <= n; i++) {
			if (s.charAt(i) < s.charAt(min))
				min = i;

		}
		result.append(s.charAt(min));
		// s.deleteCharAt(min);

		buildLowestNumberRec(s.substring(min + 1, s.length()), n - min);

	}

	public static boolean wordBreak(String s, Set<String> wordDict) {
		boolean t[] = new boolean[s.length() + 1];
		t[0] = true;
		for (int i = 0; i < s.length(); i++) {
			if (t[i] == false)
				continue;

			for (String str : wordDict) {
				int len = str.length();
				int end = i + len;
				if (end > s.length())
					continue;

				if (t[end] == true)
					continue;

				if (s.substring(i, end).equals(str))
					t[end] = true;

			}

		}

		return t[s.length()];

	}

	public static List<String> wordBreak2(String s, Set<String> wordDict) {

		return null;

	}

	public static int pascal(int x, int y) {

		int[] currentrow;
		int[] previosrow = { 1 };
		printArray(previosrow);
		for (int i = 2; i <= x; i++) {
			currentrow = new int[i];
			currentrow[0] = 1;
			currentrow[i - 1] = 1;

			for (int j = 0; j < i - 2; j++) {
				currentrow[j + 1] = previosrow[j] + previosrow[j + 1];
			}
			printArray(currentrow);
			previosrow = currentrow;

		}
		return previosrow[y];

	}

	private static void printArray(int[] previosrum) {
		// TODO Auto-generated method stub
		for (int i = 0; i < previosrum.length; i++) {
			System.out.print(previosrum[i] + " ");
		}
		System.out.println();

	}

}
