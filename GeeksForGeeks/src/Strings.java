public class Strings {

	static String ZigZag(String str, int n) {

		String[] s = new String[n];

		int row = 0;
		boolean flag = true;

		for (int i = 0; i < str.length(); i++) {
			if (s[row] == null)
				s[row] = "" + str.substring(i, i + 1);
			else
				s[row] += str.charAt(i);

			if (row == 0)
				flag = true;
			else if (row == n - 1)
				flag = false;

			row = flag ? row + 1 : row - 1;

		}
		String result = "";
		for (String string : s) {
			result += string;
		}

		return result;

	}

	static boolean OneEdit(String s1, String s2) {
		int m = s1.length();
		int n = s2.length();

		int count = 0;
		if (Math.abs(m - n) > 1)
			return false;

		int i = 0;
		int j = 0;
		while (i < m && j < n) {
			if (s1.charAt(i) != s2.charAt(j)) {
				if (count == 1)
					return false;
				if (m > n)
					i++;
				else if (n > m)
					j++;
				else {
					i++;
					j++;
				}
				count++;
			} else {
				j++;
				i++;
			}

		}
		if (i < m || j < n)
			count += 1;

		return count <= 1;

	}

	static void swap(int a, int b) {
		System.out.println("a : " + a + " b : " + b);
		b = a + b;
		a = b - a;
		b = b - a;
		System.out.println("a : " + a + " b : " + b);

	}

	public static void main(String[] args) {
		// System.out.println(ZigZag("ABCDEFGH", 2));

		//System.out.println(OneEdit("gfg", "gfg"));
		swap(132, 98);
	}

}
