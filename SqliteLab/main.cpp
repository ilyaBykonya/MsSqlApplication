#include <QApplication>
#include <QtWidgets>
#include <QtSql>

int main(int argc, char *argv[]) {
    QApplication app{ argc, argv };
    QWidget window;
    auto window_layout = new QVBoxLayout{};
    window.setLayout(window_layout);
        auto address_input = new QLineEdit{};
        address_input->setPlaceholderText("Address");
        auto database_input = new QLineEdit{};
        database_input->setPlaceholderText("Database");
        auto user_input = new QLineEdit{};
        user_input->setPlaceholderText("User");
        auto password_input = new QLineEdit{};
        password_input->setPlaceholderText("Password");
        auto connect_button = new QPushButton{ "Connect" };
        window_layout->addWidget(address_input);
        window_layout->addWidget(database_input);
        window_layout->addWidget(user_input);
        window_layout->addWidget(password_input);
        window_layout->addWidget(connect_button);


    QObject::connect(connect_button, &QPushButton::clicked, [&] {
        auto database = QSqlDatabase::addDatabase("QODBC");
        database.setDatabaseName(QString{ "DRIVER={SQL Server};SERVER=%1;DATABASE=%2;" }.arg(address_input->text()).arg(database_input->text()));
        database.setUserName(user_input->text());
        database.setPassword(password_input->text());
        qDebug() << "Open result => " << database.open();
    });

    window.show();
    return app.exec();
}
