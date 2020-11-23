using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab8
{
    class Program
    {
        // меняет местами первую букву со второй, третью с четвертой и т.д.
        static string SwapCouples(string s)
        {
            string result = "";
            for (int i = 0; i < s.Length; i += 2)
            {
                char c = s[i];
                if(i < s.Length - 1)
                    result += s[i + 1];
                result += s[i];
            }
            return result;
        }

        // подсчитывает количество букв в строке
        static int CountLetters(string s)
        {
            int count = 0;
            for (int i = 0; i < s.Length; i++)
            {
                if(Char.IsLetter(s[i]))
                {
                    count += 1;
                }
            }
            return count;
        }

        // заменяет все вхождения подстроки substr1 на подстроку substr2
        static string ReplaceSubstr(string s, string substr1, string substr2)
        {
            return s.Replace(substr1, substr2);
        }

        // Вывести только те слова сообщения, которые содержат не более чем n букв
        static void PrintShortWords(string s, int n)
        {
            Console.WriteLine("\nСлова сообщения, не превыщающие {0} символов:", n);
            string[] words = s.Split(new char[5] { ' ', ',', '.', ';', '-' }, StringSplitOptions.RemoveEmptyEntries);
            for(int i = 0; i < words.Length; i++)
            {
                if(words[i].Length <= n)
                {
                    Console.WriteLine(words[i]);
                }
            }
        }

        // Печатает все слова, кроме заканчивающихся на заданный символ
        static void PrintFilteredWords(string s, char ch)
        {
            Console.WriteLine("\nСлова сообщения, не оканчивающихся на '{0}':", ch);
            string[] words = s.Split(new char[5] { ' ', ',', '.', ';', '-' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                if (!words[i].EndsWith(ch.ToString()))
                {
                    Console.WriteLine(words[i]);
                }
            }
        }

        // Найти самое длинное слово сообщения
        static string TheLongestWord(string s)
        {
            int maxIndex = 0; // индекс самого длинного слова
            string[] words = s.Split(new char[5] { ' ', ',', '.', ';', '-' }, StringSplitOptions.RemoveEmptyEntries);
            if(words.Length == 0)
                return "";
            for (int i = 1; i < words.Length; i++)
            {
                if (words[i].Length > words[maxIndex].Length)
                {
                    maxIndex = i;
                }
            }
            return words[maxIndex];
        }

        static void Main(string[] args)
        {
            string message = "Осмысленное текстовое сообщение, для примера";
            Console.WriteLine("Исходная строка: {0}", message);

            // I) Разработать программу, которая для заданной строки s:

            // - меняет местами первую букву со второй, третью с четвертой и т.д.
            Console.WriteLine("\nПопарная замена букв: {0}", SwapCouples(message));

            // - подсчитывает количество букв в строке
            Console.WriteLine("\nКоличество букв в строке: {0}", CountLetters(message));

            // - заменяет все вхождения подстроки substr1 на подстроку substr2
            Console.WriteLine("\nПодмена подстроки: {0}", ReplaceSubstr(message, "сообщение", "послание"));

            // II) Дана строка, в которой содержится осмысленное текстовое сообщение. 
            // Слова сообщения разделяются пробелами и знаками препинания.

            // - Вывести только те слова сообщения, которые содержат не более чем n букв.
            PrintShortWords(message, 7);

            // - Удалить из сообщения все слова, которые заканчиваются на заданный символ.
            PrintFilteredWords(message, 'е');

            // - Найти самое длинное слово сообщения.
            Console.WriteLine("\nСамое длинное слово в строке: {0}", TheLongestWord(message));

            Console.Write("\nНажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
