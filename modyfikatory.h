#ifndef MODYFIKATORY_H
#define MODYFIKATORY_H

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QString>

#include "matbase.h"

// klasa RotSlider opisuje rotacje obiektu

class RotSlider : public QFrame {
  Q_OBJECT

private:
  QSlider *mainRotSlider;
  QLabel *labelRotSlider;
  QHBoxLayout *layoutRotSlider;
  QString nazwa2;
  int angle;

public:
  RotSlider(QString, QWidget *);

signals:

  void RotValueChanged(int);

public slots:

  void externalValue(int);
};

// klasa punktu srodkowego oznacza
// mozliwosc przesuwania srodka
// ukladu wspolrzednych.

class CenSlider : public QFrame {
  Q_OBJECT

private:
  QSlider *mainCenSlider;
  QSlider *otherCenSlider;
  QLabel *labelCenSlider;
  QString nazwa2;
  QHBoxLayout *layoutCenSlider;
  float center;

public:
  CenSlider(QString nazwa, QWidget *parent);

signals:

  void CenValueChanged(float);

protected slots:

  void internalValue(int);

public slots:

  void externalValue(float);
};

// klasa zmiany promienia.
// tutaj pozwalamy sobie na duzy zasieg
// stosujac jednoczesnie dwa suwaki

// moglbym dziedziczyc z CenSlider
// ale za duzo roboty

class DoubleSlider : public QFrame {
  Q_OBJECT

private:
  QSlider *mainDoubleSlider;
  QSlider *otherDoubleSlider;
  QLabel *labelDoubleSlider;
  QString nazwa2;
  QGridLayout *layoutDoubleSlider;
  double radius;
  QPushButton *wez;

public:
  DoubleSlider(QString nazwa, QWidget *parent);

signals:

  void DoubleValueChanged(double);

protected slots:

  void externalValue(int);
  void dialogBoxRequested();
};
// zmiana srodka sfery w C^2.
// zapisywana jako zmiana dwoch
// parametrow zespolonych.
class ComplexSlider : public QFrame {
  Q_OBJECT

private:
  QSlider *mainRealSlider;
  QSlider *otherRealSlider;
  QSlider *mainImagSlider;
  QSlider *otherImagSlider;
  QLabel *labelComplexSlider;
  QLabel *labelRealSlider;
  QLabel *labelImagSlider;
  QGridLayout *layoutComplexSlider;
  complex z;
  QString nazwa2;
  QPushButton *wez;

public:
  ComplexSlider(QString nazwa, QWidget *parent);

signals:

  void ComplexValueChanged(complex);

protected slots:

  void externalValue(int);
  void dialogBoxRequested();

public slots:

  void externalComplex(complex);
};
#endif
