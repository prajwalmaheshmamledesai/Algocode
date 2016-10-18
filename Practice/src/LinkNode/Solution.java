package LinkNode;

public class Solution {

	public static class ListNode {
		int val;
		ListNode next;

		ListNode(int x) {
			val = x;
		}
	}

	public static ListNode addTwoNumbers(ListNode l1, ListNode l2) {
		ListNode head = null;
		int carry = 0;
		while (l1 != null && l2 != null){
			int temp = l1.val + l2.val + carry;
			temp = temp % 10;
			ListNode listNode = new ListNode(temp);
			listNode.next = head;
			head = listNode;
			
			l1 = l1.next;
			l2 = l2.next;
		}
		
		while (l1 != null){
			int temp = l1.val + carry;
			temp = temp % 10;
			ListNode listNode = new ListNode(temp);
			listNode.next = head;
			head = listNode;
			
			l1 = l1.next;
			
		}
		
		while (l2 != null){
			int temp = l2.val + carry;
			temp = temp % 10;
			ListNode listNode = new ListNode(temp);
			listNode.next = head;
			head = listNode;
			
			l2 = l2.next;
			
		}
		
		return head;

	}
	
	public static void main(String[] args) {
		ListNode  listNode = null;
		ListNode listNode2 = new ListNode(2);
		listNode.next = listNode2;
	}

}
