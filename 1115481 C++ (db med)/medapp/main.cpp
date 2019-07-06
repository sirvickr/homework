#include "pch.h"

#include "Product.h"
#include "Client.h"
#include "Supplier.h"
#include "Sale.h"
#include "LinkProdSupp.h"

using namespace std;

enum class Action : int { add, edit, del };

// Строковые константы, содержащие имена файлов данных
static const char* productsFileName = "db/products";
static const char* clientsFileName = "db/clients";
static const char* suppliersFileName = "db/suppliers";
static const char* salesFileName = "db/sales";
static const char* linksFileName = "db/links";
static const char* saleDetailsFileName = "db/details";

// Фабрика для создания объектов.
// Ключу, т.е. имени файла, здесь соответствует
// значение - функция создания конкретного объекта.
// Для экононмии места и большей выразительности,
// функции здесь заданы в виде лямбда-выражений, 
// т.е. выражений вида 
// [](параметры_функции) -> возвращаемое_значение { 
//		тело_функции 
// }
map<string, function<DbItem* ()>> factory = {
	{ productsFileName, []() -> DbItem* { 
		return new Product(); 
	} },
	{ clientsFileName, []() -> DbItem* { 
		return new Client(); 
	} },
	{ suppliersFileName, []() -> DbItem* { 
		return new Supplier();
	} },
	{ salesFileName, []() -> DbItem* { 
		return new Sale(); 
	} },
	{ linksFileName, []() -> DbItem* { 
		return new LinkProdSupp(); 
	} },
	{ saleDetailsFileName, []() -> DbItem* { 
		return new Details(); 
	} },
};

list<DbItem*> products;
list<DbItem*> clients;
list<DbItem*> suppliers;
list<DbItem*> sales;
list<DbItem*> links;
list<DbItem*> saleDetails;

int maxId(const list<DbItem*>& items) {
	int result = 0;
	for (auto item : items) {
		if (auto unique = dynamic_cast<UniqueItem*>(item)) {
			if (result < unique->Id()) {
				result = unique->Id();
			}
		}
	}
	return result;
}

void deleteById(list<DbItem*>& items, int id) {
	items.remove_if([id](const DbItem* item) {
		if (auto unique = dynamic_cast<const UniqueItem*>(item)) {
			if (unique->Id() == id)
				return true;
			return false;
		}
	});
}

UniqueItem* findById(const list<DbItem*>& items, int id) {
	auto it = find_if(begin(items), end(items), [id](const DbItem* item) {
		if (auto unique = dynamic_cast<const UniqueItem*>(item)) {
			if (unique->Id() == id)
				return true;
			return false;
		}
	});
	if (it == end(items))
		return nullptr;
	return dynamic_cast<UniqueItem*>(*it);
}

void clearList(list<DbItem*>& items) {
	for(auto item : items) {
		if(item) {
			delete item;
			item = nullptr;
		}
	}
	items.clear();
}

int printList(std::ostream& stream, const list<DbItem*>& items, bool reqId = false, const char* title = nullptr) {
	int result = 0;
	if(title)
		stream << title << endl;
	for(const auto& item: items) {
		item->serialize(stream);
		stream << endl;
	}
	if (reqId) {
		cout << "Input id: ";
		cin >> result;
	}
	return result;
}

void prinDetails(int idSale, const char* title = nullptr) {
	if (title)
		cout << title << endl;
	for (const auto& item : saleDetails) {
		auto detail = dynamic_cast<Details*>(item);
		if (!detail) {
			cerr << "Integrity error: wrong details subclass" << endl;
			continue;
		}
		if (detail->IdSale() == idSale) {
			auto product = static_cast<Product*>(findById(products, detail->IdProduct()));
			if (!product) {
				cerr << "Integrity error: product id " << detail->IdProduct() << endl;
				continue;
			}
			auto client = static_cast<Client*>(findById(clients, detail->IdCustomer()));
			if (!product) {
				cerr << "Integrity error: client id " << detail->IdCustomer() << endl;
				continue;
			}
			cout << "\t" << client->Name() << ": " << product->Name() << "\t" << product->Price() << "\t" << detail->Count() << endl;
		}
	}
}

void prinSales(const char* title = nullptr) {
	if (title)
		cout << title << endl;
	for (const auto& item : sales) {
		auto sale = dynamic_cast<Sale*>(item);
		if (!sale) {
			cerr << "Integrity error: wrong sale subclass" << endl;
			continue;
		}
		cout << sale->Date() << endl;
		prinDetails(sale->Id());
	}
}

void saveList(const list<DbItem*>& items, const char* fileName) {
	ofstream file(fileName);
	if(!file) {
		cerr << "failed to open file " << fileName << endl;
		return;
	}
	printList(file, items);
}

void loadList(list<DbItem*>& items, const char* fileName) {
	ifstream file(fileName);
	if (!file) {
		cerr << "failed to open file " << fileName << endl;
		return;
	}
	while (true) {
		DbItem* item = factory[fileName]();
		if (item->deserialize(file)) {
			items.push_back(item);
		}
		else {
			delete item;
			break;
		}
	}
}

void loadLists() {
    loadList(products, productsFileName);
    loadList(clients, clientsFileName);
    loadList(suppliers, suppliersFileName);
    loadList(sales, salesFileName);
    loadList(links, linksFileName);
    loadList(saleDetails, saleDetailsFileName);
}

void saveLists() {
    saveList(products, productsFileName);
    saveList(clients, clientsFileName);
    saveList(suppliers, suppliersFileName);
    saveList(sales, salesFileName);
    saveList(links, linksFileName);
    saveList(saleDetails, saleDetailsFileName);
}

