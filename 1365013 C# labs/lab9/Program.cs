using System;

namespace lab9
{
    class Program
    {
        const int RuFirst = 1040;
        const int RuLast = 1103;

        // Вывести массив строк
        static void PrintStrings(string[] strings, string caption = "")
        {
            if (caption.Length >= 0)
                Console.WriteLine(caption);
            for (int i = 0; i < strings.Length; i++)
            {
                Console.WriteLine(strings[i]);
            }
        }

        // Определяет, содержится ли в сообщении заданное слово.
        static bool FindWord(string s, string word)
        {
            string[] words = s.Split(new char[4] { ' ', ',', '.', ';' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                if (words[i] == word)
                {
                    return true;
                }
            }
            return false;
        }

        // Замените искомое слово.
        static void PrintCensoredWords(string s, string oldWord, string newWord)
        {
            Console.WriteLine("\nСлова зацензурированного сообщения (\"{0}\" заменяется на \"{1}\"):", oldWord, newWord);
            string[] words = s.Split(new char[4] { ' ', ',', '.', ';' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                if (words[i] != oldWord)
                {
                    Console.WriteLine(words[i]);
                }
                else
                {
                    Console.WriteLine(newWord);
                }
            }
        }

        // Вывести все слова заданной длины
        static void PrintEqualWords(string s, int n)
        {
            Console.WriteLine("\nСлова сообщения, длина которых равна {0} символов:", n);
            string[] words = s.Split(new char[4] { ' ', ',', '.', ';' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                if (words[i].Length == n)
                {
                    Console.WriteLine(words[i]);
                }
            }
        }

        // Вывести все русские слова в тексте.
        static void PrintCapitalizedWords(string s)
        {
            Console.WriteLine("\nВсе руские слова");
            string[] words = s.Split(new char[4] { ' ', ',', '.', ';' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                bool russian = true;
                for (int j = 0; j < words.Length; j++)
                {
                    if (words[i][0] < RuFirst || words[i][0] > RuLast)
                    {
                        russian = false;
                        break;
                    }
                }
                if(russian)
                {
                    Console.WriteLine(words[i]);
                }
            }
        }

        // Вывести все слова в сообщении заданной длины, записанные с заглавной буквы
        static void PrintCapitalizedWords(string s, int n = -1)
        {
            Console.Write("\nСлова сообщения, начинающиеся с заглавной буквы");
            if(n >= 0)
                Console.Write(", длина которых равна {0} символов:", n);
            Console.WriteLine("");
            string[] words = s.Split(new char[4] { ' ', ',', '.', ';' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                if (words[i][0] != Char.ToUpper(words[i][0]))
                    continue;
                if (n >= 0 && words[i].Length != n)
                    continue;
                Console.WriteLine(words[i]);
            }
        }

        // Вывести номера телефонов, записанные в формате xx-xx-xx, xxx-xxx или xxx-xx-xx
        static void PrintPhoneNumbers(string s)
        {
            Console.WriteLine("\nСлова сообщения, имеющие формат телефонного номера (xx-xx-xx, xxx-xxx или xxx-xx-xx)");
            string[] words = s.Split(new char[4] { ' ', ',', '.', ';' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                if(words[i].IsPhoneNumber())
                {
                    Console.WriteLine(words[i]);
                }
            }
        }

        static void Main(string[] args)
        {
            // Часть 1
            // Дана строка, в которой содержится осмысленное текстовое сообщение. 
            // Слова сообщения разделяются пробелами и знаками препинания.

            string message = "Бессмысленное текстовое Сообщение, for the task: 23-14-85; 325-901; 537-22-12";
            Console.WriteLine("Часть 1. Исходная строка: {0}", message);

            string substr = "текстовое";
            Console.Write("Введите: чем заменить слово \"{0}\": ", substr);
            string substitute = Console.ReadLine();
            // Определите, содержится ли в сообщении заданное слово. 
            Console.Write("Слово {0} ", substr);
            if(!FindWord(message, substr))
                Console.Write("не ");
            Console.WriteLine("содержится в исходном сообщении");
            // Замените его на слово, определенное пользователем. 
            PrintCensoredWords(message, substr, substitute);
            Console.Write("\nНажмите любую клавишу...");
            Console.ReadKey();

            // Выведите все слова заданной длины. 
            PrintEqualWords(message, 9);
            // Выведите на экран все слова сообщения, записанные с заглавной буквы.
            PrintCapitalizedWords(message);

            // Удалите из сообщения все знаки препинания. 
            Console.WriteLine("\nСообщение без знаков препинания: {0}", message.WithoutPunctuation());
            // Найдите все русские слова в тексте.
            PrintCapitalizedWords(message);
            // Выведите на экран все слова в сообщении заданной длины записанные с заглавной буквы.
            PrintCapitalizedWords(message, 9);

            // В сообщении могут встречаться номера телефонов, записанные в формате xx-xx-xx, xxx-xxx или xxx-xx-xx. 
            // Вывести все номера телефонов, которые содержатся в сообщении.
            PrintPhoneNumbers(message);

            Console.WriteLine("\nКлассы\n");

            // Часть 2
            // 1. Приведите пример класса (см. класс Accumulator)
            // 2. Для объявленного класса определите методы
            // 3. Создайте несколько экземпляров класса и продемонстрируйте работу с ним.

            Human infant = new Human();
            for (int i = 0; i < 5; i++)
                infant.Grow();
            infant.Print();
            Console.WriteLine("");

            Human adolescent = new Human(Mammal.Genders.Female, "Светлана", 15);
            adolescent.Print();
            Console.WriteLine("");

            // Часть 3

            // 1. Для вашего примера разработайте родительский класс и класс наследник.
            // 2. Дополните классы конструктором по умолчанию и конструктором, принимающим аргументы.
            //    При этом конструктор дочернего класса должен ссылаться на конструктор родительского класса или на свой другой конструктор.
            // 3. Дополните класс статическим полем.
            // 4. Дополните класс полем – экземпляром параметризованного класса.
            // 5. Дополните класс деструктором.
            // 6. Написать демонстрационную программу, в которой создаются и разрушаются объекты 
            //    пользовательского класса и каждый вызов конструктора и деструктора сопровождается 
            //    выдачей соответствующего сообщения (какой объект, какой конструктор или деструктор вызван).

            Employee mark = new Employee(Mammal.Genders.Male, "Марк", "Google", 25);
            Console.WriteLine("");

            // Часть 4

            // 1. Дополните класс свойствами разных видов (только для чтения, только для записи, для записи и для чтения). 
            // 2. Дополните класс примером скрытия члена базового класса. Продемонстрируйте как образом из потомка можно обратиться к скрытым полям и методам.
            // 3. Продемонстрируйте для вашего класса пример совместимости типов.

            Mammal mammal = new Employee(Mammal.Genders.Male, "Фёдор", "Microsoft", 22);
            Console.WriteLine("");

            // Часть 4

            // 5. Разработать класс для представления объекта матрица, состоящая из элементов типа short int.
            //    Определить конструктор с двумя параметрами целого типа – размерность матрицы, 
            //    который можно использовать как конструктор умолчания.
            //    Определить конструктор, который создаёт новую матрицу таким образом, что все её элементы больше
            //    элементов другой матрицы на заданное число, и который можно использовать как конструктор копирования.
            //    Определить деструктор. 
            //    Определить преобразования из переменной типа short int в матрицу – заполнение матрицы 
            //    и из матрицы в переменную типа double – среднее арифметическое элементов матрицы.

            Matrix m1 = new Matrix();
            m1.Print("default:");
            Matrix m2 = new Matrix(2, 3, 1);
            m2.Print("\nexplicit:");
            Matrix m3 = new Matrix(m2);
            m3.Print("\nmodified copy:");
            Matrix m4 = Convert.ToInt16(3);
            m4.Print("\nfrom short int:");

            Matrix m = new Matrix(3, 4);
            for (int i = 0; i < m.Rows; i++)
            {
                for (int j = 0; j < m.Rows; j++)
                {
                    m[i, j] = Convert.ToInt16(i + j);
                }
            }
            m.Print("\nm:");
            Console.WriteLine($"\navg(m): {(double)m}");

            // 6. Определите класс Time, реализующий понятие времени. 
            //    Также напишите программу, использующую созданный класс, выводящую его на экран. 
            //    Текущее время задается с клавиатуры. 
            //    Для класса дополнительно определите операцию преобразования.

            Console.WriteLine("\nТестирование класса Time");

            Time time1 = new Time(13, 45, 71);
            Console.WriteLine($"\nВремя (13, 45, 71): {time1.ToString()}");

            Time time2 = new Time(0, 0, 3725);
            Console.WriteLine($"\nВремя (1ч 2м 5с, конструктор): {time2.ToString()}");

            Time time3 = 3725;
            Console.WriteLine($"\nВремя (1ч 2м 5с, оператор): {time3.ToString()}");

            Console.WriteLine($"\nСекунды (3725с, оператор): {(int)time3}");

            Console.Write("\nВведите текущее время с клавиатуры (Ч:М:С): ");
            string s = Console.ReadLine();
            Time time = s;
            Console.WriteLine($"\nТекущее время: {time.ToString()}");

            Console.Write("\nНажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
