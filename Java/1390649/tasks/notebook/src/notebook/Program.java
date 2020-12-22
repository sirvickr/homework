package notebook;
import java.io.*;
import java.util.HashMap;

public class Program {

	public static void main(String[] args) {
		String fileName = "notebook.txt";
		//<debug>
		
		//args = new String[] { "-add", "1", "Важная заметка" };
		//args = new String[] { "-add", "2", "Встреча" };
		//args = new String[] { "-add", "3", "Урок" };

		args = new String[] { "-show" };

		//args = new String[] { "-rm", "XXX" };
		
		//</debug>
		
		if(args.length == 0) {
			System.out.println("no arguments");
			return;
		}
		
		HashMap<String, String> data = new HashMap<>();
		
		// прочитать данные
		try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
			String line;
			while ((line = br.readLine()) != null) {
	        	String[] words = line.trim().split("[\t]+");
	        	if(words.length == 2) {
		        	data.put(words[0], words[1]);
	        	}
			}
	    } catch(IOException ex) {
	        System.out.println("Файл с данными отсутствует");
	    }


		String cmd = args[0];
		if(cmd == "-add") {
			if(args.length == 3) {
				data.put(args[1], args[2]);
		        System.out.println("Добавлена заметка " + args[1] + ": " + args[2]);
			}
		} else if(cmd == "-rm") {
			if(args.length == 2) {
				String key = args[1];
				if(data.containsKey(key)) {
					data.remove(key);
			        System.out.println("Удалена заметка " + key);
				}
			}
		} else if(cmd == "-show") {
			for (String key : data.keySet()) {
				System.out.println(key + "\t" + data.get(key));
			}
		}

		// сохранить данные
		try (FileWriter writer = new FileWriter(fileName, false)) {
			for (String key : data.keySet()) {
				writer.write(key + "\t" + data.get(key));
				writer.append('\n');
			}
	    } catch(IOException ex) {
	        System.out.println(ex.getMessage());
		}
	}

}