Action getAction() {
	int action = 0;
	do {
		cout << "Input action:" << endl;
		cout << "[1] Add" << endl;
		cout << "[2] Edit" << endl;
		cout << "[3] Delete" << endl;
		cout << "action: ";
		cin >> action;
	} while(action < 1 || action > 3);
	return static_cast<Action>(action - 1);
}

bool getConfirmation(const char* prompt) {
	int input = 0;
	do {
		cout << prompt << endl;
		cout << "[0] No" << endl;
		cout << "[1] Yes" << endl;
		cout << "Your choice: ";
		cin >> input;
	} while (input < 0 || input > 1);
	return static_cast<bool>(input);
}

DbItem* reqNamedItem(const list<DbItem*>& items, const char* fileName, NamedItem* item = nullptr) {
	string name;
	cout << "input name: ";
	cin >> name;
	if (item) {
		item->Name(name);
		return item;
	}
	int id = maxId(items) + 1;
	item = dynamic_cast<NamedItem*>(factory[fileName]());
	if (item) {
		item->Id(id);
		item->Name(name);
		return item;
	}
	return item;
}

Product* reqProduct(Product* item = nullptr) {
	string name;
	double price = 0.0;
	cout << "input name: ";
	cin >> name;
	cout << "input price: ";
	cin >> price;
	if (item) {
		item->Name(name);
		item->Price(price);
		return item;
	}
	int id = maxId(products) + 1;
	return new Product(id, name, price);
}

void addNamedItem(list<DbItem*>& items, const char* fileName) {
	auto newItem = reqNamedItem(items, fileName);
	if (newItem) {

	}
	items.push_back(newItem);
}

void addProduct() {
	Product* newItem = reqProduct();
	products.push_back(newItem);
}

void editNamedItem(list<DbItem*>& items, const char* fileName) {
	int id = printList(cout, items, true, fileName);
	NamedItem* item = dynamic_cast<NamedItem*>(findById(items, id));
	if (item) {
		reqNamedItem(items, fileName, item);
	}
}

void editProduct() {
	int id = printList(cout, products, true, productsFileName);
	Product* item = dynamic_cast<Product*>(findById(products, id));
	if (item) {
		reqProduct(item);
	}
}

void delNamedItem(list<DbItem*>& items, const char* fileName) {
	int id = printList(cout, items, true, fileName);
	deleteById(items, id);
}

void delProduct() {
	int id = printList(cout, products, true, productsFileName);
	deleteById(products, id);
}

void addSale() {
	string date;
	cout << "input date: ";
	cin >> date;

	int id = maxId(sales) + 1;
	Sale* sale = dynamic_cast<Sale*>(factory[salesFileName]());
	if (sale) {
		sale->Id(id);
		sale->Date(date);
		sales.push_back(sale);
	}
	do {
		int idProduct = printList(cout, products, true, "Select product:");
		int idCustomer = printList(cout, clients, true, "Select client:");
		int count = 0;
		cout << "input count: ";
		cin >> count;

		Details* detail = dynamic_cast<Details*>(factory[saleDetailsFileName]());
		if (detail) {
			detail->IdSale(id);
			detail->IdCustomer(idCustomer);
			detail->IdProduct(idProduct);
			detail->Count(count);
			saleDetails.push_back(detail);
		}
	} while (getConfirmation("Continue?"));
}

void namedItemAction(list<DbItem*>& items, const char* fileName) {
	size_t index = 0;
	Action action = getAction();
	switch (action) {
	case Action::add:
		addNamedItem(items, fileName);
		break;
	case Action::edit:
		editNamedItem(items, fileName);
		break;
	case Action::del:
		delNamedItem(items, fileName);
		break;
	}
}

void productsAction() {
	size_t index = 0;
	Action action = getAction();
	switch (action) {
	case Action::add:
		addProduct();
		break;
	case Action::edit:
		editProduct();
		break;
	case Action::del:
		delProduct();
		break;
	}
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");
	loadLists();
	
	int counter = 0;
    int menu = -1;
    while (menu != 0) {
        cout << endl << endl << "Main menu:" << endl;
        cout << "[0] Exit" << endl;
        cout << "[1] Products" << endl;
        cout << "[2] Clients" << endl;
        cout << "[3] Suppliers" << endl;
        cout << "[4] Sales" << endl;
        cout << "[5] Actions with products" << endl;
        cout << "[6] Actions with clients" << endl;
        cout << "[7] Actions with suppliers" << endl;
        cout << "[8] Add sale record" << endl;

        cout  << endl;

        cout << "Ваш выбор: ";
        cin.sync();
        cin >> menu;

        switch(menu) {
        case 1: // Продукция
            printList(cout, products, false, "Products");
        	break;
        case 2: // Клиенты
            printList(cout, clients, false, "Clients");
        	break;
        case 3: // Поставщики
            printList(cout, suppliers, false, "Suppliers");
        	break;
        case 4: // Продажи
			prinSales("Sales");
        	break;
        case 5: // Действия с продукцией
			productsAction();
        	break;
        case 6: // Действия с клиентами
			namedItemAction(clients, clientsFileName);
        	break;
        case 7: // Действия с поставщиками
			namedItemAction(suppliers, suppliersFileName);
			break;
        case 8: // Добавить продажу
			addSale();
        	break;
        }
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
        //break; // for debug
    }
    
    saveLists();
    
    clearList(products);
    clearList(clients);
    clearList(suppliers);
    clearList(sales);
    clearList(links);
    clearList(saleDetails);
    
	return 0;
}

