#include "logindialog.h"
#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login_dialog w;
    w.show();

    return a.exec();
}
