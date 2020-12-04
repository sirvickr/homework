using System;
using System.Text;

namespace lab8
{
    class Program
    {
        // меняет местами первую букву со второй, третью с четвертой и т.д.
        static string SwapCouples(string s)
        {
            StringBuilder sb = new StringBuilder(s);
            for (int i = 0; i < sb.Length; i += 2)
            {
                char ch = sb[i];
                if (i < sb.Length - 1)
                {
                    sb[i] = sb[i + 1];
                    sb[i + 1] = ch;
                }
            }
            return sb.ToString();
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
            StringBuilder sb = new StringBuilder(s);
            return sb.Replace(substr1, substr2).ToString();
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

        static void RemoveIf(ref StringBuilder sb, ref int begin, ref int end, char ch)
        {
            int length = end - begin;
            if (length > 0)
            {
                if (sb[end - 1] == ch)
                {
                    sb.Remove(begin, length);
                    begin -= length;
                    end -= length;
                }
            }
        }

        // Печатает все слова, кроме заканчивающихся на заданный символ
        static void PrintFilteredWords(string s, char ch)
        {
            StringBuilder sb = new StringBuilder(s);
            int begin = 0;
            int index = 0;
            bool withinWord = false;
            for (; index < sb.Length; index++)
            {
                char c = sb[index];
                if(c == ' ' || c == '\t')
                {
                    if (withinWord)
                    {
                        RemoveIf(ref sb, ref begin, ref index, ch);
                        withinWord = false;
                    }
                }
                else
                {
                    if (!withinWord)
                    {
                        begin = index;
                        withinWord = true;
                    }
                }
            }
            if (index - begin > 0)
            {
                RemoveIf(ref sb, ref begin, ref index, ch);
            }
            Console.WriteLine("\nСообщение без слов, оканчивающихся на '{0}': {1}", ch, sb.ToString());
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
            PrintFilteredWords(message, 'я');

            // - Найти самое длинное слово сообщения.
            Console.WriteLine("\nСамое длинное слово в строке: {0}", TheLongestWord(message));

            Console.Write("\nНажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
