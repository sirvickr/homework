#include "mainwindow.h"

#include <QVector>
#include <QDebug>

static QVector<Invention> samples = {
	{ Invention::Realm::Physics,   "Костыль", 2020, "Левша, Правша, Косолапый мишка", false, false, QDate::currentDate() },
	{ Invention::Realm::Maths,     "Велосипед", 2015, "Билл Гейтс, Илон Маск, Дмитрий Рогозин", false, true, QDate(2020, 11, 25) },
	{ Invention::Realm::Biology,   "Спутник-V", 2015, "Центр им. Гамалеи", false, true, QDate(2020, 12, 15) },
	{ Invention::Realm::Physics,   "Открытие спонтанной радиоактивности", 1903, "Антуан Анри Беккерель", true, false, QDate::currentDate() },
	{ Invention::Realm::Physics,   "Создание квантовой механики", 1932, "Вернер Карл Гейзенберг", true, false, QDate::currentDate() },
	{ Invention::Realm::Physics,   "Открытие нейтрона", 1935, "Джеймс Чедвик", true, false, QDate::currentDate() },
	{ Invention::Realm::Physics,   "Открытие космических лучей", 1936, "Виктор Франц Гесс", true, false, QDate::currentDate() },
	{ Invention::Realm::Physics,   "Открытие позитрона", 1935, "Карл Дейвид Андерсон", true, false, QDate::currentDate() },
	{ Invention::Realm::Chemistry, "Открытие квазикристаллов", 2011, "Дан Шехтман", true, false, QDate::currentDate() },
	{ Invention::Realm::Chemistry, "Открытие фуллеренов", 1996, "Роберт Кёрл, Харолд Крото, Ричард Смелли", true, false, QDate::currentDate() },
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	qDebug() << "enter ctor";

	connect(ui.lstInventions, SIGNAL(currentRowChanged(int)), this, SLOT(showItem(int)));

	connect(ui.lstAutors, SIGNAL(currentRowChanged(int)), this, SLOT(showAuthor(int)));

	qDebug() << "fill realm list";
	// заполнение перечня возможных значений области применения
	for(int i = 0; i < Invention::Realm::enum_size; i++) {
		ui.cbxRealm->addItem(Invention::realmName(static_cast<Invention::Realm>(i)));
	}
	ui.lstInventions->setSortingEnabled(true);

	showItems();
	qDebug() << "leave ctor";
}

MainWindow::~MainWindow()
{
}

Invention MainWindow::viewToObject()
{
	return {
		static_cast<Invention::Realm>(ui.cbxRealm->currentIndex()),
		ui.txtName->text(),
		ui.txtYear->text().toInt(),
		getAuthors(),
		ui.chkAward->checkState() == Qt::CheckState::Checked,
		ui.chkPatent->checkState() == Qt::CheckState::Checked,
		ui.datPatentReg->date()
	};
}

