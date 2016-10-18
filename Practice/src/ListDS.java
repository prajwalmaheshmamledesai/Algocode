// This is the text editor interface. 
// Anything you type or change here will be seen by the other person in real time.

//insert,mean/median

// 1, 2, 2, 1, 3
//1 1 2 2 
import java.io.IOException;
import java.util.*;
import java.util.Map.Entry;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

class ListDS {

	public static TreeMap<Integer, Integer> map = new TreeMap<>();

	public static int sum = 0;

	public static int size = 0;

	static void insert(int value) {
		if (map.containsKey(value) == true) {
			int temp = map.get(value);
			map.put(value, temp + 1);
		} else
			map.put(value, 1);
		sum += value;
		size++;

	}

	static int mean() {
		return (sum / size);

	}

	static ObjectMapper json = new ObjectMapper();

	// 1, 2, 2, 1, 3
	// 1 1 2 2 6 6 7 7 7 8 8
	static int median() throws JsonGenerationException, JsonMappingException, IOException {
		Set s = map.entrySet();
		System.out.println(json.writeValueAsString(map));
		Iterator i = s.iterator();

		int median = 0;
		for (int j = 0; j < size / 2;) {
			Map.Entry m = (Map.Entry) i.next();

			j += (Integer) m.getValue();
			if (j >= (size / 2)) {

				if (size % 2 == 0) {
					j = j - (Integer) m.getValue();
					median += (Integer) m.getKey();
					if (j < size / 2 - 1) {
						median += (Integer) ((Entry) i.next()).getKey();
						median /= 2;
					}
					return median;
				}

				return j <= size / 2 ? (Integer) ((Entry) i.next()).getKey() : (Integer) m.getKey();

			}
			// i.next();

		}
		return 0;
	}

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		insert(1);
		insert(1);
		insert(2);
		insert(2);
		insert(6);
		insert(6);
		insert(7);
		insert(7);
		insert(7);

		insert(8);
		insert(8);
		insert(9);
		insert(9);

		System.out.println(mean());
		System.out.println(median());

	}

}