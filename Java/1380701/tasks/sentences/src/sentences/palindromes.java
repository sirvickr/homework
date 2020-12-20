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
		// открываем исходный файл для чтения
		FileReader reader = new FileReader("input.txt");
		// читаем весь файл в буфер
		int result = reader.read(buf);
		if(result != -1) {
			// в случае успеха, открываем также файл для записи результатов
			FileWriter writer = new FileWriter("output.txt", true);
			// преобразуем массив байт в удобный строковый тип
			String str = new String(buf);
			// разделяем всё содержимое файла на предложения (предложения отделяются точкой)
			String[] sentences = str.split("[.]+");
			// объявляем флаг, который будет истинным только при первой итерации
			boolean first = true;
			// проходим в цикле по каждому предложению
			for(String sentence : sentences) {
				// объявляем флаг, сигнализирующий наличие палиндрома в данном предложении
        		boolean hasPalindrome = false;
        		// делим данное предложение на отдельные слова
	        	String[] words = sentence.split("[ \t,-;:\n]+");
	        	// проходим в цикле по каждому слову данного предложения
	        	for(String word : words) {
	        		if(word.length() > 1) {
	        			// объявляем флаг, сигнализирующий, что данное слово является палиндромом
	        			// и сразу предполагаем, что это так и есть (истина)
		        		boolean isPalindrome = true;
		        		// объявляем индекс для доступа к буквам с конца данного слова
		        		int j = word.length() - 1;
		        		// проходим в цикле по буквам слова, одновременно и с начала, и с конца
		        		for(int i = 0; i < word.length() / 2; i++, j--) {
		        			// сравниваем первую букву с последней, вторую с предпоследней и т.д.
		        			if(Character.toLowerCase(word.charAt(i)) != Character.toLowerCase(word.charAt(j))) {
		        				// как только увидели, что буквы разные, сразу понимаем, что данное слово не симметрично
		        				isPalindrome = false;
		        				// и выходим из цикла по буквам, дальше не важно - слово уже точно не является палинлромом
		        				break;
		        			}
		        		}
		        		// если всё-таки симметричное слово найдено, отмечаем, что данное предложение содержит палиндром
		        		if(isPalindrome) {
		        			System.out.println("\"" + word + "\"");
		        			hasPalindrome = true;
		        			// и прекращаем перебор слов, поскольку предложение уже всё равно надо записать
		        			// и нет смысла дальше тратить на него время
		        			break;
		        		}
	        		}
	        	} // for(word)
	        	// если в только что обрботанном предложении найдено симметричное слово, пишем его в файл с результатами
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
