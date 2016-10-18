package com.prajwal.heap;

public class Heap {

	static class heapNode {
		public int size;
		public int capacity;
		public int[] harr;

		public heapNode(int capacity) {
			this.capacity = capacity;
			this.size = 0;
			this.harr = new int[capacity];
		}

	}

	static void minHeap(heapNode hn, int idx) {
		int smallest = idx;
		int left = (idx << 1) + 1;
		int right = (idx + 1) << 1;

		if (left < hn.size && hn.harr[left] < hn.harr[smallest])
			smallest = left;

		if (right < hn.size && hn.harr[right] < hn.harr[smallest])
			smallest = right;

		if (idx != smallest) {
			int temp = hn.harr[smallest];
			hn.harr[smallest] = hn.harr[idx];
			hn.harr[idx] = temp;

			minHeap(hn, smallest);
		}

	}
}
