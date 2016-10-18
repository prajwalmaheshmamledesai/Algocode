package leeteCode;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class client {
	public static void start() {
		// TODO Auto-generated method stub
		try {
			Socket client = new Socket("localhost", 5555);

			PrintWriter out = new PrintWriter(client.getOutputStream(), true);

			BufferedReader buf = new BufferedReader(new InputStreamReader(
					client.getInputStream()));
			Scanner sc=new Scanner(System.in);
			String send = sc.nextLine();
			out.println(send);

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	public static void main(String[] args) {
		start();
	}

}
