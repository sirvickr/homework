using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Globalization;

namespace bakery
{
    // Основной класс приложения
    class Program
    {
        // список хлебозаводов
        static List<Factory> factories = new List<Factory>();
        // список изделий
        static List<Product> products = new List<Product>();
        // список ингредиентов
        static List<Ingredient> ingredients = new List<Ingredient>();
        // список связей изделий с ингредиентами
        static List<Constitution> consists = new List<Constitution>();
        // настройки локализации
        static CultureInfo _cultureInfo = (CultureInfo)CultureInfo.CurrentCulture.Clone();

        // имена файлов с данными
        static string[] paths = new string[]{
            "factories",
            "products",
            "ingredients",
            "consists"
        };

        // основная функция - точка входа приложения
        static void Main(string[] args)
        {
            // дробная часть в файлах отделена точкой, а не запятой
            _cultureInfo.NumberFormat.NumberDecimalSeparator = ".";

            Console.Title = "Сеть хлебозаводов";
            // загрузить список хлебзаводов
            ReadList(0, 2);
            // загрузить список изделий
            ReadList(1, 6);
            // загрузить список ингредиентов
            ReadList(2, 5);
            // загрузить список, связывающий изделия и ингредиенты
            ReadList(3, 2);

            // цикл взаимодействия с пользователем
            while (true)
            {
                // напечатать меню и запросить выбор пользователя
                char choice = UserChoice();
                Console.WriteLine("Вы выбрали действие: " + choice);
                if (choice == '0')
                {
                    // 0 - завершаем работу
                    break;
                }
                switch(choice)
                {
                    case '1': // Перечень изделий, выпускаемых хлебозаводом
                        FactoryProductsList();
                        break;
                    case '2': // Суммарная стоимость изделий, выпускаемых хлебозаводом
                        TotalCost();
                        break;
                    case '3': // Изделия с просроченными ингредиентами
                        ExpiredProductsList();
                        break;
                    case '4': // Список хлебозаводов в порядке убывания объёма производства
                        DescendingFactoriesList();
                        break;
                    case '5': // Изделие с наибольшим количеством ингредиентов
                        MaxIngredientsProduct();
                        break;
                    case '6': // Изделие с наибольшей прибылью при реализации
                        MaxRevenueProduct();
                        break;
                    case '7': // Добавить изделие, выпускаемое хлебозаводом
                        AddProduct();
                        break;
                    case '8': // Удалить изделие, выпускаемое хлебозаводом
                        RemoveProduct();
                        break;
                    default:
                        Console.WriteLine("Такого действия нет в списке");
                        break;
                }
                Console.Write("Нажмите любую клавишу для продолжения работы ");
                Console.ReadKey(true);
            }
        }

        // Вывод пользовательского меню и запрос выбора
        static char UserChoice()
        {
            Console.Clear();
            Console.WriteLine("0 - Выход");
            Console.WriteLine("1 - Перечень изделий, выпускаемых хлебозаводом");
            Console.WriteLine("2 - Суммарная стоимость изделий, выпускаемых хлебозаводом");
            Console.WriteLine("3 - Изделия с просроченными ингредиентами");
            Console.WriteLine("4 - Список хлебозаводов в порядке убывания объёма производства");
            Console.WriteLine("5 - Изделие с наибольшим количеством ингредиентов");
            Console.WriteLine("6 - Изделие с наибольшей прибылью при реализации");
            Console.WriteLine("7 - Добавить изделие, выпускаемое хлебозаводом");
            Console.WriteLine("8 - Удалить изделие, выпускаемое хлебозаводом");
            return Console.ReadKey(true).KeyChar;
        }

        // Запрос у пользователя номера интересующего его хлебозавода
        static int RequestFactoryKey()
        {
            Console.WriteLine("Список заводов:");
            Console.WriteLine("№\tНазвание");
            foreach (var item in factories)
            {
                Console.WriteLine("{0}\t{1}", item.Key, item.Name);
            }
            Console.Write("Введите № завода: ");
            return Convert.ToInt32(Console.ReadLine().Trim());
        }

        // Перечень изделий, выпускаемых хлебозаводом
        // choose - запросить номер у пользователя
        // если false, то просто выводим список изделий
        static int FactoryProductsList(bool choose = false)
        {
            // выводим список всех хлебозаводов
            // и запрашиваем у пользователя номер
            int id = RequestFactoryKey();
            // фильтруем все изделий по номеру выбранного хлебозавода
            var result = products.Where(p => p.FactoryKey == id);
            // выводим список всех изделий в этой проекции
            Console.WriteLine("№\tВес\tЦена\tВыпуск\tГоден до\t\tНазвание");
            foreach (var p in result)
            {
                Console.WriteLine("{0}\t{1}\t{2}\t{3}\t{4}\t{5}",
                    p.Key, p.Weight, p.Price, p.Count, p.ExpiryDate, p.Name);
            }
            if (choose)
            {
                // запрашиваем у пользователя номер хлебозавода
                Console.Write("Введите № партии: ");
                return Convert.ToInt32(Console.ReadLine().Trim());
            }
            return 0;
        }

