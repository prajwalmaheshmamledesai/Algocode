package leeteCode;

import javax.swing.plaf.SliderUI;

public class myThread implements Runnable {

	private semaphore mutex = null;

	public myThread(semaphore sem) {
		// TODO Auto-generated constructor stub
		if (mutex == null)
			mutex = sem;

	}

	static int n = 1;

	synchronized void odd() {
		n++;
		try {

			Thread.sleep(10);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		n += 1;
		System.out.println(Thread.currentThread() + " n = " + n);
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		mutex.down();
		System.out.println(Thread.currentThread() + " entered");

		odd();

		mutex.up();
		System.out.println(Thread.currentThread() + " left");

	}

}
