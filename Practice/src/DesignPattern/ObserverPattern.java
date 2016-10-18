package DesignPattern;

import java.util.ArrayList;
import java.util.List;

public class ObserverPattern {

	public abstract class Observer {
		protected Subject subject;

		abstract Object Update(Object object);
	}

	public class Subject {
		List<Observer> observers = new ArrayList<>();

		private int state = 0;

		public int getState() {
			return state;
		}

		public void setState(int state) {
			this.state = state;
			notifyAllObservers();
		}

		public void notifyAllObservers() {
			for (Observer observer : observers) {
				observer.Update(null);
			}

		}

		public void attachObserver(Observer observer) {
			observers.add(observer);
		}

		public void detachObserver(Observer observer) {
			observers.remove(observer);
		}

	}

	public class BinaryObserver extends Observer {

		public BinaryObserver(Subject subject) {
			subject.attachObserver(this);
		}

		@Override
		Object Update(Object object) {
			System.out.println("I am Binary Observer");
			return null;
		}

	}

	public class DecimalObserver extends Observer {

		public DecimalObserver(Subject subject) {
			subject.attachObserver(this);
		}

		@Override
		Object Update(Object object) {
			System.out.println("I am Decimal Observer");
			return null;
		}

	}

	public static void main(String[] args) {
		ObserverPattern observerPattern = new ObserverPattern();
		Subject subject = observerPattern.new Subject();

		BinaryObserver binaryObserver = observerPattern.new BinaryObserver(subject);
		DecimalObserver decimalObserver = observerPattern.new DecimalObserver(subject);

		System.out.println("First change");
		subject.setState(10);
		System.out.println("Second change");
		subject.setState(25);

	}

}
