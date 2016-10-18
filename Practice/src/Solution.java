import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

public class Solution {

	static ObjectMapper json = new ObjectMapper();

	public static int[] twoSum(int[] nums, int target) {
		int res[] = {};
		for (int i = 0; i < nums.length; i++) {
			for (int j = i + 1; j < nums.length; j++) {
				if (nums[i] + nums[j] == target) {
					int res1[] = { i, j };
					return res1;
				}
			}
		}
		return res;
	}

	static HashMap<Integer, Integer> hashMap = new HashMap<>();

	public static int[] twoSum1(int[] nums, int target) {
		for (int i = 0; i < nums.length; i++) {
			Integer key = hashMap.get(target - nums[i]);
			if (key != null) {
				int res1[] = { key, i };
				return res1;
			} else {

				hashMap.put(nums[i], i);
			}
		}
		int res[] = {};
		return res;
	}

	static void permutation(char[] c, int k, int n) {
		if (k == n) {
			try {
				System.out.println(json.writeValueAsString(c));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return;
		}
		for (int i = 0; i < c.length; i++) {
			char ch = c[k];
			c[k] = c[i];
			c[i] = ch;
			permutation(c, k + 1, n);
			ch = c[k];
			c[k] = c[i];
			c[i] = ch;
		}

	}

	static List<String> list = new ArrayList<String>();

	static void subString(String string) {
		for (int i = 0; i < string.length(); i++) {
			for (int j = i; j < string.length(); j++) {
				StringBuffer buffer = new StringBuffer();
				for (int k = i; k <= j; k++) {
					buffer.append(string.charAt(k));
				}
				list.add(buffer.toString());
			}
		}
	}

	int oneEdit(String str1, String str2, int m, int n) {

		int[][] a = new int[m + 1][n + 1];

		for (int i = 0; i <= m; i++) {
			for (int j = 0; j <= n; j++) {

				if (i == 0)
					a[i][j] = j;
				else if (j == 0)
					a[i][j] = i;
				else if (str1.charAt(i) == str2.charAt(j)) {
					a[i][j] = a[i - 1][j - 1];
				} else
					a[i][j] = Math.min(a[i][j - 1], Math.min(a[i - 1][j], a[i - 1][j - 1]));
			}
		}
		return a[m][n];

	}

	static void binarySearch(int[] a, int l, int h, int val) {
		if (l >= h)
			return;
		int m = (l + h) / 2;
		if (val == a[m])
			System.out.println(m);
		else if (val > a[m])
			binarySearch(a, m + 1, h, val);
		else
			binarySearch(a, l, m, val);

	}

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		int test[] = { 2, 1, 9, 4, 4, 56, 90, 3 };
		Arrays.sort(test);
		System.out.println(json.writeValueAsString(test));
		binarySearch(test, 0, test.length - 1, 9);
		//
		// try {
		// System.out.println(json.writeValueAsString(twoSum1(test, 8)));
		// } catch (IOException e) {
		// e.printStackTrace();
		// }
		// char[] c = { 'a', 'b', 'c' };
		// permutation(c, 0, 2);
		// subString("hello");
		// try {
		// System.out.println(json.writeValueAsString(list) + list.size());
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }

	}
}