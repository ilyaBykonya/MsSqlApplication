#include <QApplication>
#include <QtWidgets>
#include <QtSql>
#include "Utils/ConnectionDialog/ConnectionDialog.h"

int main(int argc, char *argv[]) {
    QApplication app{ argc, argv };
    QWidget window;
    auto window_layout = new QVBoxLayout{};
    window.setLayout(window_layout);
        auto connect_button = new QPushButton{ "Connect" };
        window_layout->addWidget(connect_button);

    QObject::connect(connect_button, &QPushButton::clicked, [&] {
        ConnectionDialog connection_info_input{ &window };
        if(connection_info_input.exec() == QDialog::DialogCode::Accepted) {
            auto database = QSqlDatabase::addDatabase("QODBC");
            database.setDatabaseName(connection_info_input.connection());

            qDebug() << "Open result => " << database.open(connection_info_input.user(), connection_info_input.password());
        }
    });

    window.show();
    return app.exec();
}
