/* 4. Входной файл содержит сведения о спортивных товарах: наименование, вид спорта, год выпуска, цена. 
 * Найти товары с массой от 15 до 25 кг, с ценой от 10000 до 50000, подсчитать их количество и дописать 
 * ответ в конец файла.
 */

package commodities;

import java.io.*;

public class Sports {

	public static void main(String[] args) {
		String fileName = "input.txt";
    	int count = 0;
		try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
			String line;
			while ((line = br.readLine()) != null) {
	        	System.out.println(line);
	        	String[] words = line.split("[\t]+");
	        	if(words.length != 5) {
        			continue;
	        	}
	        	float weight = Float.parseFloat(words[3]);
	        	float price = Float.parseFloat(words[4]);
	        	if(weight >= 15000.0f && weight <= 25000.0f && price >= 10000.0f && price <= 50000.0f) {
	        		count++;
	        	}
			}
	    } catch(IOException ex) {
	        System.out.println(ex.getMessage());
	    }
		try (FileWriter writer = new FileWriter(fileName, true)) {
			writer.write("\nРазмер подмножества товаров, удовлетворяющих условию: " + count);
	    } catch(IOException ex) {
	        System.out.println(ex.getMessage());
		}
	}

}
