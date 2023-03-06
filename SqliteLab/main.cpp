#include <QApplication>
#include <QtWidgets>
#include <QtSql>
#include "Utils/ConnectionDialog/ConnectionDialog.h"
#include "View/Application/Application.h"


//DESKTOP-24O70ES\LOCAL_SQL_SERVER
//Bykonya
int main(int argc, char *argv[]) {
    QApplication app{ argc, argv };
    Application window{};
    window.show();
    return app.exec();
}
