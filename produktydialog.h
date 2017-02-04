#ifndef PRODUKTYDIALOG_H
#define PRODUKTYDIALOG_H

#include <QWidget>
#include <QMap>

#include "basedialog.h"
#include <QPushButton>

namespace Ui {
	class ProduktyDialog;
}

class ProduktyDialog : public BaseDialog
{
		Q_OBJECT

	public:
		explicit ProduktyDialog(QWidget *parent = 0);
		~ProduktyDialog();

	private:
		Ui::ProduktyDialog *ui;

	public slots:
		void onNewList(QList<QPair<QString, int> > Lista);
		virtual void accept(void);
		void onTextChanged(void);
		virtual void open();

	signals:

		void categoriesRequest(void);
};

#endif // PRODUKTYDIALOG_H
