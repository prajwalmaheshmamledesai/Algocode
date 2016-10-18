package leeteCode;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;

public class Server {

	public static void main(String[] args) {
		String ip = "localhost";
		int port = 5555;

		try {
			ServerSocket ser = new ServerSocket(port);
			Socket client = ser.accept();

			PrintWriter out = new PrintWriter(client.getOutputStream(),true);
			
			BufferedReader buf = new BufferedReader(new InputStreamReader(
					client.getInputStream()));
			String in;
			while ((in = buf.readLine()) != null) {
				System.out.println("Recieved "+in);
			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
