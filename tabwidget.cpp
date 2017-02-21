#include "tabwidget.h"
#include "ui_tabwidget.h"

TabWidget::TabWidget(BaseDialog *Dial, QSqlQueryModel * M, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabWidget),
    Dialog(Dial)
{    
    Model = M;   

	ui->setupUi(this);

    ButtonDistabler.push_back(ui->DeleteButton);
    ui->DeleteButton->setDisabled(true);

    auto Ss = ui->View->selectionModel();
    auto Mm = ui->View->model();

    ui->View->setModel(Model);

    Ss->deleteLater();
    Mm->deleteLater();

    onRefresh();

    if (Dialog)
    {
        Dialog->setParent(this, Qt::Dialog);
        connect(ui->AddButton, &QToolButton::clicked, this, &TabWidget::onAddButton);
        connect(ui->DeleteButton, &QToolButton::clicked, this, &TabWidget::onDeleteButton);
    }
    else
    {
        ui->AddButton->hide();
        ui->DeleteButton->hide();
    }

    ui->View->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->FilterBox->addItem(tr("All fields"), -1);

	for(int i = 0; i < Model->columnCount(); ++i)
		ui->FilterBox->addItem(Model->headerData(i, Qt::Horizontal).toString(), i);

    if (Dialog)
        connect(Dialog, &BaseDialog::accepted, this, &TabWidget::onDialogAccepted);

	connect(Model, &QSqlQueryModel::dataChanged, this, &TabWidget::onSearchChanged);
	connect(Model, &QSqlQueryModel::layoutChanged, this, &TabWidget::onSearchChanged);
	connect(ui->View->horizontalHeader(), &QHeaderView::sectionClicked, this, &TabWidget::onSearchChanged);

    connect(ui->View->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TabWidget::onSelectionChagned);
    connect(ui->View->horizontalHeader(), &QHeaderView::sectionClicked, [this](void)-> void {
        for(QAbstractButton* Button : this->ButtonDistabler)
            Button->setDisabled(true);
        emit propetSelection(false);
    });

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
QSqlQueryModel *TabWidget::getModel() const
{
    return Model;    
}

void TabWidget::setModel(QSqlQueryModel *value)
{
    Model = value;
}

QTableView *TabWidget::getView()
{
    return ui->View;
}

void TabWidget::addPayButton()
{    
    QPushButton* Pay = new QPushButton(tr("Pay"), this);
    ui->horizontalLayout->insertWidget(ui->horizontalLayout->indexOf(ui->AddButton),Pay);
    connect(Pay, &QPushButton::clicked, this, &TabWidget::onPayClicked);
    Pay->setDisabled(true);
    ButtonDistabler.push_back(Pay);
}

void TabWidget::addCompleteButton()
{
    QPushButton* Complete = new QPushButton(tr("Complete order"), this);
    ui->horizontalLayout->insertWidget(ui->horizontalLayout->indexOf(ui->AddButton),Complete);
    connect(Complete, &QPushButton::clicked, this, &TabWidget::onCompleteClicked);
    Complete->setDisabled(true);
    ButtonDistabler.push_back(Complete);
}

void TabWidget::addDetailsButton()
{
    QPushButton* Details = new QPushButton(tr("Details"), this);
    ui->horizontalLayout->insertWidget(ui->horizontalLayout->indexOf(ui->AddButton),Details);
    connect(Details, &QPushButton::clicked, this, &TabWidget::onDetailsClicked);
    Details->setDisabled(true);
    ButtonDistabler.push_back(Details);
}

void TabWidget::addEditButton()
{
    QPushButton* Edit = new QPushButton(tr("Edit"), this);
    ui->horizontalLayout->insertWidget(ui->horizontalLayout->indexOf(ui->AddButton),Edit);
    connect(Edit, &QPushButton::clicked, this, &TabWidget::onEditClicked);
    Edit->setDisabled(true);
    ButtonDistabler.push_back(Edit);
}

void TabWidget::hideSearchBar()
{
    ui->SearchField->hide();
    ui->FilterBox->hide();
}

void TabWidget::hideStandardButtons()
{
    ui->AddButton->hide();
    ui->DeleteButton->hide();    
}

int TabWidget::getCurrentSelectedPK()
{

    auto M = ui->View->model();
    auto Item = M->index(ui->View->currentIndex().row(), 0);
    auto PK = Item.data().toInt();

    if(PK)
        return PK;
    else
        return -1;
}
BaseDialog *TabWidget::getDialog()
{
    return Dialog;
}

void TabWidget::setDialog(BaseDialog *value)
{
    Dialog = value;
}

void TabWidget::onSelectionChagned(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    bool TurnOn = !newSelection.isEmpty();

    for(QAbstractButton * X : ButtonDistabler)
        X->setEnabled(TurnOn);

    emit propetSelection(TurnOn);
}


void TabWidget::onAddButton()
{
    if (Dialog) Dialog->open();
}

void TabWidget::onDeleteButton()
{
    auto Index  = ui->View->selectionModel()->currentIndex();    
    if(!ui->View->selectionModel()->selection().isEmpty())
	{
		QMessageBox::StandardButton Reply = QMessageBox::question(this, tr("Delete record"), tr("Do you want to delete selected row"), QMessageBox::Yes|QMessageBox::No);

		if(Reply == QMessageBox::StandardButton::Yes)
		{
            if( !Model->removeRow(Index.row()) )
                QMessageBox::warning(this, tr("Can not delete record"), tr("This record is in relation with other table"));

			ui->View->setCurrentIndex(QModelIndex());

            onRefresh();
		}
	}
	else
	{
		QMessageBox::information(this, tr("Error"), tr("You didn't selected any item"), QMessageBox::Ok);
	}
}

void TabWidget::onDialogAccepted(QMap<QString, QVariant> Vect)
{    
    if (QSqlTableModel* M = dynamic_cast<QSqlTableModel*>(Model))
	{
		QSqlRecord Record;

		for(auto i = Vect.constBegin(); i != Vect.constEnd(); ++i)
		{
			Record.append(QSqlField(i.key(), i.value().type()));
			Record.setValue(i.key(), i.value());
        }

        if(!M->insertRecord(-1, Record))
            QMessageBox::critical(this, tr("Error"), tr("Can not insert row"));
        else
            QMessageBox::information(this, tr("Information"), tr("Row inserted"));
        M->select();
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

void TabWidget::onOrderDataReady(QMap<QString, QVariant> Map, QList<QStringList> ProductList)
{
//    ZamowieniaDialog *D = dynamic_cast<ZamowieniaDialog*>(Dialog);
//    if(D)
//        D->open(ProductList, Map);
}

void TabWidget::onPayClicked()
{
    auto Index = ui->View->currentIndex();
    if(Index.isValid())
        emit pay(Model->index(Index.row(), Model->record().indexOf("Identyfikator")).data());
}

void TabWidget::onCompleteClicked()
{
    auto Index = ui->View->currentIndex();
    if(Index.isValid())
        emit complete(Model->index(Index.row(), Model->record().indexOf("Identyfikator")).data());
}

void TabWidget::onDetailsClicked()
{
    auto Index = ui->View->currentIndex();
    if(Index.isValid())
    {
        QMap<QString, QVariant> Map;
        QSqlRecord Record = Model->record(Index.row());
        for(int x = 0; x<Record.count(); ++x)
            Map.insert(Record.fieldName(x), Record.value(x));

        emit details(Map);
    }

}

void TabWidget::onEditClicked()
{
    auto Index = ui->View->currentIndex();
    if(Index.isValid())
    {
        QString Oplacone = Model->index(Index.row(), Model->record().indexOf("Opłacone")).data().toString();
        QString Zrealizowane = Model->index(Index.row(), Model->record().indexOf("Zrealizowano")).data().toString();
        int PK = Model->index(Index.row(), Model->record().indexOf("Identyfikator")).data().toInt();

        if(Oplacone=="Tak" || Zrealizowane=="Tak")
            QMessageBox::information(this, tr("Can not do this"), tr("Can not edit paid or completed order"));
        else
        {
            emit requestOrderData(PK);
        }
    }
    else
        QMessageBox::information(this, tr("Error"), tr("You didn't selected any item"), QMessageBox::Ok);
}

void TabWidget::onRefresh()
{
    if(QSqlTableModel *M = dynamic_cast<QSqlTableModel*>(Model))
        M->select();

    onSearchChanged();
}
