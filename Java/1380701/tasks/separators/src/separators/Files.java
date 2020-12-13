/* 2. Выделить в текстовом файле все слова, разделенные символами-разделителями «_.,;:\n\t!?», 
 * и все слова, в которых количество гласных латинских букв равно количеству согласных латинских букв, 
 * построчно записать в другой текстовый файл.
 */

package separators;

import java.io.*;
import java.util.regex.*;

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
		try(FileReader reader = new FileReader("input.txt")) {
	        int result = reader.read(buf);
	        if(result != -1) {
    			Pattern vowels = Pattern.compile("(?iu)[aeiouyAEIOUY]");

    			String str = new String(buf);
	        	System.out.print(str);
	        	String[] words = str.split("[_.,;:\\n\\t!?]+");
        		System.out.println("");
	        	for(String word : words) {
        			Matcher m = vowels.matcher(word);
        			int vowelCount = 0;
        			while (m.find()) {
        				vowelCount++;
        			}
        			System.out.println("\"" + word + "\": " + vowelCount);
	        	}
	        } 
	    } catch(IOException ex) {
	        System.out.println(ex.getMessage());
	    }       
	}

}
