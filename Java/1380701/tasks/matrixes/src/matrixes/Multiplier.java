/* 5. В первом файле хранится k матриц размерности m×n, во втором l матриц размерности n×m. 
 * Записать в третий файл перемножение матриц из первого и второго файла с четными номерами, 
 * если в файлах существуют матрицы с одинаковыми номерами (k может быть меньше l и наоборот). 
 * Вывести на экран содержимое первого, второго и третьего файлов.
 * */

package matrixes;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Multiplier {
	
	// распечатать матрицу на экране
	static void printMatrix(ArrayList<float[]> m) {
		for(float[] row : m) {
			for(float cell : row) {
				System.out.print(" " + cell);
			}
			System.out.println("");
		}
		System.out.println("");
	}
	
	// прочитать из файла матрицы с чётными номерами
	static ArrayList<ArrayList<float[]>> readEvenMatrixes(String fileName) throws IOException
	{
		System.out.println("Содержимое файла " + fileName + ":");
		ArrayList<ArrayList<float[]>> result = new ArrayList<ArrayList<float[]>>();
		BufferedReader br = new BufferedReader(new FileReader(fileName));
		String line;
		int n = 0;
		int index = 0; // номер матрицы
		ArrayList<float[]> matrix = new ArrayList<float[]>();
		while ((line = br.readLine()) != null) {
        	String[] words = line.trim().split("[\t]+");
        	if(words.length > 1) {
	        	n = words.length;
	        	float[] row = new float[n];
	        	for(int j = 0; j < n; j++)
	        		row[j] = Float.parseFloat(words[j]);
				matrix.add(row);
        	} else {
	        	if(n > 0) { // только что закончили формирование матрицы
	    			index++; // индекс сразу преобразуется в порядковый номер
	        		if(index % 2 == 0) { // если порядклвый номер чётный, добавляем матрицу в результат
	        			printMatrix(matrix);
	        			result.add(matrix);
	        		}
					matrix = new ArrayList<float[]>();

					n = 0;
	        	}
        	}
        	///System.out.println("");
		}
		br.close();
		return result;
	}
	
	// записать список матриц в файл
	static void writeMatrixList(String fileName, ArrayList<ArrayList<float[]>> matrixes) throws IOException
	{
		System.out.println("Содержимое файла " + fileName + ":");
		FileWriter writer = new FileWriter(fileName, false);
		for(int x = 0; x < matrixes.size(); x++) {
			ArrayList<float[]> m = matrixes.get(x);
			for(float[] row : m) {
				boolean first = true;
				for(float cell : row) {
					if(!first)
						writer.append('\t');
					first = false;
					writer.write(Float.toString(cell));
				}
				writer.append('\n');
			}
			writer.append('\n');
			printMatrix(m);
		}
		//writer.write("\nРазмер подмножества товаров, удовлетворяющих условию: " + count);
		writer.close();
	}
	
	public static void main(String[] args) throws IOException {
		String iFileName1 = "1.txt";
		String iFileName2 = "2.txt";
		String oFileName = "3.txt";
		ArrayList<ArrayList<float[]>> A = readEvenMatrixes(iFileName1);
		ArrayList<ArrayList<float[]>> B = readEvenMatrixes(iFileName2);
		ArrayList<ArrayList<float[]>> C = new ArrayList<ArrayList<float[]>>();
		int m = 0;
		int n = 0;
		int p = 0;
		for(int x = 0; x < A.size() && x < B.size(); x++) {
			ArrayList<float[]> a = A.get(x);
			ArrayList<float[]> b = B.get(x);
			if(p == 0) {
				if(a.isEmpty() || b.isEmpty())
					continue;
				m = a.size();
				n = a.get(0).length;
				p = b.get(0).length;
			}
			ArrayList<float[]> c = new ArrayList<float[]>(m);
            for (int i = 0; i < m; i++) {
            	float[] row = new float[p];
                for (int j = 0; j < p; j++) {
                    float value = 0;
                    for (int k = 0; k < n; k++) {
                        value += a.get(i)[k] * b.get(k)[j];
                    }
                    row[j] = value;
                }
                c.add(row);
            }
			C.add(c);
		}
		writeMatrixList(oFileName, C);
	}

}
