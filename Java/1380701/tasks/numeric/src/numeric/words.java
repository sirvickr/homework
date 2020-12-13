/* 1. Напечатать все слова из текстового файла, содержащие хотя бы одну цифру.
 * 
 */

package numeric;

import java.io.*;

public class words {

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
	        	String str = new String(buf);
	        	System.out.print(str);
	        	String[] words = str.split("[ \t\n]+");
        		System.out.println("");
	        	for(String word : words) {
	        		if(word.matches(".*\\d.*")) {
	        			System.out.println("\"" + word + "\"");
	        		}
	        	}
	        } 
	    } catch(IOException ex) {
	        System.out.println(ex.getMessage());
	    }       
	}

}
