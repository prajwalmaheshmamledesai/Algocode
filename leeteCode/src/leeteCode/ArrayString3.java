package leeteCode;

import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Stack;

public class ArrayString3 {
	/**
	 * Count the maximum depth of parenthesis nesting, i.e.
	 * "abc(123(xyz))m(((n)))o" -> 3.
	 *
	 * @param input
	 *            any string
	 * @return deepest parenthesization level
	 * @throws IllegalArgumentException
	 *             if input is null or contains a mismatch "a)b(c" or "a(b"
	 */
	public static int nestedParenthesisDepth(String input)

			throws IllegalArgumentException {

		// Stack<Character> s = new Stack<Character>();
		int max = 0;
		int maximum = 0;
		for (int i = 0; i < input.length(); i++) {
			if (input.charAt(i) == '(') {
				// s.push(input.charAt(i));
				max++;
			} else if (input.charAt(i) == ')') {
				if (max <= 0)
					throw new IllegalArgumentException("wrong");
				if (maximum < max)
					maximum = max;
				max--;

			}
		}
		if (max != 0)
			throw new IllegalArgumentException("wrong");

		return maximum;
		// ...
	}

	public static int minMoves(char[] seats) {
		int n = seats.length;

		// step 1. count people
		int k = 0;
		for (int i = 0; i < n; i++) {
			if (seats[i] == 'X') {
				k++;
			}
		}

		// step 2. move the window of size k and count the hops
		int min = Integer.MAX_VALUE;

		for (int j = 0; j < n - k; j++) {
			int hops = 0, p = j;

			for (int i = 0; i < n; i++) {
				if (seats[i] == 'X') {

					hops += Math.abs(i - p++);
				}
			}

			min = Math.min(min, hops);
		}

		return min;
	}

	static boolean rGTogether(String s, int m) {
		int count = 0;
		int j = 1;
		for (int i = 0; i < s.length(); i++) {
			if (s.charAt(i) == 'g' && s.charAt(j) == 'g')
				count++;
			else
				count = 0;
			j++;
		}
		return count == m;

	}

	static int minimumSwapsRequired(int m, String s) {
		int k = 0;
		s.replaceAll("X", "G");
		for (int i = 0; i < s.length(); i++) {
			if (s.charAt(i) == 'G')
				k++;

		}
		if (m == 0 || rGTogether(s, m) == true)
			return 0;
		int min = Integer.MAX_VALUE;

		for (int i = 0; i < s.length() - k; i++) {
			int hops = 0;
			int p = i;
			int count = 0;
			for (int j = 0; j < s.length(); j++) {
				if (s.charAt(j) == 'G') {

					hops += Math.abs(j - p++);

					if (count++ == m)
						break;
				}

			}

			min = Math.min(min, hops);
		}
		System.out.println(min);
		return min;

	}

	public static boolean canJump(int[] nums) {
		if (nums.length <= 1)
			return true;

		int max = nums[0]; // max stands for the largest index that can be
							// reached.

		for (int i = 0; i < nums.length; i++) {
			// if not enough to go to next
			if (max <= i && nums[i] == 0)
				return false;

			// update max
			if (i + nums[i] > max) {
				max = i + nums[i];
			}

			// max is enough to reach the end
			if (max >= nums.length - 1)
				return true;
		}

		return false;

	}

	private static void reverse(int[] nums, int start, int end) {
		// System.out.println(" ---> Start:" + start + " end:" + end + "run:"
		// + (end - start) / 2);
		int run = (end - start) / 2;
		if (start > end)
			return;
		for (int i = 0; i < run + 1; i++) {

			int temp = nums[start];
			if (nums[start] == ' ')
				temp = ' ';
			nums[start] = nums[end];
			// System.out.println(temp + " " + nums[start] + " " + i);
			nums[end] = temp;
			start++;
			end--;
		}

	}

	public static void rotate(int[] nums, int k) {
		if (k > nums.length)
			k = nums.length;
		reverse(nums, 0, k);
		reverse(nums, k + 1, nums.length - 1);
		reverse(nums, 0, nums.length - 1);

	}

	public static void main(String[] args) {
		// System.out.println(nestedParenthesisDepth("abc(123(xyz))m(((n)))o"));
		// String s = "....X..XX...X..";
		// System.out.println(minMoves(s.toCharArray()));
		// int[] nums = { 3, 4, 3, 2, 1 };
		// System.out.println(canJump(nums));
		// // System.out.println(findKthLargest(nums, 1));
		//
		// char[] c = { '2', '3', '4' };
		// permuatation(c, 0);

		// rotate(nums, 3);h

		 System.out.println(convert("PAYPALISHIRING", 3));
		//minimumSwapsRequired(3, "BGBGBGG");

	}

	static int partition(int[] nums, int l, int h) {
		int pivot = nums[h];
		int i = l;
		for (int j = l; j < h - 1; j++) {
			if (nums[j] >= pivot) {
				int temp = nums[j];
				nums[j] = nums[i];
				nums[i] = temp;
				i++;
			}
		}
		int temp = nums[i];
		nums[i] = pivot;
		nums[h] = temp;
		return i;

	}

	public static int findKthLargest(int[] nums, int k) {

		int low = 0;
		int high = nums.length - 1;
		int j;

		k = k - 1;
		while (true) {
			j = partition(nums, low, high);
			if (j == k)
				return nums[j];
			else if (j < k)
				low = j + 1;
			else if (j > k)
				high = j - 1;
		}

	}

	static void permuatation(char[] array, int k) {

		if (k == array.length) {
			for (int i = 0; i < array.length; i++) {
				System.out.print(array[i]);
			}
			System.out.println();
		}

		for (int i = k; i < array.length; i++) {
			char c = array[i];
			array[i] = array[k];
			array[k] = c;
			permuatation(array, k + 1);
			c = array[i];
			array[i] = array[k];
			array[k] = c;
		}

	}

	public static String convert(String s, int numRows) {

		if (numRows == 1)
			return s;

		if (s.length() < numRows)
			return s;
		String[] str = new String[numRows];

		int row = 0;
		boolean down = true;
		for (int i = 0; i < s.length(); i++) {
			if (str[row] == null)
				str[row] = s.substring(i, i + 1);
			else
				str[row] += s.charAt(i);

			if (row == numRows - 1)
				down = false;

			if (row == 0)
				down = true;

			if (down)
				row++;
			else
				row--;

		}

		StringBuffer res = new StringBuffer();
		for (String string : str) {
			res.append(string);
		}

		return res.toString();

	}
}
