#pragma once
#include <QSqlTableModel>
#include <QPointer>

class CategoriesListRepository: public QObject {
    Q_OBJECT
private:
    QPointer<QSqlTableModel> m_storage;
public:
    CategoriesListRepository(const QSqlDatabase& connection = {}, QObject *parent = nullptr);
    void setConnection(const QSqlDatabase& connection);
    void clearConnection();
public:
    std::uint64_t id_for_name(const QString& name) const;
    QString name_for_id(std::uint64_t id) const;
    QStringList all_categories() const;
signals:
    void updated();
};

