#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.lstInventions, SIGNAL(currentRowChanged(int)), this, SLOT(showItem(int)));

	connect(ui.lstAutors, SIGNAL(currentRowChanged(int)), this, SLOT(showAuthor(int)));

	inventions[0].realm(Invention::Realm::Physics);
	inventions[0].name("Костыль");
	inventions[0].year(2020);
	inventions[0].authors("Левша, Правша, Косолапый мишка");
	inventions[0].award(false);
	inventions[0].patent(false);
	inventions[0].patentDate(QDate::currentDate());

	inventions[1].realm(Invention::Realm::Maths);
	inventions[1].name("Велосипед");
	inventions[1].year(2015);
	inventions[1].authors("Билл Гейтс, Илон Маск, Дмитрий Рогозин");
	inventions[1].award(false);
	inventions[1].patent(true);
	inventions[1].patentDate(QDate(2020, 11, 25));

	for(int i = 0; i < Invention::Realm::enum_size; i++) {
		ui.cbxRealm->addItem(Invention::realmName(static_cast<Invention::Realm>(i)));
	}

	for(int i = 0; i < MaxItemCount; i++) {
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(inventions[i].name() + "\t" + QString::number(inventions[i].year()));
		item->setData(Qt::UserRole, i);
		ui.lstInventions->addItem(item);
	}
}

void MainWindow::setPatentWidgets(bool patent)
{
	if(currentItem < 0)
		return;
	ui.lblPatentReg->setVisible(patent);
	ui.datPatentReg->setVisible(patent);
	if(patent) {
		ui.chkPatent->setCheckState(Qt::CheckState::Checked);
		ui.datPatentReg->setDate(inventions[currentItem].patentDate());
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

void MainWindow::delAuthor()
{
	if(currentAuthor < 0)
		return;
	delete ui.lstAutors->takeItem(currentAuthor);
	int count = ui.lstAutors->count();
	if(count > 0) {
		if(currentAuthor == count)
			currentAuthor--;
		ui.lstAutors->setCurrentRow(currentAuthor);
	} else {
		currentAuthor = -1;
	}
}

void MainWindow::showItem(int index)
{
	currentItem = index;
	if(currentItem < 0)
		return;
	const Invention &invention = inventions[ui.lstInventions->item(currentItem)->data(Qt::UserRole).toInt()];
	setRealmWidgets(invention.realm());
	ui.txtName->setText(invention.name());
	ui.txtYear->setText(QString::number(invention.year()));
	ui.chkAward->setCheckState(invention.award() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	ui.chkPatent->setCheckState(invention.patent() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	setPatentWidgets(invention.patent());
	putAuthors(invention.authors());
}

void MainWindow::showAuthor(int index)
{
	if(currentItem < 0)
		return;
	currentAuthor = index;
	if(currentAuthor < 0) {
		ui.txtAuthor->setEnabled(false);
	} else {
		ui.txtAuthor->setText(ui.lstAutors->item(currentAuthor)->text());
		ui.txtAuthor->setEnabled(true);
	}
}

void MainWindow::on_cbxRealm_currentIndexChanged(int index)
{
	if(currentItem < 0)
		return;
	if(checkIndex(index)) {
		inventions[currentItem].realm(static_cast<Invention::Realm>(index));
	}
}

void MainWindow::on_txtName_editingFinished()
{
	if(currentItem < 0)
		return;
	inventions[currentItem].name(ui.txtName->text());
}

void MainWindow::on_chkPatent_toggled(bool checked)
{
	if(currentItem < 0)
		return;
	inventions[currentItem].patent(checked);
	setPatentWidgets(checked);
}

void MainWindow::on_chkAward_toggled(bool checked)
{
	if(currentItem < 0)
		return;
	inventions[currentItem].award(checked);
}

void MainWindow::on_datPatentReg_userDateChanged(const QDate &date)
{
	if(currentItem < 0)
		return;
	inventions[currentItem].patentDate(date);
}

void MainWindow::on_txtAuthor_editingFinished()
{
	if(currentItem < 0)
		return;
	if(currentAuthor < 0)
		return;
	QString author = ui.txtAuthor->text();
	if(author.size() > 0) {
		ui.lstAutors->item(currentAuthor)->setText(ui.txtAuthor->text());
	} else {
		delAuthor();
	}
	inventions[currentItem].authors(getAuthors());
}

void MainWindow::on_btnDelAuthor_clicked()
{
	if(currentItem < 0)
		return;
	ui.txtAuthor->clear();
	delAuthor();
	inventions[currentItem].authors(getAuthors());
}