        // Суммарная стоимость изделий, выпускаемых хлебозаводом
        static void TotalCost()
        {
            // запросить у пользователя номер хлебозавода
            int id = RequestFactoryKey();
            // фильтруем изделия по выбранному номеру хлебозавода
            var result = products.Where(p => p.FactoryKey == id)
                // группируем изделия по номеру хлебозавода
                .GroupBy(p => p.FactoryKey)
                // суммируем стоимость партии в каждой группе
                .Select(g => new {
                    Id = g.Key,
                    Total = g.Sum(p => p.Price * p.Count)
                });
            // вывод результата
            foreach (var group in result)
            {
                Console.WriteLine("Суммарная стоимость выпущенных изделий: {0} руб.", group.Total);
            }
        }

        // Изделия с просроченными ингредиентами
        static void ExpiredProductsList()
        {
            // сперва получаем объединение по внешнему ключу 
            // таблицы consists и таблицы ингредиентов ingredients, 
            // включая в результат запроса как значения связующих 
            // ключей, так и информационные поля
            var ingr = consists.Join(
                 ingredients,
                 c => c.IngredientKey,
                 i => i.Key,
                 (c, i) => new {
                     c.ProductKey,
                     c.IngredientKey,
                     i.Name,
                     i.Weight,
                     i.ExpiryDate
                 }
            );
            // затем объединяем таблицу изделий products и полученную 
            // на предыдущем шаге проекцию, группируя результат по изделиям 
            // и собирая для каждого из них список просроченных ингредиентов
            var view = products.GroupJoin(
                ingr,
                p => p.Key,
                c => c.ProductKey,
                (prod, cons) => new
                {
                    prod.Key,
                    prod.Name,
                    Ingredients = cons.Where(w => w.ExpiryDate < DateTime.Now)
                }
            );
            Console.WriteLine("Изделия с просроченным ингредиентами");
            Console.WriteLine("№\tНазвание");
            // вывод результата (только позиции с просроченными ингредиентами)
            foreach (var p in view.Where(v => v.Ingredients.Count() > 0))
            {
                Console.WriteLine("{0}\t{1} (просроченных ингредиентов: {2}):",
                    p.Key, p.Name, p.Ingredients.Count());
                foreach (var i in p.Ingredients)
                {
                    Console.WriteLine("\t - {0} ({1}) {2}", i.ExpiryDate, i.IngredientKey, i.Name);
                }
            }
        }

        // Список хлебозаводов в порядке убывания объёма производства
        static void DescendingFactoriesList()
        {
            // объединяем по внешнему ключу FactoryKey таблицы 
            // хлебозаводов и изделий и группируем результат, 
            // подсчитывая для каждого хлебозавода объем выпущенной продукции. 
            // В конце сортируем результат по этому полю.
            var result = factories.GroupJoin(
                products,
                f => f.Key,
                p => p.FactoryKey,
                (fact, prods) => new
                {
                    fact.Key,
                    fact.Name,
                    Count = prods.Sum(prod => prod.Count)
                }
            ).OrderByDescending(f => f.Count);
            // вывод результата
            Console.WriteLine("№\tНазвание\tОбъём производства");
            foreach (var f in result)
            {
                Console.WriteLine("{0}\t{1}\t\t{2}", f.Key, f.Name, f.Count);
            }
        }

        // Изделие с наибольшим количеством ингредиентов
        static void MaxIngredientsProduct()
        {
            // сперва получаем объединение по внешнему ключу таблицы consists 
            // и таблицы ингредиентов ingredients, включая в результат запроса 
            // значение ключа и наименование ингредиента
            var ingr = consists.Join(
                ingredients,
                c => c.IngredientKey,
                i => i.Key,
                (c, i) => new {
                    c.ProductKey,
                    i.Name
                }
            );
            // затем объединяем таблицу изделий products и полученную 
            // на предыдущем шаге проекцию, группируя результат по изделиям 
            // и собирая для каждого из них список ингредиентов
            var view = products.GroupJoin(
                ingr,
                p => p.Key,
                c => c.ProductKey,
                (prod, cons) => new
                {
                    prod.Key,
                    prod.Name,
                    IngredCount = cons.Count(),
                    Ingredients = cons.Select(w => w)
                }
            );
            // в получившейся проекции вычисляем максимальное 
            // количество ингредиентов среди всех изделий
            int maxCount = view.Max(v => v.IngredCount);
            // вывод результата (только позиции с наибольшим числом ингредиентов)
            Console.WriteLine("Изделия с наибольшим числом ингредиентов");
            Console.WriteLine("№\tНазвание");
            foreach (var p in view.Where(v => v.IngredCount == maxCount))
            {
                Console.WriteLine("{0}\t{1} (ингредиентов: {2}):", 
                    p.Key, p.Name, p.IngredCount);
                foreach (var i in p.Ingredients)
                {
                    Console.WriteLine("\t - {0}", i.Name);
                }
            }
        }

