#ifndef OBRAZ_H
#define OBRAZ_H
#include "GLpart.h"
#include "modyfikatory.h"
#include <QLabel>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

class controlPanel : public QWidget {
  Q_OBJECT

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

  controlPanel(QWidget *parent = 0);
};

class Obraz : public QWidget {
  Q_OBJECT

public:
  Obraz(QWidget *parent = 0);

private:
  GLpart *rysunekGL;
  controlPanel *cowidac;
  QLabel *DNindex;
  QLabel *picture1;
  QLabel *picture2;
  std::vector<map> Maps;
  std::vector<function *> Parameters;

signals:

  void changeFunction(int);
  void changeFunction(map);
  void changeWidth(double);
  void changeCentre(std::complex<double>);

public slots:

  void f_global();
  void read_from_file();

  void wWidth05();
  void wWidth07();
  void wWidth10();
  void wWidth12();
  void wWidth15();
  void wWidth17();
  void wWidth20();
  void wWidth25();

  void askForCentre();

  void reloadPictures(QString index);
};

#endif
