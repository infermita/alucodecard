#include <QCoreApplication>
#include "lib/nfcthread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NfcThread nfcTh;
    nfcTh.start();

    return a.exec();
}
