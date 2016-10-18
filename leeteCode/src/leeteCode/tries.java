package leeteCode;

import java.io.IOException;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;

public class tries {

	static class trieNode {
		Object value;

		boolean iscomplete = false;
		trieNode children[] = new trieNode[256];

		// 9663043596

	}

	trieNode root = new trieNode();

	public tries() {
		// TODO Auto-generated constructor stub
		root.value = 0;
		for (int i = 0; i < root.children.length; i++) {
			root.children[i] = null;
		}

	}

	trieNode getnode() {
		trieNode n = new trieNode();
		n.value = 0;
		for (int i = 0; i < n.children.length; i++) {
			n.children[i] = null;
		}
		return n;

	}

	void insert(String key) {

		trieNode crawl = root;
		for (int i = 0; i < key.length(); i++) {
			int index = key.charAt(i);
			if (crawl.children[index] == null) {
				crawl.children[index] = getnode();
			}
			crawl = crawl.children[index];
		}
		crawl.iscomplete = true;

	}

	boolean search(String key) {
		trieNode crawl = root;
		for (int i = 0; i < key.length(); i++) {
			int index = key.charAt(i);
			if (crawl.children[index] == null) {
				return false;
			}
			crawl = crawl.children[index];
		}
		return crawl.iscomplete;
	}

	static ObjectMapper json = new ObjectMapper();

	public static void main(String[] args) throws JsonGenerationException, JsonMappingException, IOException {
		tries t = new tries();
		t.insert("hi");
		t.insert("hello");
		t.insert("helloworld");
		t.insert("hell");
		//System.out.println(json.writerWithDefaultPrettyPrinter().writeValueAsString(t.root.children));
		System.out.println(t.search("helloworld"));

	}

}