        // Изделие с наибольшей прибылью при реализации
        static void MaxRevenueProduct()
        {
            // делаем проекцию таблицы изделий, выбирая из неё 
            // наименование и вычисляя прибыль как произведение 
            // цены за единицу изделия на объём выпуска
            var view = products.Select(p => new {
                p.Name,
                Revenue = p.Price * p.Count
            });
            // вычисляем максимальное значение прибыли среди всех изделий
            var maxVal = view.Max(v => v.Revenue);
            // фильтруем эту проекцию, оставляя только те изделия, 
            // у которых прибыль равна максимальному значению
            var result = view.Where(v => v.Revenue == maxVal);
            // вывод результата
            Console.WriteLine("Изделия с наибольшей прибылью:");
            foreach (var item in result)
            {
                Console.WriteLine("{0} ({1} руб).", item.Name, item.Revenue);
            }
        }

        // Добавить изделие, выпускаемое хлебозаводом
        static void AddProduct()
        {
            // выводим на экран список заводов хлебозаводов
            // и запрашиваем у пользователя номер
            int factoryKey = RequestFactoryKey();
            // формируем ключ
            int key = products.Count + 1;
            // запрашиваем у пользователя остальные поля
            Console.Write("Введите наименование продукции: ");
            string name = Console.ReadLine().Trim();
            Console.Write("Введите вес единицы продукции (кг): ");
            double weight = double.Parse(Console.ReadLine().Trim(), _cultureInfo);
            Console.Write("Введите дату окончания срока годности (ГГГГ-ММ-ДД): ");
            DateTime date = DateTime.Parse(Console.ReadLine().Trim());
            Console.Write("Введите объём партии (шт): ");
            int count = Convert.ToInt32(Console.ReadLine().Trim());
            Console.Write("Введите цену единицы продукции: ");
            double price = double.Parse(Console.ReadLine().Trim(), _cultureInfo);
            // создаём объект и добавляем в таблицу
            Product product = new Product();
            product.Key = key;
            product.FactoryKey = factoryKey;
            product.Name  = name;
            product.Weight = weight;
            product.ExpiryDate =date ;
            product.Count = count;
            product.Price = price;
            products.Add(product);
        }

        // Удалить изделие, выпускаемое хлебозаводом
        static void RemoveProduct()
        {
            // выводим на экран список заводов изделий
            // и запрашиваем у пользователя номер
            int key = FactoryProductsList(true);
            // находим изделие с таким номером (ключом)
            var item = products.SingleOrDefault(p => p.Key == key);
            // если нашли - удаляем
            if (item != null)
            {
                products.Remove(item);
            }
        }

        // Загрузка списка из файла
        static void ReadList(int index, int fieldCount)
        {
            if(index >= paths.Length)
                return;
            // открываем файл на чтение
            StreamReader stream = new StreamReader(paths[index], Encoding.Default);
            string line;
            int key = 1;
            switch(index)
            {
                case 0:
                    // читаем построчно файл со списком хлебозаводов
                    while ((line = stream.ReadLine()) != null)
                    {
                        string[] fields = line.Split(';');
                        if(fields.Length == fieldCount)
                        {
                            Factory item = new Factory();
                            item.Key = key++;
                            item.Name = fields[0];
                            item.ProdDate = DateTime.Parse(fields[1]);
                            factories.Add(item);
                        }
                    }
                    break;
                case 1:
                    // читаем построчно файл со списком изделий
                    while ((line = stream.ReadLine()) != null)
                    {
                        string[] fields = line.Split(';');
                        if (fields.Length == fieldCount)
                        {
                            Product item = new Product();
                            item.Key = key++;
                            item.FactoryKey = Convert.ToInt32(fields[0]);
                            item.Name = fields[1];
                            item.Weight = double.Parse(fields[2], _cultureInfo);
                            item.ExpiryDate = DateTime.Parse(fields[3]);
                            item.Count = Convert.ToInt32(fields[4]);
                            item.Price = double.Parse(fields[5], _cultureInfo);
                            products.Add(item);
                        }
                    }
                    break;
                case 2:
                    // читаем построчно файл со списком иигредиентов
                    while ((line = stream.ReadLine()) != null)
                    {
                        string[] fields = line.Split(';');
                        if (fields.Length == fieldCount)
                        {
                            Ingredient item = new Ingredient();
                            item.Key = key++;
                            item.Name = fields[0];
                            item.Weight = double.Parse(fields[1], _cultureInfo);
                            item.DeliveryDate = DateTime.Parse(fields[2]);
                            item.ExpiryDate = DateTime.Parse(fields[3]);
                            ingredients.Add(item);
                        }
                    }
                    break;
                case 3:
                    // читаем построчно файл со списком связей
                    while ((line = stream.ReadLine()) != null)
                    {
                        string[] fields = line.Split(';');
                        if (fields.Length == fieldCount)
                        {
                            Constitution item = new Constitution();
                            item.Key = key++;
                            item.ProductKey = Convert.ToInt32(fields[0]);
                            item.IngredientKey = Convert.ToInt32(fields[1]);
                            consists.Add(item);
                        }
                    }
                    break;
            } // switch
            Console.WriteLine(paths[index]);
        }
    }
}
