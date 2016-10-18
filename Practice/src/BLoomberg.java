import java.util.Comparator;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class BLoomberg {

	public static class Person {
		String lastName;
		String firstName;

		public Person(String lastName, String firstName) {
			this.lastName = lastName;
			this.firstName = firstName;
		}

	}

	public static class compare implements Comparator<Person> {

		@Override
		public int compare(Person o1, Person o2) {
			// TODO Auto-generated method stub
			return o1.lastName.compareTo(o2.lastName);
		}

	}

	public static TreeSet<Person> persons = new TreeSet<>(new Comparator<Person>() {

		@Override
		public int compare(Person o1, Person o2) {
			// TODO Auto-generated method stub
			return o1.lastName.compareTo(o2.lastName);
		}
	});

	public static TreeMap<Person, Integer> map = new TreeMap<>(new Comparator<Person>() {

		@Override
		public int compare(Person o1, Person o2) {
			// TODO Auto-generated method stub
			return o1.lastName.compareTo(o2.lastName);
		}
	});

	static void isUnique(String[] firstNames, String[] lastNames) {
		for (int i = 0; i < lastNames.length; i++) {
			Person person = new Person(lastNames[i], firstNames[i]);

			if (persons.contains(person) == false) {
				persons.add(person);
			}
		}
		
		Iterator i = persons.iterator();

		while (i.hasNext()) {
			// Map.Entry me = (Map.Entry) i.next();
			Person p = (Person) i.next();
			System.out.println("lastName: " + p.lastName + " firstName: " + p.firstName);
		}

	}

	public static void main(String[] args) {
		String[] lastNames = { "Zack", "Mamledesai", "Gates", "Jobs", "Manley","Jobs" };
		String[] firstNames = { "Mark", "Prajwal", "Bill", "Steve", "Dawn","Steve" };
		isUnique(firstNames, lastNames);
	}

}
