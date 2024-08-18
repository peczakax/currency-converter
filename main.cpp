#include <QApplication>
#include "CurrencyConverter.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CurrencyConverter converter;
    converter.show();
    return app.exec();
}
