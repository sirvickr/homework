/* 2. Выделить в текстовом файле все слова, разделенные символами-разделителями «_.,;:\n\t!?», 
 * и все слова, в которых количество гласных латинских букв равно количеству согласных латинских букв, 
 * построчно записать в другой текстовый файл.
 */

package separators;

import java.io.*;

public class Files {

	public static void main(String[] args) {
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
		try {
			FileReader reader = new FileReader("input.txt");
			
			int result = reader.read(buf);
			if(result != -1) {
				FileWriter writer = new FileWriter("output.txt", false);

				String vowels = "aeiouyAEIOUY";
			
				String str = new String(buf);
				System.out.print(str);
				String[] words = str.split("[_.,;:\\n\\t!?]+");
				System.out.println("");
				for(String word : words) {
					int vowelCount = 0;
					int consonantCount = 0;
					for(int i = 0; i < word.length(); i++) {
						char ch = word.charAt(i);
						if (vowels.indexOf(ch) >=0) {
							vowelCount++;
						} else if(Character.isLetter(ch)) {
							consonantCount++;
						}
					}
					if(vowelCount == consonantCount) {
						System.out.println("\"" + word + "\": v " + vowelCount + " c " + consonantCount);
						writer.write(word);
						writer.append('\n');
					}
				} // for(word)
				writer.close();
			}
			
			reader.close();
	    } catch(IOException ex) {
	        System.out.println(ex.getMessage());
	    }       
	}

}
