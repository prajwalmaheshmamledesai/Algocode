package ThreadDeepDive;

public class Threads implements Runnable {

	static Integer count = 0;

	static Semaphore semaphore;

	int id;

	public Threads(int id) throws Exception {
		this.id = id;
		semaphore = new Semaphore(1);
	}

	@Override
	public void run() {
		semaphore.down();

		for (int i = 0; i < 10; i++) {
			count++;
			System.out.println(count + " " + id);
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		semaphore.up();

	}

	public static void main(String[] args) throws Exception {
		Thread thread = new Thread(new Threads(1));
		Thread thread1 = new Thread(new Threads(2));
		Thread thread2 = new Thread(new Threads(3));
		Thread thread3 = new Thread(new Threads(4));
		thread.start();
		thread1.start();
		thread2.start();
		thread3.start();

	}

}
