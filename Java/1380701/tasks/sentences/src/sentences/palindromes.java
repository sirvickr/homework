/* 3. Дано два файла с предложениями.  
 * Дописать в конец второго файла предложения из первого файла, в которых есть симметричные слова.
 */

package sentences;

import java.io.*;

public class palindromes {

	public static void main(String[] args) throws IOException {
		String fileName = "input.txt";
		File file = new File(fileName);
		if(!file.exists()) {
	        System.out.println("Файл не найден: " + fileName);
	        return;
		}
		if(0 == file.length()) {
	        System.out.println("Файл пуст: " + fileName);
	        return;
		}
		char[] buf = new char[(int) file.length()];

		FileReader reader = new FileReader("input.txt");
		
		int result = reader.read(buf);
		if(result != -1) {
			FileWriter writer = new FileWriter("output.txt", true);
		
			String str = new String(buf);
			String[] sentences = str.split("[.]+");
			boolean first = true;
			for(String sentence : sentences) {
        		boolean hasPalindrome = false;
	        	String[] words = sentence.split("[ \t,-;:\n]+");
	        	for(String word : words) {
	        		if(word.length() > 1) {
		        		boolean isPalindrome = true;
		        		int j = word.length() - 1;
		        		for(int i = 0; i < word.length() / 2; i++, j--) {
		        			if(Character.toLowerCase(word.charAt(i)) != Character.toLowerCase(word.charAt(j))) {
		        				isPalindrome = false;
		        				break;
		        			}
		        		}
		        		if(isPalindrome) {
		        			System.out.println("\"" + word + "\"");
		        			hasPalindrome = true;
		        			break;
		        		}
	        		}
	        	} // for(word)
	        	if(hasPalindrome) {
	        		if(first) {
	        			// отделяем содержимое файла от первого предложения
	        			first = false;
						writer.append('\n');
	        		}
					writer.write(sentence);
					writer.append('.');
	        	}
			} // for(sentence)
			writer.close();
		}
		
		reader.close();
	}

}
