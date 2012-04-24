import java.io.*;
import java.net.*;

public class AnuRandom{

	private DataInputStream in;

	private String page;

	private int numberOfBytes;

	public AnuRandom(){
		this.in = null;
		this.page = "";
		this.numberOfBytes = 0;
	}

	public AnuRandom(int numberOfBytes){
		this.numberOfBytes = numberOfBytes;
		this.in = null;
		this.page = "";
	}

	public void setNumberOfBytes(int numberOfBytes){
		this.numberOfBytes = numberOfBytes;
	}

	public int getNumberOfBytes(){
		return this.numberOfBytes;
	}

	public String getPageString(){
		return this.page;
	}

	public void getPage(){
		try{
			URL u = new URL("http://150.203.48.55/RawHex.php");
			this.in = new DataInputStream(new BufferedInputStream(u.openStream()));
			String temp = "";
			while ((temp = this.in.readLine()) != null){
				this.page += temp;
			}
		}
		catch(MalformedURLException e){
			e.printStackTrace();
			System.exit(1);
		}
		catch(IOException e){
			e.printStackTrace();
			System.exit(1);
		}
		finally{
			try{
				this.in.close();
			}
			catch(IOException e){
				e.printStackTrace();
			}
		}
	}
}
