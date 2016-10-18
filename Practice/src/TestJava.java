import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;

public class TestJava {

	static {
		System.out.println("I am static");

	}

	final int a = 10;

	@Override
	protected void finalize() throws Throwable {
		System.out.println("I am finalize");
		super.finalize();
	}

	public static void main(String[] args) throws Throwable {
		TestJava java = new TestJava();
		// try {
		//
		// System.out.println(java.a);
		// System.out.println(0 / 0);
		// } finally {
		// System.out.println("I m finally");
		// java.finalize();
		// }
		//
		// TestJava.AbstractClass derived = java.new Derived();
		// derived.Draw();
		// derived.sayHi();

	}

	public interface Levels {
		public class Items {
			public String value;
			public String path;

			public String getValue() {
				return value;
			}
		}
	}

	public abstract class AbstractClass implements Externalizable {
		void Draw() {
			System.out.println("I Draw");
		}

		void sayHi() {
			System.out.println("Say HI");

		}
	}

	public class Derived extends AbstractClass {
		@Override
		void Draw() {
			System.out.println("I Draw Derived");
		}

		@Override
		public void writeExternal(ObjectOutput out) throws IOException {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
			// TODO Auto-generated method stub
			
		}
	}

}
