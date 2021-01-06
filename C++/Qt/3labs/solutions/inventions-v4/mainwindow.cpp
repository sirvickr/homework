#include "mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), filesFilter("Изобретения (*.inv);;Все файлы (*)")
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
	// устанавливаем все возможные биты в Enabled
	int mask = (1 << EnabledBits::bitMax) - 1;
	mask = verifyRealmValue(mask);
	mask = verifyNameValue(mask);
	mask = verifyYearValue(mask);
	mask = verifyBuffer(mask);
	// сбрасываем активность при пустом браузере
	if(ui.lstInventions->count() == 0) {
		mask = resetBit(mask, EnabledBits::bitDeleteInvention);
		mask = resetBit(mask, EnabledBits::bitUpdateInvention);
		mask = resetBit(mask, EnabledBits::bitUpdateAuthor);
		mask = resetBit(mask, EnabledBits::bitDeleteAuthor);
	}
	// сбрасываем активность в зависимости от синхронизации данных с хранилищем
	if(!db.isModified()) {
		mask = resetBit(mask, EnabledBits::bitSaveDatabase);
		mask = resetBit(mask, EnabledBits::bitSaveDatabaseAs);
	}
	// если база не загружена и не сохранена, она и есть новая
	if(fileName.isEmpty()) {
		mask = resetBit(mask, EnabledBits::bitNewDatabase);
		mask = resetBit(mask, EnabledBits::bitSaveDatabase);
	}

	ui.btnAdd->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitCreateInvention)));
	ui.btnUpdate->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitUpdateInvention)));
	ui.btnDelete->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitDeleteInvention)));
	ui.btnAddAuthor->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitCreateAuthor)));
	ui.btnUpdateAuthor->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitUpdateAuthor)));
	ui.btnDelAuthor->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitDeleteAuthor)));
	ui.btnNewDatabase->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitNewDatabase)));
	ui.btnLoadDatabase->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitLoadDatabase)));
	ui.btnSaveDatabase->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitSaveDatabase)));
	ui.btnSaveDatabaseAs->setEnabled(static_cast<bool>(mask & (1 << EnabledBits::bitSaveDatabaseAs)));
}

int MainWindow::verifyRealmValue(int mask)
{	// не "Наизвестная область"
	if(ui.cbxRealm->currentIndex() < 1) {
		mask = resetBit(mask, EnabledBits::bitCreateInvention);
		mask = resetBit(mask, EnabledBits::bitUpdateInvention);
	}
	return mask;
}

int MainWindow::verifyNameValue(int mask)
{	// не пустое название
	if(ui.txtName->text().trimmed().size() == 0) {
		mask = resetBit(mask, EnabledBits::bitCreateInvention);
		mask = resetBit(mask, EnabledBits::bitUpdateInvention);
	}
	return mask;
}

int MainWindow::verifyYearValue(int mask)
{
	if(ui.txtYear->text().toInt() < 1900) {
		mask = resetBit(mask, EnabledBits::bitCreateInvention);
		mask = resetBit(mask, EnabledBits::bitUpdateInvention);
	}
	return mask;
}

int MainWindow::verifyBuffer(int mask)
{	// буфер не изменён
	Invention invention;
	if(db.record(currentItem, invention)) {
		if(buffer == invention) {
			mask = resetBit(mask, EnabledBits::bitCreateInvention);
			mask = resetBit(mask, EnabledBits::bitUpdateInvention);
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
		Invention invention;
		if(db.record(currentItem, invention)) {
			ui.datPatentReg->setDate(invention.patentDate());
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
	ui.lstInventions->clear();
	for(auto entry = db.cbegin(); entry != db.cend(); entry++) {
		const Invention& invention = entry.value();
		QListWidgetItem *item = new InventionItem(invention.rank());
		item->setText(QString::number(entry.key()) + "\t" + Invention::realmName(invention.realm())
					  + "\t" + QString::number(invention.year()) + "\t" + invention.name());
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
	Invention invention;
	if(db.record(currentItem, invention)) {
		buffer = invention;
		objectToView(buffer);
	}
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

void MainWindow::on_btnAdd_clicked()
{
	currentItem = db.append(buffer);
	showItems(currentItem, true);
}

void MainWindow::on_btnUpdate_clicked()
{
	db.update(currentItem, buffer);
	showItems(currentItem, true);
}

void MainWindow::on_btnDelete_clicked()
{
	int index = keyToIndex(currentItem);
	db.remove(currentItem);
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
	QString author = ui.txtAuthor->text().trimmed();
	if(author.size() > 0) {
		new QListWidgetItem(author, ui.lstAutors);
		buffer.authors(getAuthors());
		verifyView();
	}
}

void MainWindow::on_btnUpdateAuthor_clicked()
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

void MainWindow::on_btnNewDatabase_clicked()
{
	db.clear();
	fileName.clear();
	showItems();
}

void MainWindow::on_btnLoadDatabase_clicked()
{
	fileName = QFileDialog::getOpenFileName(
		this,                    // parent
		"Открыть базу данных",   // caption
		"",                      // dir
		filesFilter);            // filter
	db.load(fileName);
	showItems();
}

void MainWindow::on_btnSaveDatabase_clicked()
{
	if(fileName.isEmpty())
		return;
	db.save(fileName);
	verifyView();
}

void MainWindow::on_btnSaveDatabaseAs_clicked()
{
	fileName = QFileDialog::getSaveFileName(
		this,                    // parent
		"Сохранить базу данных", // caption
		"",                      // dir
		filesFilter);            // filter
	db.save(fileName);
	verifyView();
}
