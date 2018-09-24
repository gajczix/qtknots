#ifndef OBRAZ_H
#define OBRAZ_H
#include "GLpart.h"
#include "functions.h"
#include "modyfikatory.h"
#include <QLabel>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

class kontrolka : public QWidget {
  Q_OBJECT

  // TODO: przeczytac to
  // czynimy suwaki publiczne,
  // zeby miec dostep do wszystkich
  // sygnalow, ktore one wysylaja.
  // w przeciwnym razie klasa kontrolka
  // musialaby miec cala siec slotow
  // i sygnalow do przekazywania danych
  // inna mozliwosc to uczynienie klasy
  // obraz przyjacielska. to wlasciwie
  // na jedno wychodzi.

public:
  RotSlider *xRotSlider;
  RotSlider *yRotSlider;
  RotSlider *zRotSlider;
  CenSlider *xCenSlider;
  CenSlider *yCenSlider;
  CenSlider *zCenSlider;
  DoubleSlider *rDoubleSlider;
  ComplexSlider *z1Slider;
  ComplexSlider *z2Slider;
  QVBoxLayout *kontrolLayout;

  kontrolka(QWidget *parent = 0);
};

class Obraz : public QWidget {
  Q_OBJECT

public:
  Obraz(QWidget *parent = 0);

private:
  GLpart *rysunekGL;
  kontrolka *cowidac;
  QLabel *DNindex;
  QLabel *picture1;
  QLabel *picture2;
  std::vector<map> Maps;
  std::vector<function *> Parameters;

signals:

  void changeFunction(int);
  void changeWidth(double);
  void changeCentre(complex);

public slots:

  /* void fJaskolka();
 void fPetla();
 void fT23();
 void fT25();
 void fT34();
 void fT35();*/
  //   void fT234();
  void f_global();

  void wGrubosc05();
  void wGrubosc07();
  void wGrubosc10();
  void wGrubosc12();
  void wGrubosc15();
  void wGrubosc17();
  void wGrubosc20();
  void wGrubosc25();

  void askForCentre();

  void reloadPictures(QString index);
};

#endif
