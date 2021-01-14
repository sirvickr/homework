using System.Text.RegularExpressions;

namespace lab9
{
    public static class StringExtender
    {
        // Удаляет из сообщения все знаки препинания
        public static string WithoutPunctuation(this string s)
        {
            return new Regex(@"[,.;-]").Replace(s, "");
        }
        // Является ли строка телефонным номером, записанным в формате xx-xx-xx, xxx-xxx или xxx-xx-xx
        public static bool IsPhoneNumber(this string s)
        {
            Regex[] expressions = new Regex[] {
                new Regex(@"^\d{2}-\d{2}-\d{2}$"),
                new Regex(@"^\d{3}-\d{3}$"),
                new Regex(@"^\d{3}-\d{2}-\d{2}$"),
            };
            foreach(Regex regex in expressions)
                if(regex.Matches(s).Count > 0)
                    return true;
            return false;
        }
    }
}
