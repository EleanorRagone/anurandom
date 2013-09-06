import java.io.IOException;

public class example{
	public static void main(String[] args){
    //Gets bytes from server, will exit if server inaccessible  
		AnuRandom random = new AnuRandom(2048);
		String temp = new String(random.getBytes());
		System.out.println(temp);
    //Gets bytes from server, throws catchable exception if server inaccessible 
    try {
      temp = new String(random.getBytesSafe());
    } catch (IOException e) {
      //Handle inaccessible server
    }
    
	}
}
