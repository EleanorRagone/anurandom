public class example{
	public static void main(String[] args){
		AnuRandom random = new AnuRandom();
		random.getPage();
		random.parsePage();
		String temp = new String(random.getBytes());
		System.out.println(temp);
	}
}
