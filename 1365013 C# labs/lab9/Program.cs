using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

            string message = "Осмысленное текстовое Сообщение, for the task: 23-14-85; 325-901; 537-22-12";
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

            // Часть 2
            // 1. Приведите пример класса (см. класс Accumulator)
            // 2. Для объявленного класса определите методы (Add - добавить значение, Value - получить текущее значение)
            // 3. Создайте несколько экземпляров класса и продемонстрируйте работу с ним.
            Console.WriteLine("\nЧасть 2");

            Accumulator accum1 = new Accumulator();
            Accumulator accum2 = new Accumulator(5);

            Console.WriteLine("\nНачальное значение аккумулятора №1: {0}", accum1.Value());
            accum1.Add(1);
            Console.WriteLine("\nПосле добавления 1 в аккумулятор №1: {0}", accum1.Value());
            accum1.Add(8);
            Console.WriteLine("\nПосле добавления 8 в аккумулятор №1: {0}", accum1.Value());

            Console.WriteLine("\nНачальное значение аккумулятора №2: {0}", accum2.Value());
            accum2.Add(5);
            Console.WriteLine("\nПосле добавления 5 в аккумулятор №2: {0}", accum2.Value());

            Console.Write("\nНажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
