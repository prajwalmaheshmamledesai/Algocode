import org.codehaus.jackson.annotate.JsonMethod;
import org.codehaus.jackson.annotate.JsonAutoDetect.Visibility;
import org.codehaus.jackson.map.ObjectMapper;

public class ShopKick {

	static public void swap(final char[] arr, final int pos1, final int pos2) {
		char t = arr[pos1];
		arr[pos1] = arr[pos2];
		arr[pos2] = t;
	}

	static void perm(char[] c, int pos, int len) {
		if (pos == len) {
			System.out.println(c);
		} else {
			for (int i = pos; i < c.length; i++) {
				swap(c, pos, i);
				perm(c, pos + 1, len);
				swap(c, pos, i);
			}
		}
	}

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) {
		json.setVisibility(JsonMethod.FIELD, Visibility.ANY);
		char[] c = { 'a', 'b', 'c' };
		perm(c, 0, c.length);
	}

}
