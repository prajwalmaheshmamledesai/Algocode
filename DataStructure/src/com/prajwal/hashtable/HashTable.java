package com.prajwal.hashtable;

import java.util.LinkedList;

public class HashTable<K, V> {

	static class node<V, K> {
		public node(K key, V value) {

			this.Key = key;
			this.Value = value;
		}

		public K Key;
		public V Value;
	}

	LinkedList<node>[] ll = new LinkedList[4001];

	public HashTable() {
		for (int i = 0; i < ll.length; i++) {
			ll[i] = new LinkedList<HashTable.node>();
		}
	}

	void put(K key, V Value) {
		int val = 0;
		if (key == null)
			val = 4000;
		else
			val = key.hashCode();
		int modValue = val % 4001;
		if (get(key) != null) {
			LinkedList<node> tempL = ll[modValue];
			int index = 0;
			for (node n : tempL) {
				if (n.Key.equals(key) == true) {
					n.Value = Value;
				}
				// sindex++;
			}
		} else

			ll[modValue].add(new node<V, K>(key, Value));

	}

	V get(K Key) {

		int val = Key.hashCode();
		int modValue = val % 4001;
		LinkedList<node> tempL = ll[modValue];
		for (node n : tempL) {
			if (n.Key.equals(Key) == true)
				return (V) n.Value;
		}
		return null;

	}

	public static void main(String[] args) {
		HashTable<Integer, String> hMap = new HashTable<Integer, String>();
		hMap.put(1, "Hello");
		hMap.put(1, "Hi");
		hMap.put(2, "How");

		System.out.println(hMap.get(1) + " " + hMap.get(2));

		HashTable<String, String> hMap1 = new HashTable<String, String>();
		// hMap.put(1, "Hello");
		hMap1.put("A", "Apple");
		hMap1.put("B", null);

		System.out.println(hMap1.get("A") + " " + hMap1.get("B"));
	}

}
