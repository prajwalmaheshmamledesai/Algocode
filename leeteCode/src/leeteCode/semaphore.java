package leeteCode;

public class semaphore {
	
	static int value = 0;
	
	public semaphore(int val) {
		// TODO Auto-generated constructor stub
		if(val < 0 )
			val =0;
		this.value = val;
		
	}
	
	
	synchronized void down(){
		while (value == 0){
			try {
				wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		value--;
		
		
	}
	
	synchronized void up(){
		value++;
		notify();
		
	}
	
	public static void main(String[] args) {
		int noofsem = 1;
		semaphore sem = new semaphore(noofsem);
		Thread[]  th = new Thread[3];
		for (int i = 0; i < th.length; i++) {
			th[i] = new Thread(new myThread(sem));
			th[i].start();
		}
		
	}
	

}
