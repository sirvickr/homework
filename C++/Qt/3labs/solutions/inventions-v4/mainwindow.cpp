#include "mainwindow.h"

#include <QVector>

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

	connect(ui.lstInventions, SIGNAL(currentRowChanged(int)), this, SLOT(showItem(int)));

	connect(ui.lstAutors, SIGNAL(currentRowChanged(int)), this, SLOT(showAuthor(int)));

	// заполнение перечня возможных значений области применения
	for(int i = 0; i < Invention::Realm::enum_size; i++) {
		ui.cbxRealm->addItem(Invention::realmName(static_cast<Invention::Realm>(i)));
	}
	ui.lstInventions->setSortingEnabled(true);

	setPatentWidgets(false);

	showItems();
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
	setRealmWidgets(invention.realm());
	ui.txtName->setText(invention.name());
	ui.txtYear->setText(QString::number(invention.year()));
	ui.chkAward->setCheckState(invention.award() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	ui.chkPatent->setCheckState(invention.patent() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	setPatentWidgets(invention.patent());
	putAuthors(invention.authors());
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

void MainWindow::verifyView()
{
	int mask = crudMax();
	mask = verifyRealmValue(mask);
	mask = verifyNameValue(mask);
	mask = verifyYearValue(mask);
	mask = verifyBuffer(mask);
	//
	if(ui.lstInventions->count() == 0) {
		mask = crudResetBit(mask, CRUD::crudDeleteInvention);
		mask = crudResetBit(mask, CRUD::crudUpdateInvention);
		mask = crudResetBit(mask, CRUD::crudUpdateAuthor);
		mask = crudResetBit(mask, CRUD::crudDeleteAuthor);
	}

	ui.btnAdd->setEnabled(static_cast<bool>(mask & (1 << CRUD::crudCreateInvention)));
	ui.btnSave->setEnabled(static_cast<bool>(mask & (1 << CRUD::crudUpdateInvention)));
	ui.btnDelete->setEnabled(static_cast<bool>(mask & (1 << CRUD::crudDeleteInvention)));
	ui.btnAddAuthor->setEnabled(static_cast<bool>(mask & (1 << CRUD::crudCreateAuthor)));
	ui.btnSaveAuthor->setEnabled(static_cast<bool>(mask & (1 << CRUD::crudUpdateAuthor)));
	ui.btnDelAuthor->setEnabled(static_cast<bool>(mask & (1 << CRUD::crudDeleteAuthor)));
}

int MainWindow::verifyRealmValue(int mask)
{	// не "Наизвестная область"
	if(ui.cbxRealm->currentIndex() < 1) {
		mask = crudResetBit(mask, CRUD::crudCreateInvention);
		mask = crudResetBit(mask, CRUD::crudUpdateInvention);
	}
	return mask;
}

int MainWindow::verifyNameValue(int mask)
{	// не пустое название
	if(ui.txtName->text().trimmed().size() == 0) {
		mask = crudResetBit(mask, CRUD::crudCreateInvention);
		mask = crudResetBit(mask, CRUD::crudUpdateInvention);
	}
	return mask;
}

int MainWindow::verifyYearValue(int mask)
{
	if(ui.txtYear->text().toInt() < 1900) {
		mask = crudResetBit(mask, CRUD::crudCreateInvention);
		mask = crudResetBit(mask, CRUD::crudUpdateInvention);
	}
	return mask;
}

int MainWindow::verifyBuffer(int mask)
{	// буфер не изменён
	auto item = inventions.find(currentItem);
	if(item != inventions.end()) {
		if(buffer == item.value()) {
			mask = crudResetBit(mask, CRUD::crudCreateInvention);
			mask = crudResetBit(mask, CRUD::crudUpdateInvention);
		}
	}
	return mask;
}

void MainWindow::setPatentWidgets(bool patent)
{
	ui.lblPatentReg->setVisible(patent);
	ui.datPatentReg->setVisible(patent);
	if(patent) {
		ui.chkPatent->setCheckState(Qt::CheckState::Checked);
		auto item = inventions.find(currentItem);
		if(item != inventions.end()) {
			ui.datPatentReg->setDate(item.value().patentDate());
		}
	} else {
		ui.chkPatent->setCheckState(Qt::CheckState::Unchecked);
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
		new InventionItem(inventions, item.trimmed(), ui.lstAutors);
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
	ui.lstInventions->clear();
	for(auto entry = inventions.cbegin(); entry != inventions.cend(); entry++) {
		const Invention& invention = entry.value();
		QListWidgetItem *item = new InventionItem(inventions);
		item->setText(Invention::realmName(invention.realm()) + "\t" + QString::number(invention.year()) + "\t" + invention.name());
		item->setData(Qt::UserRole, entry.key());
		ui.lstInventions->addItem(item);
	}
	if(isKey) {
		current = keyToIndex(current);
	}
	if(current >= 0) {
		selectCurrent(ui.lstInventions, current);
	}
	verifyView();
}

void MainWindow::showItem(int index)
{
	if(index < 0)
		return;
	currentItem = ui.lstInventions->item(index)->data(Qt::UserRole).toInt();
	buffer = inventions[currentItem];
	objectToView(buffer);
}

void MainWindow::showAuthor(int index)
{
	currentAuthor = index;
	if(currentAuthor < 0) {
		ui.txtAuthor->clear();
	} else {
		ui.txtAuthor->setText(ui.lstAutors->item(currentAuthor)->text());
	}
}

void MainWindow::selectCurrent(QListWidget* lst, int& index) {
	if(index == lst->count())
		index--;
	lst->setCurrentRow(index);
}

void MainWindow::on_btnFill_clicked()
{
	foreach (const auto& sample, samples) {
		inventions[nextID++] = sample;
	}
	showItems();
}

void MainWindow::on_btnAdd_clicked()
{
	currentItem = nextID++;
	inventions[currentItem] = viewToObject();
	showItems(currentItem, true);
}

void MainWindow::on_btnSave_clicked()
{
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	item.value() = viewToObject();
	showItems(currentItem, true);
}

void MainWindow::on_btnDelete_clicked()
{
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	int index = keyToIndex(currentItem);
	inventions.remove(currentItem);
	showItems(index, false);
}

void MainWindow::on_cbxRealm_currentIndexChanged(int index)
{
    if(index < 0)
        return;
    buffer.realm(static_cast<Invention::Realm>(index));
    verifyView();
}

void MainWindow::on_txtName_editingFinished()
{
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
	buffer.patentDate(date);
	verifyView();
}

void MainWindow::on_btnAddAuthor_clicked()
{
	auto item = inventions.find(currentItem);
	if(item == inventions.end())
		return;
	QString author = ui.txtAuthor->text().trimmed();
	if(author.size() > 0) {
		new InventionItem(inventions, author, ui.lstAutors);
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
