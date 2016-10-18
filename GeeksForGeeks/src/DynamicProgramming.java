public class DynamicProgramming {

	static int OneEdit(String s1, String s2) {

		int m = s1.length();
		int n = s2.length();

		int[][] a = new int[m + 1][n + 1];

		for (int i = 0; i <= m; i++) {
			for (int j = 0; j <= n; j++) {

				if (i == 0)
					a[i][j] = j;
				else if (j == 0)
					a[i][j] = i;
				else if (s1.charAt(i - 1) == s2.charAt(j - 1))
					a[i][j] = a[i - 1][j - 1];

				else
					a[i][j] = 1 + Math.min(a[i][j - 1],
							Math.min(a[i - 1][j - 1], a[i - 1][j]));

			}

		}

		return a[m][n];

	}

	public static void main(String[] args) {
		System.out.println(OneEdit("abc", "aac"));
	}

}
