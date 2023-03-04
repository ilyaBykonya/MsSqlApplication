#pragma once
#include <QSqlDatabase>
#include <QDialog>

class ReadOnlyTableView : public QDialog {
    Q_OBJECT
private:
public:
    ReadOnlyTableView(const QSqlDatabase& connection, const QString& table, QWidget *parent = nullptr);
};