void MainWindow::objectToView(const Invention& invention)
{
	qDebug() << "enter objectToView: " << invention.name();
	setRealmWidgets(invention.realm());
	ui.txtName->setText(invention.name());
	ui.txtYear->setText(QString::number(invention.year()));
	ui.chkAward->setCheckState(invention.award() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	ui.chkPatent->setCheckState(invention.patent() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
//	setPatentWidgets(invention.patent());
	putAuthors(invention.authors());
	qDebug() << "leave objectToView: " << invention.name();
}

int MainWindow::keyToIndex(int key)
{
	int index = -1;
	for(int i = 0; i < ui.lstInventions->count(); ++i) {
		QListWidgetItem* item = ui.lstInventions->item(i);
		if(key == item->data(Qt::UserRole).toInt()) {
			index = i;
			break;
		}
	}
	return index;
}

bool MainWindow::verifyView()
{
	bool valid = verifyRealmValue() && verifyNameValue() && verifyYearValue();

	/*ui.btnAdd->setEnabled(valid);
	ui.btnSave->setEnabled(valid);

	ui.btnDelete->setEnabled(ui.lstInventions->count() > 0);*/

	auto item = inventions.find(currentItem);
	if(item != inventions.end()) {
		ui.btnSave->setEnabled(buffer != item.value());
	}
//	qDebug() << "  eq " << eq;
//	qDebug() << "buffer " << buffer.realm() << " " << buffer.name();
//	qDebug() << "item   " << item.value().realm() << " " << item.value().name();

	return valid;
}

bool MainWindow::verifyRealmValue()
{	// не "Наизвестная область"
	bool valid = ui.cbxRealm->currentIndex() > 0;
	if(!valid)
		qDebug() << "Выберите область применения";
	return valid;
}

bool MainWindow::verifyNameValue()
{	// не пустое название
	bool valid = ui.txtName->text().trimmed().size() > 0;
	if(!valid)
		qDebug() << "Название должно быть непустым";
	return valid;
}

bool MainWindow::verifyYearValue()
{
	bool valid = ui.txtYear->text().toInt() >= 1900;
	if(!valid)
		qDebug() << "Недопустимое значение года";
	return valid;
}

void MainWindow::setPatentWidgets(bool patent)
{
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	ui.lblPatentReg->setVisible(patent);
	ui.datPatentReg->setVisible(patent);
	if(patent) {
		//ui.chkPatent->setCheckState(Qt::CheckState::Checked);
		ui.datPatentReg->setDate(item.value().patentDate());
	} else {
		//ui.chkPatent->setCheckState(Qt::CheckState::Unchecked);
	}
}

void MainWindow::setRealmWidgets(Invention::Realm realm)
{
	ui.cbxRealm->setCurrentIndex(realm);
	ui.chkAward->setEnabled(realm != Invention::Realm::Maths);
}

void MainWindow::putAuthors(const QString& authors)
{
	ui.lstAutors->clear();
	ui.txtAuthor->clear();
	QStringList list = authors.split(',', Qt::SkipEmptyParts);
	foreach (const QString& item, list) {
		new QListWidgetItem(item.trimmed(), ui.lstAutors);
	}
	if(list.size() > 0) {
		ui.lstAutors->setCurrentRow(0);
	}
}

QString MainWindow::getAuthors()
{
	QString result;
	for(int i = 0; i < ui.lstAutors->count(); ++i) {
		QListWidgetItem* item = ui.lstAutors->item(i);
		if(i)
			result += ", ";
		result += item->text();
	}
	return result;
}

void MainWindow::showItems(int current, bool isKey)
{
	qDebug() << "enter showItems: current " << current << " isKey " << isKey;
	ui.lstInventions->clear();
	qDebug() << "fill the list";
	for(auto entry = inventions.cbegin(); entry != inventions.cend(); entry++) {
		const Invention& invention = entry.value();
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(Invention::realmName(invention.realm()) + "\t" + QString::number(invention.year()) + "\t" + invention.name());
		item->setData(Qt::UserRole, entry.key());
		ui.lstInventions->addItem(item);
	}
	if(isKey) {
		current = keyToIndex(current);
		qDebug() << ">> index = " << current;
	}
	if(current >= 0) {
		selectCurrentInvention(current);
	}
	verifyView();
	qDebug() << "leave showItems";
}

void MainWindow::showItem(int index)
{
	if(index < 0)
		return;
	currentItem = ui.lstInventions->item(index)->data(Qt::UserRole).toInt();
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	buffer = item.value();
	qDebug() << ">>buffer: " << buffer.realm() << " " << buffer.name();
	objectToView(item.value());
}

void MainWindow::showAuthor(int index)
{
	currentAuthor = index;
	if(currentAuthor < 0) {
		ui.txtAuthor->setEnabled(false);
		ui.txtAuthor->clear();
	} else {
		ui.txtAuthor->setText(ui.lstAutors->item(currentAuthor)->text());
		ui.txtAuthor->setEnabled(true);
	}
//	verifyView();
}

int MainWindow::selectCurrent(QListWidget* lst, int& index) {
	int key = -1;
	int count = lst->count();
	qDebug() << "enter selectCurrent: index " << index << " count " << count;
	if(count > 0) {
		if(index == count)
			index--;
		lst->setCurrentRow(index);
		key = ui.lstInventions->item(index)->data(Qt::UserRole).toInt();
	} else {
		index = -1;
	}
	qDebug() << "leave selectCurrent: index " << index << " key " << key;
	return key;
}

void MainWindow::selectCurrentInvention(int& index)
{
	int key = selectCurrent(ui.lstInventions, index);
	auto item = inventions.find(key);
	if(item == inventions.end()) {
		qDebug() << "selectCurrentInvention failed: index " << index << " key " << key;
		return;
	}
}

void MainWindow::on_btnFill_clicked()
{
	qDebug() << "enter btnFill_clicked: nextID " << nextID;
	for (const auto& sample : samples) {
		inventions[nextID++] = sample;
	}
	showItems();
	qDebug() << "leave btnFill_clicked: nextID " << nextID;
}

void MainWindow::on_btnAdd_clicked()
{
	qDebug() << "enter btnAdd_clicked: nextID " << nextID;
	currentItem = nextID++;
	inventions[currentItem] = viewToObject();
	showItems(currentItem, true);
	qDebug() << "leave btnAdd_clicked: nextID " << nextID;
}

void MainWindow::on_btnSave_clicked()
{
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	auto obj = viewToObject();
	if(buffer != obj) {
		qDebug() << "############################################################################################";
	}
	item.value() = viewToObject();
	showItems(currentItem, true);
}

void MainWindow::on_btnDelete_clicked()
{
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	buffer = Invention();
	int index = keyToIndex(currentItem);
	inventions.remove(currentItem);
	showItems(index, false);
}

void MainWindow::on_cbxRealm_currentIndexChanged(int index)
{
	qDebug() << "cbxRealm_currentIndexChange: " << index;
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;

	buffer.realm(static_cast<Invention::Realm>(ui.cbxRealm->currentIndex()));
	verifyView();
}

void MainWindow::on_txtName_editingFinished()
{
	qDebug() << "txtName_editingFinished: " << ui.txtName->text();
	buffer.name(ui.txtName->text());
	verifyView();
}

void MainWindow::on_txtYear_editingFinished()
{
	buffer.year(ui.txtYear->text().toInt());
	verifyView();
}

void MainWindow::on_chkAward_toggled(bool checked)
{
	buffer.award(checked);
	verifyView();
}

void MainWindow::on_chkPatent_toggled(bool checked)
{
	buffer.patent(checked);
	verifyView();
	setPatentWidgets(checked);
}

void MainWindow::on_datPatentReg_userDateChanged(const QDate &date)
{
	/*auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	item.value().patentDate(date);*/
}
#if 0
void MainWindow::on_txtAuthor_editingFinished()
{
	verifyView();
	/*auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	if(currentAuthor < 0)
		return;
	QString author = ui.txtAuthor->text();
	if(author.size() > 0) {
		ui.lstAutors->item(currentAuthor)->setText(ui.txtAuthor->text());
	} else {
		delAuthorItem();
	}
*/
}
#endif
void MainWindow::on_btnAddAuthor_clicked()
{
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	QString author = ui.txtAuthor->text().trimmed();
	if(author.size() > 0) {
		new QListWidgetItem(author, ui.lstAutors);
		buffer.authors(getAuthors());
		verifyView();
	}
}

void MainWindow::on_btnSaveAuthor_clicked()
{
	if(currentAuthor < 0)
		return;
	QString author = ui.txtAuthor->text().trimmed();
	if(author.size() > 0) {
		ui.lstAutors->item(currentAuthor)->setText(ui.txtAuthor->text());
		buffer.authors(getAuthors());
		verifyView();
	}
}

void MainWindow::on_btnDelAuthor_clicked()
{
	if(currentAuthor < 0)
		return;
	delete ui.lstAutors->takeItem(currentAuthor);
	selectCurrent(ui.lstAutors, currentAuthor);
	ui.txtAuthor->clear();
	buffer.authors(getAuthors());
	verifyView();
}
