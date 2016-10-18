package leeteCode;

public class DynamicAlgo2 {

	public static int pow(int x, int y) {
		y = y - 1;
		if (y <= 0)
			return x;

		return x * pow(x, y - 1);
	}

	public static int powIterative(int x, int y) {
		int result = 1;
		for (int i = 0; i < y; i++) {
			result *= x;

		}
		return result+1;
	}

	public static void main(String[] args) {
		// System.out.println(powIterative(3, 2));

		//System.out.println(minDistance("pqra", "axyz"));
		System.out.println(LongestCommonSubString("FBDAZMN", "FBDAMN"));
		
		//System.out.println(countPossibilities(2));
	}

	public static int minDistance(String word1, String word2) {

		int m = word1.length();
		int n = word2.length();

		if (m == 0 || n == 0)
			return Math.abs(m - n);

		int[][] matrix = new int[m + 1][n + 1];

		for (int i = 0; i <= m; i++) {
			for (int j = 0; j <= n; j++) {

				if (i == 0)
					matrix[i][j] = j;

				else if (j == 0)
					matrix[i][j] = i;

				else if (word1.charAt(i - 1) == word2.charAt(j - 1))
					matrix[i][j] = matrix[i - 1][j - 1];

				else
					matrix[i][j] = 1 + Math.min(matrix[i][j - 1],
							Math.min(matrix[i - 1][j], matrix[i - 1][j - 1]));

			}
		}

		return matrix[m][n];

	}

	
	static int result = 0;

	static int LongestCommonSubString(String str1, String str2) {
		int m = str1.length();
		int n = str2.length();

		int a[][] = new int[m + 1][n + 1];

		for (int i = 0; i <= m; i++) {
			for (int j = 0; j <= n; j++) {
				if (i == 0 || j == 0)
					a[i][j] = 0;
				else if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
					a[i][j] = 1 + a[i - 1][j - 1];
					result = Math.max(a[i][j], result);
				} else
					a[i][j] = 0;

			}

		}
		
		return result;

	}
	
	public static int countPossibilities(int n) {
	    if (n == 1 || n == 2) return n;
	    return countPossibilities(n - 1) + countPossibilities(n - 2);
	}
}
