package com.prajwal.strings;

public class Strings {

	static void onesAndTwos(String str) {
		StringBuffer ones = new StringBuffer();
		for (int i = 0; i < str.length(); i++) {
			ones.append((str.charAt(i) - '0') == 0 ? '1' : '0');
		}

		System.out.println(ones);
		StringBuffer twos = ones;
		int i = 0;
		for (i = ones.length() - 1; i >= 0; i--) {
			if (ones.charAt(i) == '0') {
				twos.replace(i, i + 1, "1");
				break;
			}
			twos.replace(i, i + 1, "0");

		}
		String _2 = twos.toString();
		if (i == -1)
			_2 = '1' + _2;
		System.out.println(_2);
	}

	public static void main(String[] args) {
		onesAndTwos("1100");
	}
}
