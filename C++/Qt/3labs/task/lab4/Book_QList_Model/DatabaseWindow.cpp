#include "DatabaseWindow.h"

DatabaseWindow::DatabaseWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	// "Обнуляем" базу данных
	//Db.clear();

	// Задаем начальные значения виджетов
	setStartStatus();

   // События кнопки "Добавить": нажатие
	connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addBook()));

   // События браузера: изменение текущей строки
	connect(ui.booksList, SIGNAL( currentRowChanged (int ) ), this, SLOT(showBook(int)));

	// Загружаем книги
	Db.load("MyFile.db");

	// Отображаем книги
	QVector<BookItem> bookItems;
	QVector<BookItem>::const_iterator i;
	ui.booksList->clear();
	bookItems = Db.records();
	for(i =bookItems.constBegin(); i != bookItems.constEnd(); ++i)
	{
		QListWidgetItem *bookItem = new QListWidgetItem;				// создаем элемент спсика
		bookItem->setText( i->name + QString("\t") + i->author );	// записываем в него отображаему строку
		bookItem->setData(Qt::UserRole, i->id);							// записываем в него идентификатор
		ui.booksList->insertItem(current, bookItem);						// используем ВСТАВКУ элемента списка
	}

	// Задаем первую строку браузера текущей
	ui.booksList->setCurrentRow(0); 
}

DatabaseWindow::~DatabaseWindow()
{
	// Сохраняем книги
	Db.save("MyFile.db");
}

// Задать стартовое состояние виджетов
void DatabaseWindow::setStartStatus()
{
	ui.nameEdit->setText("");
	ui.authorEdit->setText("");
	ui.genreEdit->setCurrentIndex(0);
	ui.yearEdit->setValue(2000);
}

// Добавить новую книгу
void DatabaseWindow::addBook()
{
	Book newBook;	// новая книга 

	// Не создаем книгу, если не заданы ее поля
	if(ui.nameEdit->text().isEmpty() || ui.authorEdit->text().isEmpty())	return;

	// "Создаем" новую книгу на основе полей ввода
	newBook = createCurrentBook();

	// Добавляем новую книгу в список так, чтобы не нарушить режим сортировки
	current = Db.append(newBook);
	
	// Добавляем книгу в браузер
	QListWidgetItem *bookItem = new QListWidgetItem;							// создаем элемент спсика
	bookItem->setText( newBook.name + QString("\t") + newBook.author );	// записываем в него отображаему строку
	bookItem->setData(Qt::UserRole, newBook.id);									// записываем в него идентификатор
	ui.booksList->insertItem(current, bookItem);									// используем ВСТАВКУ элемента списка

	// Выделяем добавленную книгу в браузере
	ui.booksList->setCurrentRow(current);

	// Обновляем активность кнопок
	if (Db.count() == 1)
	{
		ui.btnUpdate->setEnabled(true);
		ui.btnDelete->setEnabled(true);
	}
}

// Создаем книгу на основании текущих значений полей ввода
Book DatabaseWindow::createCurrentBook()
{
	Book book;	// создаваемая книга 

	book.name =		ui.nameEdit->text();
   book.author =	ui.authorEdit->text();
	book.genre =	ui.genreEdit->currentIndex();
	book.year =		ui.yearEdit->value();	

	return book;
}

// Отобразить книгу в заданной строке. ВНИМАНИЕ!!!! Упрощенный вариант слота
void DatabaseWindow::showBook(int currentRow)
{
	const Book *book;	// отображаемая книга

	// Находим индекс книги в списке по ее идентификатору
	book = Db.record(ui.booksList->item(currentRow)->data(Qt::UserRole).toUInt());

	// Отображаем содержимое книги в полях
	ui.nameEdit->setText(book->name);
	ui.authorEdit->setText(book->author);
	ui.genreEdit->setCurrentIndex(book->genre);
	ui.yearEdit->setValue(book->year);
}