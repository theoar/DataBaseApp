#include "tabwidget.h"
#include "ui_tabwidget.h"

TabWidget::TabWidget(BaseDialog *Dial, QSqlQueryModel * M, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabWidget),
	Dialog(Dial),
	Model(M)
{    
	ui->setupUi(this);

    auto Ss = ui->View->selectionModel();
    auto Mm = ui->View->model();
	ui->View->setModel(Model);

    Ss->deleteLater();
    Mm->deleteLater();

	if (Dialog) Dialog->setParent(this, Qt::Dialog);

	ui->FilterBox->addItem(tr("All fields"), -1);

	for(int i = 0; i < Model->columnCount(); ++i)
		ui->FilterBox->addItem(Model->headerData(i, Qt::Horizontal).toString(), i);

	if (Dialog) connect(Dialog, &BaseDialog::accepted, this, &TabWidget::onDialogAccepted);

	connect(Model, &QSqlQueryModel::dataChanged, this, &TabWidget::onSearchChanged);
	connect(Model, &QSqlQueryModel::layoutChanged, this, &TabWidget::onSearchChanged);
	connect(ui->View->horizontalHeader(), &QHeaderView::sectionClicked, this, &TabWidget::onSearchChanged);
}

TabWidget::~TabWidget()
{
	delete ui;
}

void TabWidget::setReadonly(bool Readonly)
{
	if (Readonly)
		ui->View->setEditTriggers(0);
	else
		ui->View->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}

void TabWidget::setDeleteable(bool Deleteable)
{
	ui->DeleteButton->setVisible(Deleteable);
}

void TabWidget::setDelegate(QItemDelegate* Delegate)
{
	ui->View->setItemDelegate(Delegate);
}

void TabWidget::insertWidget(QWidget* W, int Pos)
{
	ui->horizontalLayout->insertWidget(Pos, W);
}

void TabWidget::hideColumns(QList<int> Hidden)
{
	ui->FilterBox->clear(); ui->FilterBox->addItem(tr("All fields"), -1);

	for (int i = 0; i < Model->columnCount(); ++i)
	{
		if (Hidden.contains(i)) ui->View->hideColumn(i);
		else ui->FilterBox->addItem(Model->headerData(i, Qt::Horizontal).toString(), i);
	}
}

void TabWidget::onAddButton()
{
	if (Dialog) Dialog->open();
}

void TabWidget::onDeleteButton()
{
	auto Index  = ui->View->selectionModel()->currentIndex();
	if(Index.isValid())
	{
		QMessageBox::StandardButton Reply = QMessageBox::question(this, tr("Delete record"), tr("Do you want to delete selected row"), QMessageBox::Yes|QMessageBox::No);

		if(Reply == QMessageBox::StandardButton::Yes)
		{
			Model->removeRow(Index.row());

			ui->View->setCurrentIndex(QModelIndex());
		}
	}
	else
	{
		QMessageBox::information(this, tr("Error"), tr("You didn't selected any item"), QMessageBox::Ok);
	}
}

void TabWidget::onDialogAccepted(QMap<QString, QVariant> Vect)
{
	if (auto M = dynamic_cast<QSqlTableModel*>(Model))
	{
		QSqlRecord Record;

		for(auto i = Vect.constBegin(); i != Vect.constEnd(); ++i)
		{
			Record.append(QSqlField(i.key(), i.value().type()));
			Record.setValue(i.key(), i.value());
		}

		M->insertRecord(-1, Record);
	}
}

void TabWidget::onSearchChanged()
{
	const QString Text = ui->SearchField->text();
	const int Index = ui->FilterBox->currentData().toInt();

	for (int i = 0; i < Model->rowCount(); ++i)
	{
		bool Hide = true;

		if (Index == -1) for (int j = 0; j < Model->columnCount(); ++j)
		{
			if (Model->data(Model->index(i, j)).toString().contains(Text, Qt::CaseInsensitive)) Hide = false;
		}
		else Hide = !Model->data(Model->index(i, Index)).toString().contains(Text, Qt::CaseInsensitive);

		ui->View->setRowHidden(i, Hide);
	}
}
