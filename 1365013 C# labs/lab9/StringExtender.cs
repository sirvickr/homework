using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
            Regex[] expressions = new Regex[] {
                new Regex(@"^\d{2}-\d{2}-\d{2}"),
                new Regex(@"^\d{3}-\d{3}"),
                new Regex(@"^\d{3}-\d{2}-\d{2}"),
            };
            foreach(Regex regex in expressions)
                if(regex.Matches(s).Count > 0)
                    return true;
            return false;
        }
    }
}
