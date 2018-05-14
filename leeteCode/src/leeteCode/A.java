package leeteCode;

public class A {
	
	A(){
		System.out.println("A Called");
		
	}
	
	static void  fun(int a){
		a =a+1;
		System.out.println(a);
	}
	
	//A a = new A();
	static int a = 0;
	public static void main(String[] args) {
		
		fun(a);
		System.out.println(a);
	}

}
