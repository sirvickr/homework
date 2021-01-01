#include "DatabaseWindow.h"

DatabaseWindow::DatabaseWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	// "��������" ���� ������
	//Db.clear();

	// ������ ��������� �������� ��������
	setStartStatus();

   // ������� ������ "��������": �������
	connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addBook()));

   // ������� ��������: ��������� ������� ������
	connect(ui.booksList, SIGNAL( currentRowChanged (int ) ), this, SLOT(showBook(int)));

	// ��������� �����
	Db.load("MyFile.db");

	// ���������� �����
	QVector<BookItem> bookItems;
	QVector<BookItem>::const_iterator i;
	ui.booksList->clear();
	bookItems = Db.records();
	for(i =bookItems.constBegin(); i != bookItems.constEnd(); ++i)
	{
		QListWidgetItem *bookItem = new QListWidgetItem;				// ������� ������� ������
		bookItem->setText( i->name + QString("\t") + i->author );	// ���������� � ���� ����������� ������
		bookItem->setData(Qt::UserRole, i->id);							// ���������� � ���� �������������
		ui.booksList->insertItem(current, bookItem);						// ���������� ������� �������� ������
	}

	// ������ ������ ������ �������� �������
	ui.booksList->setCurrentRow(0); 
}

DatabaseWindow::~DatabaseWindow()
{
	// ��������� �����
	Db.save("MyFile.db");
}

// ������ ��������� ��������� ��������
void DatabaseWindow::setStartStatus()
{
	ui.nameEdit->setText("");
	ui.authorEdit->setText("");
	ui.genreEdit->setCurrentIndex(0);
	ui.yearEdit->setValue(2000);
}

// �������� ����� �����
void DatabaseWindow::addBook()
{
	Book newBook;	// ����� ����� 

	// �� ������� �����, ���� �� ������ �� ����
	if(ui.nameEdit->text().isEmpty() || ui.authorEdit->text().isEmpty())	return;

	// "�������" ����� ����� �� ������ ����� �����
	newBook = createCurrentBook();

	// ��������� ����� ����� � ������ ���, ����� �� �������� ����� ����������
	current = Db.append(newBook);
	
	// ��������� ����� � �������
	QListWidgetItem *bookItem = new QListWidgetItem;							// ������� ������� ������
	bookItem->setText( newBook.name + QString("\t") + newBook.author );	// ���������� � ���� ����������� ������
	bookItem->setData(Qt::UserRole, newBook.id);									// ���������� � ���� �������������
	ui.booksList->insertItem(current, bookItem);									// ���������� ������� �������� ������

	// �������� ����������� ����� � ��������
	ui.booksList->setCurrentRow(current);

	// ��������� ���������� ������
	if (Db.count() == 1)
	{
		ui.btnUpdate->setEnabled(true);
		ui.btnDelete->setEnabled(true);
	}
}

// ������� ����� �� ��������� ������� �������� ����� �����
Book DatabaseWindow::createCurrentBook()
{
	Book book;	// ����������� ����� 

	book.name =		ui.nameEdit->text();
   book.author =	ui.authorEdit->text();
	book.genre =	ui.genreEdit->currentIndex();
	book.year =		ui.yearEdit->value();	

	return book;
}

// ���������� ����� � �������� ������. ��������!!!! ���������� ������� �����
void DatabaseWindow::showBook(int currentRow)
{
	const Book *book;	// ������������ �����

	// ������� ������ ����� � ������ �� �� ��������������
	book = Db.record(ui.booksList->item(currentRow)->data(Qt::UserRole).toUInt());

	// ���������� ���������� ����� � �����
	ui.nameEdit->setText(book->name);
	ui.authorEdit->setText(book->author);
	ui.genreEdit->setCurrentIndex(book->genre);
	ui.yearEdit->setValue(book->year);
}