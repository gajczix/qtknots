#include "obraz.h"
#include <QApplication>
#include <cstdio>
using namespace std;

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setFont(QFont("Helvetica", 11));

  Obraz obrazek;
  obrazek.show();
  return app.exec();
}
