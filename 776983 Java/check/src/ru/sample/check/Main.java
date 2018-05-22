package ru.sample.check;

class Product {
    String name;
    String units = "шт";
    double price = 0.0;
    double quantity = 0.0;
    Product(String n, String u, double p, double q) {
        name = n;
        units = u;
        price = p;
        quantity = q;
    }
}

public class Main {

    public static void main(String[] args) {
        // заполняем для примера массив из 4 товаров
        int prodCount = 4; // количество товаров в корзине
        Product[] products = new Product[prodCount];
        products[0] = new Product("Мука", "кг", 30.0, 2.0);
        products[1] = new Product("Молоко", "л", 50.0, 1.5);
        products[2] = new Product("Сметана", "л", 80.0, 0.4);
        products[3] = new Product("Томаты", "кг", 48.5, 1.8);
        double total = 0; // итоговая стоимость
        System.out.println("Наименование\tЦена\tКол-во\tСумма");
        // печатаем на чеке список товаров и считаем итоговую сумму
        for(int i = 0; i < prodCount; i++) {
            double sum = products[i].price * products[i].quantity;
            System.out.println(products[i].name + "\t\t\t" + products[i].price + " р."
                    + "\t" + products[i].quantity + "\t" + products[i].units + "\t" + sum);
            total += sum;
        }
        System.out.println("Итого\t\t\t" + total + " р.");
        // посчитаем наличные в тысячных купюрах (для примера)
        int cash = (int)(Math.ceil(total / 1000.0) * 1000.0);
        // печатаем наличные и сдачу
        System.out.println("Наличные\t\t" + cash + " р.");
        System.out.println("Сдача\t\t\t" + (cash - total) + " р.");
    }
}
