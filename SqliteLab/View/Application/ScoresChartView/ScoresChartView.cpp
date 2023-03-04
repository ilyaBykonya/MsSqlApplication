#include "ScoresChartView.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSqlRecord>
#include <QImageReader>


ScoresChartView::ScoresChartView(const QSqlDatabase& connection, QWidget* parent)
    :QWidget{ parent }, m_model{ new QSqlTableModel{ this, connection } } {
        m_model->setTable("chart");
        m_model->select();
        auto chart = new QChart{};
        m_chart = new QChartView{ chart };
        auto make_report_button = new QPushButton{ "Report" };
        connect(make_report_button, &QPushButton::clicked, this, &ScoresChartView::saveReport);

        auto window_layout = new QVBoxLayout;
        this->setLayout(window_layout);
            window_layout->addWidget(m_chart, 1);
            window_layout->addWidget(make_report_button);

        auto categories = QStringList{};
        auto points = new QBarSet{ "Scores" };
        auto bounds = std::pair<int32_t, int32_t>{ 0, 0 };
        for(auto row = 0; row < m_model->rowCount(); ++row) {
            auto score = m_model->record(row).value("score").toInt();
            categories.append(QString::number(m_model->record(row).value("id").toInt()));
            points->append(score);
            bounds.first = std::min(bounds.first, score);
            bounds.second = std::max(bounds.second, score);
        }


        auto series = new QBarSeries;
        series->append(points);
        chart->addSeries(series);
        chart->setTitle("Scores log chart");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        auto axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        chart->addAxis(axisY, Qt::AlignLeft);
        axisY->setRange(bounds.first * 1.1, bounds.second * 1.1);
        series->attachAxis(axisY);
    }

void ScoresChartView::saveReport() {
    auto file = QFileDialog::getOpenFileName(this, {}, {}, "Images (*.png *.jpg)");
    if(file.isEmpty())
        return;

    m_chart->grab().save(file);
}
