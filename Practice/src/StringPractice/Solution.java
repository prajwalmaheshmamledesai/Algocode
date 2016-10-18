package StringPractice;

public class Solution {

	static String reverseWord(String str) {
		StringBuilder rev = new StringBuilder();
		for (int i = str.length() - 1; i >= 0; i--) {
			rev.append(str.charAt(i));
		}
		return rev.toString();

	}

	static String reverseLine(String line) {
		StringBuilder rev = new StringBuilder();
		int j = line.length();
		for (int i = line.length() - 1; i >= 0; i--) {
			if (line.charAt(i) == ' ')
				j = i;
			else if (i == 0 || line.charAt(i - 1) == ' ') {
				if (rev.length() != 0)
					rev.append(' ');
				rev.append(line.substring(i, j));
			}
		}
		return rev.toString();
	}

	static int atoi(String str) {
		int num = 0;
		for (int i = 0; i < str.length(); i++) {
			num = num * 10 + (str.charAt(i) - '0');
		}
		return num;
	}

	static int oneEdit(String str1, String str2, int m, int n) {
		if (m == 0)
			return n;
		if (n == 0)
			return m;

		if (str1.charAt(m - 1) == str2.charAt(n - 1))
			return oneEdit(str1, str2, m - 1, n - 1);
		return 1 + Math.min(oneEdit(str1, str2, m, n - 1),
				Math.min(oneEdit(str1, str2, m - 1, n), oneEdit(str1, str2, m - 1, n - 1)));

	}

	public static void main(String[] args) {
		// System.out.println(reverseLine("hello india"));
		//System.out.println(atoi("112349"));
		System.out.println(oneEdit("abcd", "aacd", 4, 4));
	}

}
