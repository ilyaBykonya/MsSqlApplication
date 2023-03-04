#pragma once
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QtCharts>
#include <QPointer>
#include <QWidget>

class ScoresChartView : public QWidget {
private:
    QPointer<QSqlTableModel> m_model;
    QPointer<QChartView> m_chart;
public:
    ScoresChartView(const QSqlDatabase& connection, QWidget* parent = nullptr);
private slots:
    void saveReport();
};

