#ifndef DATABASESAVELISTACTIONWINDOW_H
#define DATABASESAVELISTACTIONWINDOW_H

#include <QDialog>

namespace Ui {
class DatabaseSaveListActionWindow;
}

class DatabaseSaveListActionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSaveListActionWindow(const QString& docId, QWidget* parent = nullptr);
    ~DatabaseSaveListActionWindow();

private:
    Ui::DatabaseSaveListActionWindow* ui;
    QString documentId = "";
};

#endif // DATABASESAVELISTACTIONWINDOW_H
