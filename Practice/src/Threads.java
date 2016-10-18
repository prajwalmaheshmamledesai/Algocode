import java.util.UUID;

import javax.swing.plaf.SliderUI;

public class Threads {

	static Integer counter = 0;

	public static class ThreadClass implements Runnable {
		int id;

		public ThreadClass(int id) {
			this.id = id;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			for (int i = 0; i < 10; i++) {
				synchronized (counter) {

					System.out.println(counter++ + " Thread id : " + id);
//					try {
//						Thread.sleep(5);
//					} catch (InterruptedException e) {
//						// TODO Auto-generated catch block
//						e.printStackTrace();
//					}
				}
			}

		}

	}

	public static void main(String[] args) throws InterruptedException {
		Thread thread = new Thread(new ThreadClass(1));
		Thread thread2 = new Thread(new ThreadClass(2));

		thread.start();
		thread2.start();

		thread.join();
		thread2.join();
	}

}
