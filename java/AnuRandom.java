import java.io.*;
import java.net.*;

/**
@author Kristian Lundkvist <kristian.lundkvist@gmail.com>
*/

/**
A class for downloading and parsing a 1024 bytes of random data from http://150.203.48.55/RawChar.php
*/
public class AnuRandom{

	/**
	DataInputStream using for reading from the website.
	*/
	private DataInputStream in;

	/**
	Variable temporary storing the webpage.
	*/
	private String page;

	/**
	Stores the bytes as a String variable.
	*/
	private String bytes;

	/**
	The number of bytes to download.
	*/
	private int numberOfBytes;

	/**
	Standard constructor.
	*/
	public AnuRandom(){
		this.in = null;
		this.page = "";
		this.bytes = "";
		this.numberOfBytes = 0;
	}

	/**
	Constructor.
	@param numberOfBytes The number of bytes to download
	*/
	public AnuRandom(int numberOfBytes){
		this.numberOfBytes = numberOfBytes;
		this.in = null;
		this.page = "";
		this.bytes = "";
	}

	/**
	Set the number of bytes.
	@param numberOfBytes The new number of bytes
	*/
	public void setNumberOfBytes(int numberOfBytes){
		this.numberOfBytes = numberOfBytes;
	}

	/**
	Returns the number of bytes.
	@return The number of bytes
	*/
	public int getNumberOfBytes(){
		return this.numberOfBytes;
	}

	/**
	Runs to methods to download and parse the webpage.
	Returns the bytes from the webpage.
	Handles exceptions internally by exiting application.
	@return The downloaded bytes
	*/
	public byte[] getBytes(){
		try {
			return getBytesSafe();
		} catch(MalformedURLException e){
			e.printStackTrace();
			System.exit(1);
		}
		catch(IOException e){
			e.printStackTrace();
			System.exit(1);
		}
		return null;
	}
	
	/**
	Runs to methods to download and parse the webpage.
	Returns the bytes from the webpage.
	Passes exceptions to be handled externally
	@return The downloaded bytes
	@throws IOException 
	@throws MalformedURLException 
	 */
	public byte[] getBytesSafe() throws MalformedURLException, IOException {
		byte[] temp = new byte[this.numberOfBytes];
		String store = "";

		while(store.length() < this.numberOfBytes){
			this.getPage();
			this.parsePage();
			store += this.bytes;
		}
		byte[] storeTemp = store.getBytes();
		for(int i = 0; i < this.numberOfBytes; i++){
			temp[i] = storeTemp[i];
		}

		return temp;
	}

	/**
	Downloads the webpage and stores it in memory.
	@throws MalformedURLException 
	@throws IOException 
	*/
	@SuppressWarnings("deprecation")
	public void getPage() throws MalformedURLException, IOException {
		try{
			URL u = new URL("http://150.203.48.55/RawChar.php");
			this.in = new DataInputStream(new BufferedInputStream(u.openStream()));
			String temp = "";
			while ((temp = this.in.readLine()) != null){
				this.page += temp;
			}
		} finally{
			try{
				if(this.in != null) {
					this.in.close();
				}
			}
			catch(IOException e){
				e.printStackTrace();
			}
		}
	}

	/**
	Parses the random bytes from the webpage.
	*/
	public void parsePage(){

		int start;
		int end;

		start = this.page.indexOf("<table class=\"rng\" cellpadding=\"10\"> <tr><td>");
		end = this.page.indexOf("</td></tr></table><br />", start);
		start += 45;

		this.bytes = this.page.substring(start, end);

	}
}
