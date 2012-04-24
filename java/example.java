public class example{
	public static void main(String[] args){
		AnuRandom random = new AnuRandom(2048);
		random.getPage();
		random.parsePage();

		String temp = new String(random.getBytes());
		System.out.println(temp);
	}
}
