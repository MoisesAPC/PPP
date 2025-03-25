#ifndef DATABASEACCESSWINDOW_H
#define DATABASEACCESSWINDOW_H

#include <QDialog>

namespace Ui {
class DatabaseAccessWindow;
}

class DatabaseAccessWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseAccessWindow(QWidget *parent = nullptr);
    ~DatabaseAccessWindow();

private:
    Ui::DatabaseAccessWindow *ui;
};

#endif // DATABASEACCESSWINDOW_H
