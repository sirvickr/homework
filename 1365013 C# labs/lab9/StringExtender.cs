using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab9
{
    public static class StringExtender
    {
        // Удаляет из сообщения все знаки препинания
        public static string WithoutPunctuation(this string s)
        {
            string result = "";
            for (int i = 0; i < s.Length; i++)
            {
                if (!Char.IsPunctuation(s[i]))
                {
                    result += s[i];
                }
            }
            return result;
        }
        // Является ли строка телефонным номером, записанным в формате xx-xx-xx, xxx-xxx или xxx-xx-xx
        public static bool IsPhoneNumber(this string s)
        {
            // xx-xx-xx
            if (s.Length == 8 && Char.IsDigit(s[0]) && Char.IsDigit(s[1]) &&
                s[2] == '-' && Char.IsDigit(s[3]) && Char.IsDigit(s[4]) &&
                s[5] == '-' && Char.IsDigit(s[6]) && Char.IsDigit(s[7]))
            {
                return true;
            }
            // xxx-xxx
            if (s.Length == 7 && Char.IsDigit(s[0]) && Char.IsDigit(s[1]) && Char.IsDigit(s[2]) &&
                s[3] == '-' && Char.IsDigit(s[4]) && Char.IsDigit(s[5]) && Char.IsDigit(s[6]))
            {
                return true;
            }
            // xxx-xx-xx
            if (s.Length == 9 && Char.IsDigit(s[0]) && Char.IsDigit(s[1]) && Char.IsDigit(s[2]) &&
                s[3] == '-' && Char.IsDigit(s[4]) && Char.IsDigit(s[5]) &&
                s[6] == '-' && Char.IsDigit(s[7]) && Char.IsDigit(s[8]))
            {
                return true;
            }

            return false;
        }
    }
}
