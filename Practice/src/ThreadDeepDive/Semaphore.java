package ThreadDeepDive;

public class Semaphore {

	int value = 0;

	public Semaphore(int value) throws Exception {
		if (value <= 0)
			throw new Exception("Enter right no of Semaphore");
		this.value = value;
	}

	synchronized void down() {
		while (value == 0) {
			try {
				wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		value--;
	}

	synchronized void up() {
		value++;
		notifyAll();
	}

}
