#include "CategoriesListRepository.h"
#include <QSqlRecord>

CategoriesListRepository::CategoriesListRepository(const QSqlDatabase &connection, QObject *parent)
    :QObject{ parent } { setConnection(connection); }

void CategoriesListRepository::setConnection(const QSqlDatabase &connection) {
    clearConnection();
    m_storage = new QSqlTableModel{ this, connection };
    m_storage->setTable("category");
    m_storage->setEditStrategy(QSqlTableModel::OnRowChange);
    m_storage->select();
    emit this->updated();
}
void CategoriesListRepository::clearConnection() {
    if(m_storage != nullptr)
        m_storage->deleteLater();
}

uint64_t CategoriesListRepository::id_for_name(const QString &name) const {
    for(auto row = 0; row < m_storage->rowCount(); ++row) {
        auto category = m_storage->record(row);
        if(category.value("name").toString() == name) {
            return category.value("id").toULongLong();
        }
    }

    return -1;
}
QString CategoriesListRepository::name_for_id(uint64_t id) const {
    for(auto row = 0; row < m_storage->rowCount(); ++row) {
        auto category = m_storage->record(row);
        if(category.value("id").toULongLong() == id) {
            return category.value("name").toString();
        }
    }

    return {};
}

QStringList CategoriesListRepository::all_categories() const {
    QStringList result;
    for(auto row = 0; row < m_storage->rowCount(); ++row)
        result << m_storage->record(row).value("name").toString();
    return result;
}
