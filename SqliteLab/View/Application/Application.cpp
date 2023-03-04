#include "Application.h"
#include "View/Application/ScoresChartView/ScoresChartView.h"
#include "Utils/ConnectionDialog/ConnectionDialog.h"
#include "ReadOnlyTableView/ReadOnlyTableView.h"
#include <QCoreApplication>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QDebug>

Application::Application(QWidget *parent)
    :QWidget{ parent }, m_connection{}, m_categories{ new CategoriesListRepository{} }, m_products{ new ProductsView{ m_categories, {} } } {
        auto table_view_button = new QPushButton{ "View table" };
        auto connect_button = new QPushButton{ "Connect" };
        auto scores_button = new QPushButton{ "Scores" };
        auto quit_button = new QPushButton{ "Quit" };
        QObject::connect(connect_button, &QPushButton::clicked, this, &Application::connectToDatabase);
        QObject::connect(table_view_button, &QPushButton::clicked, this, &Application::showTableView);
        QObject::connect(scores_button, &QPushButton::clicked, this, &Application::showScoresView);
        QObject::connect(quit_button, &QPushButton::clicked, qApp, &QCoreApplication::quit);


        auto controls_layout = new QGridLayout;
            controls_layout->addWidget(table_view_button, 0, 0, 1, 1);
            controls_layout->addWidget(connect_button, 0, 1, 1, 1);
            controls_layout->addWidget(scores_button, 1, 0, 1, 1);
            controls_layout->addWidget(quit_button, 1, 1, 1, 1);

        auto window_layout = new QVBoxLayout;
        this->setLayout(window_layout);
            window_layout->addWidget(m_products);
            window_layout->addLayout(controls_layout);
    }

void Application::connectToDatabase() {
    ConnectionDialog connection_info_input{ this };
    if(connection_info_input.exec() != QDialog::DialogCode::Accepted)
        return;

    m_connection = QSqlDatabase::addDatabase("QODBC");
    m_connection.setDatabaseName(connection_info_input.connection());
    m_connection.setUserName(connection_info_input.user());
    m_connection.setPassword(connection_info_input.password());
    m_connection.open();

    m_categories->setConnection(m_connection);
    m_products->setConnection(m_connection);
}

void Application::showScoresView() {
    (new ScoresChartView{ m_connection })->show();
}

void Application::showTableView() {
    auto table_name = QInputDialog::getText(this, "Table", "Table name:");
    if(table_name.isEmpty())
        return;

    ReadOnlyTableView{ m_connection, table_name, this }.exec();
}
