/* 5. Определить класс "одномерный массив". В класс включить два конструктора:
1) создание одномерного массива по количеству элементов; 2) копирующий конструктор.
Определить функции-члены: вывод на экран элементов массива, получение значения 
элемента по его порядковому номеру, нахождение произведения элементов, значения которых
по модулю меньше 5
*/

#include <iostream>
#include <time.h>

using namespace std;

// класс одномерного массива
template<class T>
class Array {
public:
	// конструктор по количеству элементов
	Array(size_t size, T init = T()) : data(nullptr), size(size) {
		if (size) {
			data = new T[size];
			for (size_t i = 0; i < size; i++) {
				data[i] = init;
			}
		}
	}
	// конструктор копирования
	Array(const Array& a) {
		if (size < a.size) {
			if (data)
				delete[] data;
			data = new T[a.size];
		}
		size = a.size;
		for (size_t i = 0; i < size; i++) {
			data[i] = a.data[i];
		}
	}
	// деструктор
	~Array() {
		if (data) {
			delete[] data;
		}
	}
	// функция доступа к количеству элементов
	size_t Size() const {
		return size;
	}
	// вывод на экран элементов массива
	void Print() const {
		for (size_t i = 0; i < size; i++)
			cout << data[i] << " ";
		cout << endl;
	}
	// получение значения элемента по его порядковому номеру (функция)
	T& GetAt(size_t index) {
		return data[index];
	}
	// получение значения элемента по его порядковому номеру (оператор)
	T& operator[](size_t index) {
		return data[index];
	}
	// произведение элементов, значения которых по модулю меньше 5
	T ProductLess5() const {
		T p = T(1);
		for (size_t i = 0; i < size; i++)
			if(abs(data[i]) < T(5))
				p *= data[i];
		return p;
	}
private:
	T* data;     // указатель на массив с данными
	size_t size; // размер массива (количество элементов)
};

int main()
{
	setlocale(LC_CTYPE, "Russian");

	// определяем псевдоним типа для целочисленного массива
	typedef Array<int> IntArray;
	// инициализируем генератор случайных чисел текущим временем
	srand(time(NULL));
	// объявляем целочисленный массив из 10 элементов
	IntArray array(10);
	// заполняем его случайными значениями от -9 до 9
	for (size_t i = 0; i < array.Size(); i++)
		array[i]  = (rand() % 19) - 9;
	// распечатываем получившийся массив
	array.Print();
	// выводим произведение элементов, значения которых по модулю меньше 5
	cout << "произведение элементов, значения которых по модулю меньше 5: " << array.ProductLess5() << endl;

	return 0;
}
