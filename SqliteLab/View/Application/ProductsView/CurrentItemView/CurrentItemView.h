#pragma once
#include <QSqlQueryModel>
#include <QPushButton>
#include <QLineEdit>
#include <QPointer>
#include <optional>

class CurrentItemView : public QWidget {
    Q_OBJECT
private:
    class ProductRecord {
    private:
        std::uint64_t m_id;
        QString m_category;
        QString m_name;
    public:
        ProductRecord(std::uint64_t id, const QString& name, const QString& category);
        const QString& category() const;
        const QString& name() const;
        std::uint64_t id() const;
    };

    std::optional<std::pair<std::uint64_t, ProductRecord>> m_record;
    QPointer<QLineEdit> m_current_id_view;
    QPointer<QLineEdit> m_current_name_view;
    QPointer<QLineEdit> m_current_category_view;
    QPointer<QPushButton> m_update_button;
    QPointer<QPushButton> m_delete_button;
    QPointer<QPushButton> m_refresh_button;
public:
    CurrentItemView(QWidget* parent = nullptr);
public slots:
    void setCurrentRecord(std::uint64_t line, std::uint64_t id, const QString& name, const QString& category);
private slots:
    void resetRecord(const std::optional<std::pair<std::uint64_t, ProductRecord>>& record);
private slots:
    void onUpdate();
    void onDelete();
    void onRefresh();
signals:
    void deleteRecord(std::uint64_t line, std::uint64_t id);
    void updateRecord(std::uint64_t line, std::uint64_t id, const QString& name, const QString& category);
};

