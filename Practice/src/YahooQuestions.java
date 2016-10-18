
public class YahooQuestions {

	public static class Date {
		public int day;
		public int month;
		public int year;

		Date(int day, int month, int year) {
			this.day = day;
			this.month = month;
			this.year = year;

		}

	}

	public static int[] month = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	static int daysDifference(Date d1, Date d2) {

		int noOfDaysYear = (d2.year - d1.year) * 365;

		int noOfDaysMonths1 = 0;
		for (int i = 0; i < d1.month; i++) {
			noOfDaysMonths1 += month[i];
		}
		int noOfDaysMonths2 = 0;
		for (int i = 0; i < d2.month; i++) {
			noOfDaysMonths2 += month[i];
		}

		int noOfDaysMonths = noOfDaysMonths2 - noOfDaysMonths1;

		int days = d2.day - d1.day;

		return noOfDaysYear + noOfDaysMonths + days;

	}

	public static void main(String[] args) {
		System.out.println(daysDifference(new Date(11, 9, 2016), new Date(1, 10, 2016)));
	}

}
