package leeteCode;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

public class ArrayStrings {

	public static int[] twoSum(int[] nums, int target) {

		HashMap<Integer, Integer> map = new HashMap<Integer, Integer>();

		// map.put(nums[0], 0);

		for (int i = 0; i < nums.length; i++) {
			int x = nums[i];
			if (map.containsKey(target - x))
				return new int[] { map.get(target - x) + 1, i + 1 };

			map.put(x, i);
		}

		return null;

	}

	static void mergeSort(int[] array, int l, int h) {

		if (h <= l)
			return;
		int m = (l + h) / 2;

		mergeSort(array, l, m);
		mergeSort(array, m + 1, h);
		merge(array, l, m, h);

	}

	private static void merge(int[] array, int l, int m, int h) {

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
			if (left[i] <= right[j]) {
				array[k] = left[i];
				i++;
				k++;
			} else if (left[i] > right[j]) {
				array[k] = right[j];
				j++;
				k++;
			}

		}

		// If i < n1

		while (i < n1) {
			array[k] = left[i];
			i++;
			k++;
		}

		// If j < n2

		while (j < n2) {
			array[k] = right[j];
			j++;
			k++;
		}

	}

	static int bSearch(int[] array, int target) {

		int l = 0;
		int h = array.length - 1;
		if (h <= l)
			return -1;

		while (l < h) {
			int m = (l + h) / 2;

			if (array[m] < target) {
				l = m + 1;
			} else {
				h = m;
			}

		}

		return (array[l] == target && l == h) ? l : -1;

	}

	static boolean isChar(char c) {
		if (((int) c >= (int) 'a' && (int) c <= (int) 'z') || ((int) c >= (int) 'A' && (int) c <= (int) 'Z')
				|| ((int) c >= (int) '0' && (int) c <= (int) '9')) {
			return true;
		}
		return false;
	}

	public static boolean isPalindrome(String s) {
		// char[] c = s.toCharArray();

		int j = s.length() - 1;
		int i = 0;
		while (i < j) {
			while ((i < j) && isChar(s.charAt(i)) == false)
				i++;
			while ((i < j) && isChar(s.charAt(j)) == false)
				j--;

			if ((i < j) && Character.toLowerCase(s.charAt(i)) != Character.toLowerCase(s.charAt(j))) {
				return false;

			} else {
				System.out.println(Character.toLowerCase(s.charAt(i)) + " " + Character.toLowerCase(s.charAt(j)));
				i++;
				j--;

			}
		}
		for (int k = 0; k < 4001; k++) {
			LList[k] = new LinkedList<Integer>();
		}
		return true;

	}

	public static int strStr(String haystack, String needle) {

		if (needle.length() > haystack.length())
			return -1;

		if (needle.length() == haystack.length())
			return needle.compareTo(haystack) == 0 ? 0 : -1;

		if (needle.length() == 0)
			return 0;

		for (int i = 0; i < haystack.length(); i++) {
			if (haystack.charAt(i) == needle.charAt(0)) {
				int j = 0;
				for (j = 0; j < needle.length() && (i + j) < haystack.length(); j++) {
					if (haystack.charAt(i + j) != needle.charAt(j))
						break;
				}
				if (j == needle.length())
					return i;
			}
		}

		return -1;

	}

	public static String reverseWordss(String s) {
		int i = 0;
		for (i = 0; i < s.length(); i++) {
			if (s.charAt(i) != ' ')
				break;
		}
		if (i == s.length())
			return "";
		if (s.length() == 0 || s.length() == 1) {
			return s;
		}
		s = s.trim();
		char[] c = s.toCharArray();
		reverse(c, 0, c.length - 1);
		int start = 0;
		for (int end = 0; end < c.length; ++end) {

			if (c[end] == ' ') {
				int count = 0;
				while (c[end + 1] == ' ')
					count++;
				System.out.println("Start:" + start + " end:" + end);
				reverse(c, start, end - 1);
				end += count;
				start = end + 1;
			}

		}

		reverse(c, start, c.length - 1);

		return new String(c).trim();

	}

	private static void reverse(char[] c, int start, int end) {
		System.out.println(" ---> Start:" + start + " end:" + end + "run:" + (end - start) / 2);
		int run = (end - start) / 2;
		if (start > end)
			return;
		for (int i = 0; i < run + 1; i++) {

			char temp = c[start];
			if (c[start] == ' ')
				temp = ' ';
			c[start] = c[end];
			System.out.println(temp + " " + c[start] + " " + i);
			c[end] = temp;
			start++;
			end--;
		}

	}

	public static void main(String[] args) {
		// int a[] = { 15, 11, 7, 2 };
		// mergeSort(a, 0, a.length - 1);
		//
		// for (int i = 0; i < a.length; i++) {
		// System.out.println(a[i]);
		// }
		// System.out.println("Index : " + bSearch(a, 2));
		//
		// int res[] = twoSum(a, 9);
		// for (int i = 0; i < res.length; i++) {
		// System.out.println(res[i]);
		// }
		// System.out.println(isPalindrome("A man, a plan, a canal: Panama"));
		// System.out.println(strStr("aaaa", "aaaa"));
		// System.out.println(reverseWords("the sky is blue"));
		 String str = "123";
//		 char[] c = str.toCharArray();
//		 permutationString(c, 0, 3);
//		//
		// System.out.println(getPermutation(1, 1));

		int[] a = { 5, 7, 5, 8, 22, 66, 23, 6 };
		List<Integer> res = maxMin(a, 0, a.length - 1, Integer.MIN_VALUE, Integer.MAX_VALUE);
		for (Integer integer : res) {
			System.out.println(integer);
		}

	}

	public static String reverseWords(String s) {
		// TODO Auto-generated method stub
		int i = 0;
		for (i = 0; i < s.length(); i++) {
			if (s.charAt(i) != ' ')
				break;
		}
		if (i == s.length())
			return "";
		if (s.length() == 0 || s.length() == 1) {
			return s;
		}
		StringBuffer sb = new StringBuffer();
		String[] l = s.split(" ");
		for (int i1 = l.length - 1; i1 >= 0; i1--) {
			String str = l[i1];
			if (str.compareTo("") == 0)
				continue;
			sb.append(str);
			sb.append(" ");
		}

		return sb.toString().trim();
	}

	static List<String> perms = new ArrayList();

	public static void permutationString(char[] c, int k, int length) {
		if (k == length) {
			String str = "";
			// for (int i = 0; i < c.length; i++) {
			// System.out.print(c[i]);
			// str += c[i];
			// }
			System.out.println(c);
			// perms.add(str);
			// System.out.println();
		} else {
			for (int i = k; i < c.length; i++) {
				// Swap the char
				char t = c[k];
				c[k] = c[i];
				c[i] = t;
				permutationString(c, k + 1, length);
				t = c[k];
				c[k] = c[i];
				c[i] = t;

			}

		}

	}

	public static String getPermutation(int n, int k) {
		String str = "";
		for (int i = 1; i <= n; i++) {
			str += i;
		}
		permutationString(str.toCharArray(), 0, str.length());
		return perms.get(k - 1);

	}

	static LinkedList<Integer>[] LList = new LinkedList[4001];

	public static List<Integer> maxMin(int[] a, int start, int end, int max, int min) {
		ArrayList<Integer> arl = new ArrayList<Integer>();
		if (end > a.length || start < 0) {
		} else if ((end == start))
			max = min = a[start];
		else if (((end - 1) == start)) {
			if (a[start] > a[end]) {
				max = a[start];
				min = a[end];
			} else {
				max = a[end];
				min = a[start];
			}
		} else {
			int mid = (start + end) / 2;

			List<Integer> r1 = maxMin(a, start, mid, max, min);
			List<Integer> r2 = maxMin(a, mid + 1, end, max, min);
			max = (r1.get(0) > r2.get(0)) ? r1.get(0) : r2.get(0);
			min = (r1.get(1) < r2.get(1)) ? r1.get(1) : r2.get(1);
		}
		arl.add(max);
		arl.add(min);
		return arl;
	}

}
