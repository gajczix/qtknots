#include "modyfikatory.h"
#include <QInputDialog>

RotSlider::RotSlider(QString nazwa, QWidget *parent = 0) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Sunken);

  nazwa2 = nazwa;

  mainRotSlider = new QSlider(Qt::Horizontal, this);
  mainRotSlider->setRange(-40, 400);
  mainRotSlider->setValue(0);

  labelRotSlider = new QLabel(nazwa2, this);
  layoutRotSlider = new QHBoxLayout(this);

  layoutRotSlider->addWidget(labelRotSlider);
  layoutRotSlider->addWidget(mainRotSlider);

  setLayout(layoutRotSlider);

  connect(mainRotSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));

  connect(this, SIGNAL(RotValueChanged(int)), mainRotSlider,
          SLOT(setValue(int)));
}

void RotSlider::externalValue(int a) {
  angle = a;
  while (angle < 0) {
    angle += 360;
  }
  while (angle > 360) {
    angle -= 360;
  }
  emit RotValueChanged(angle);
}

CenSlider::CenSlider(QString nazwa, QWidget *parent = 0) : QFrame(parent) {
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Sunken);

  nazwa2 = nazwa;

  mainCenSlider = new QSlider(Qt::Horizontal, this);
  mainCenSlider->setRange(-10, 30);
  mainCenSlider->setValue(0);
  mainCenSlider->setSingleStep(1);
  mainCenSlider->setPageStep(5);
  mainCenSlider->setTickPosition(QSlider::TicksAbove);
  mainCenSlider->setTickInterval(3);

  otherCenSlider = new QSlider(Qt::Horizontal, this);
  otherCenSlider->setRange(-20, 120);
  otherCenSlider->setValue(0);
  otherCenSlider->setSingleStep(1);
  otherCenSlider->setPageStep(5);
  otherCenSlider->setTickPosition(QSlider::TicksAbove);
  otherCenSlider->setTickInterval(10);

  center = 0;

  labelCenSlider = new QLabel(this);
  labelCenSlider->setText(nazwa2 + QString("=%1").arg(center, 0, 'f', 2));

  layoutCenSlider = new QHBoxLayout(this);
  layoutCenSlider->addWidget(labelCenSlider);
  layoutCenSlider->addWidget(mainCenSlider);
  layoutCenSlider->addWidget(otherCenSlider);

  setLayout(layoutCenSlider);

  connect(mainCenSlider, SIGNAL(valueChanged(int)), this,
          SLOT(internalValue(int)));
  connect(otherCenSlider, SIGNAL(valueChanged(int)), this,
          SLOT(internalValue(int)));
}

void CenSlider::internalValue(int) {
  int r = otherCenSlider->value();
  int s = mainCenSlider->value();
  if (r < 0 and s > -10) {
    disconnect(mainCenSlider, SIGNAL(valueChanged(int)), this,
               SLOT(internalValue(int)));
    disconnect(otherCenSlider, SIGNAL(valueChanged(int)), this,
               SLOT(internalValue(int)));
    r += 100;
    s -= 1;
    mainCenSlider->setValue(s);
    otherCenSlider->setValue(r);
    connect(mainCenSlider, SIGNAL(valueChanged(int)), this,
            SLOT(internalValue(int)));
    connect(otherCenSlider, SIGNAL(valueChanged(int)), this,
            SLOT(internalValue(int)));
  }

  if (r > 100 and s < 30) {
    disconnect(mainCenSlider, SIGNAL(valueChanged(int)), this,
               SLOT(internalValue(int)));
    disconnect(otherCenSlider, SIGNAL(valueChanged(int)), this,
               SLOT(internalValue(int)));
    r -= 100;
    s += 1;
    mainCenSlider->setValue(s);
    otherCenSlider->setValue(r);
    connect(mainCenSlider, SIGNAL(valueChanged(int)), this,
            SLOT(internalValue(int)));
    connect(otherCenSlider, SIGNAL(valueChanged(int)), this,
            SLOT(internalValue(int)));
  }

  center = float(s) + 0.01 * float(r);
  labelCenSlider->setText(nazwa2 + QString("=%1").arg(center, 0, 'f', 2));
  emit CenValueChanged(center);
}

void CenSlider::externalValue(float f) {
  disconnect(mainCenSlider, SIGNAL(valueChanged(int)), this,
             SLOT(internalValue(int)));
  disconnect(otherCenSlider, SIGNAL(valueChanged(int)), this,
             SLOT(internalValue(int)));
  if (f > 30.99) {
    f = 30.99;
  }
  if (f < -10.0) {
    f = -10.0;
  }
  int s = (int)f;
  int r = int(100 * (f - s));
  mainCenSlider->setValue(s);
  otherCenSlider->setValue(r);
  connect(mainCenSlider, SIGNAL(valueChanged(int)), this,
          SLOT(internalValue(int)));
  connect(otherCenSlider, SIGNAL(valueChanged(int)), this,
          SLOT(internalValue(int)));
}

DoubleSlider::DoubleSlider(QString nazwa, QWidget *parent = 0)
    : QFrame(parent) {

  nazwa2 = nazwa;

  mainDoubleSlider = new QSlider(Qt::Horizontal, this);
  mainDoubleSlider->setRange(0, 20);
  mainDoubleSlider->setValue(1);
  mainDoubleSlider->setSingleStep(1);
  mainDoubleSlider->setPageStep(5);
  mainDoubleSlider->setTickPosition(QSlider::TicksAbove);
  mainDoubleSlider->setTickInterval(1);

  otherDoubleSlider = new QSlider(Qt::Horizontal, this);
  otherDoubleSlider->setRange(-30, 130);
  otherDoubleSlider->setValue(0);
  otherDoubleSlider->setSingleStep(1);
  otherDoubleSlider->setPageStep(5);
  otherDoubleSlider->setTickPosition(QSlider::TicksBelow);
  otherDoubleSlider->setTickInterval(5);

  wez = new QPushButton(this);
  wez->setText("Enter");

  radius = 1.0;

  labelDoubleSlider = new QLabel(this);
  labelDoubleSlider->setText(nazwa2 + QString("=%1").arg(radius, 0, 'f', 2));

  layoutDoubleSlider = new QGridLayout(this);
  layoutDoubleSlider->addWidget(labelDoubleSlider, 1, 0);
  layoutDoubleSlider->addWidget(mainDoubleSlider, 0, 1);
  layoutDoubleSlider->addWidget(otherDoubleSlider, 2, 1);
  layoutDoubleSlider->addWidget(wez, 1, 2);

  setLayout(layoutDoubleSlider);

  connect(mainDoubleSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(otherDoubleSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(wez, SIGNAL(clicked()), this, SLOT(dialogBoxRequested()));
}

void DoubleSlider::externalValue(int) {
  radius = double(mainDoubleSlider->value()) +
           0.01 * double(otherDoubleSlider->value());
  if (radius < 0.0) {
    radius = 0.01;
  }
  labelDoubleSlider->setText(nazwa2 + QString("=%1").arg(radius, 0, 'f', 2));
  emit DoubleValueChanged(radius);
}

void DoubleSlider::dialogBoxRequested() {
  bool ok;
  double r = QInputDialog::getDouble(this, tr("Enter radius"),
                                     tr("Radius of the sphere"), 1.0, 0.01,
                                     20.99, 4, &ok);
  if (ok) {
    radius = r;
    disconnect(mainDoubleSlider, SIGNAL(valueChanged(int)), this,
               SLOT(externalValue(int)));
    disconnect(otherDoubleSlider, SIGNAL(valueChanged(int)), this,
               SLOT(externalValue(int)));
    mainDoubleSlider->setValue(int(radius));
    otherDoubleSlider->setValue(int(100.0 * (radius - int(radius))));
    labelDoubleSlider->setText(nazwa2 + QString("=%1").arg(radius, 0, 'f', 2));
    connect(mainDoubleSlider, SIGNAL(valueChanged(int)), this,
            SLOT(externalValue(int)));
    connect(otherDoubleSlider, SIGNAL(valueChanged(int)), this,
            SLOT(externalValue(int)));
    emit DoubleValueChanged(radius);
  }
}

ComplexSlider::ComplexSlider(QString nazwa, QWidget *parent = 0)
    : QFrame(parent) {

  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Sunken);

  nazwa2 = nazwa;

  z = complex(0.0, 0.0);

  mainRealSlider = new QSlider(Qt::Horizontal, this);
  mainRealSlider->setRange(-20, 20);
  mainRealSlider->setValue(0);
  mainRealSlider->setSingleStep(1);
  mainRealSlider->setPageStep(5);

  mainImagSlider = new QSlider(Qt::Horizontal, this);
  mainImagSlider->setRange(-20, 20);
  mainImagSlider->setValue(0);
  mainImagSlider->setSingleStep(1);
  mainImagSlider->setPageStep(5);

  otherRealSlider = new QSlider(Qt::Horizontal, this);
  otherRealSlider->setRange(-20, 130);
  otherRealSlider->setValue(0);
  otherRealSlider->setSingleStep(1);
  otherRealSlider->setPageStep(10);

  otherImagSlider = new QSlider(Qt::Horizontal, this);
  otherImagSlider->setRange(-20, 130);
  otherImagSlider->setValue(0);
  otherImagSlider->setSingleStep(1);
  otherImagSlider->setPageStep(10);

  labelComplexSlider = new QLabel(nazwa2, this);

  labelRealSlider = new QLabel(QString("re=%1").arg((z.real())), this);
  labelImagSlider = new QLabel(QString("im=%1").arg((z.imag())), this);

  wez = new QPushButton(this);
  wez->setText("Input");

  layoutComplexSlider = new QGridLayout(this);

  layoutComplexSlider->addWidget(labelComplexSlider, 0, 1);
  layoutComplexSlider->addWidget(labelRealSlider, 1, 1);
  layoutComplexSlider->addWidget(labelImagSlider, 2, 1);
  layoutComplexSlider->addWidget(mainRealSlider, 1, 2);
  layoutComplexSlider->addWidget(mainImagSlider, 2, 2);
  layoutComplexSlider->addWidget(otherRealSlider, 1, 3);
  layoutComplexSlider->addWidget(otherImagSlider, 2, 3);
  layoutComplexSlider->addWidget(wez, 0, 3);

  setLayout(layoutComplexSlider);

  connect(mainRealSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(mainImagSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(otherRealSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(otherImagSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));

  connect(wez, SIGNAL(clicked()), this, SLOT(dialogBoxRequested()));
}
void ComplexSlider::externalValue(int) {
  double x =
      double(mainRealSlider->value()) + 0.01 * double(otherRealSlider->value());
  double y =
      double(mainImagSlider->value()) + 0.01 * double(otherImagSlider->value());

  labelRealSlider->setText(QString("re=%1").arg(x));
  labelImagSlider->setText(QString("im=%1").arg(y));
  z = complex(x, y);

  emit ComplexValueChanged(z);
}
void ComplexSlider::dialogBoxRequested() {
  bool ok1 = false;
  bool ok2 = false;
  double r1 = QInputDialog::getDouble(this, tr("Center of sphere"),
                                      tr("Enter the real part"), 0.0, -20.01,
                                      20.99, 2, &ok1);
  double r2 = QInputDialog::getDouble(this, tr("Center of sphere"),
                                      tr("Enter the imaginary part"), 0.0, -20,
                                      20.99, 2, &ok2);

  if (ok1 && ok2) {
    z = complex(r1, r2);
    disconnect(mainRealSlider, SIGNAL(valueChanged(int)), this,
               SLOT(externalValue(int)));
    disconnect(mainImagSlider, SIGNAL(valueChanged(int)), this,
               SLOT(externalValue(int)));
    disconnect(otherRealSlider, SIGNAL(valueChanged(int)), this,
               SLOT(externalValue(int)));
    disconnect(otherImagSlider, SIGNAL(valueChanged(int)), this,
               SLOT(externalValue(int)));

    mainRealSlider->setValue(int(r1));
    otherRealSlider->setValue(int(100.0 * (r1 - int(r1))));

    mainRealSlider->setValue(int(r2));
    otherRealSlider->setValue(int(100.0 * (r2 - int(r2))));

    labelRealSlider->setText(QString("re=%1").arg(r1));
    labelImagSlider->setText(QString("im=%1").arg(r2));

    connect(mainRealSlider, SIGNAL(valueChanged(int)), this,
            SLOT(externalValue(int)));
    connect(mainImagSlider, SIGNAL(valueChanged(int)), this,
            SLOT(externalValue(int)));
    connect(otherRealSlider, SIGNAL(valueChanged(int)), this,
            SLOT(externalValue(int)));
    connect(otherImagSlider, SIGNAL(valueChanged(int)), this,
            SLOT(externalValue(int)));

    emit ComplexValueChanged(z);
  }
}

void ComplexSlider::externalComplex(complex za) {
  z = za;
  disconnect(mainRealSlider, SIGNAL(valueChanged(int)), this,
             SLOT(externalValue(int)));
  disconnect(mainImagSlider, SIGNAL(valueChanged(int)), this,
             SLOT(externalValue(int)));
  disconnect(otherRealSlider, SIGNAL(valueChanged(int)), this,
             SLOT(externalValue(int)));
  disconnect(otherImagSlider, SIGNAL(valueChanged(int)), this,
             SLOT(externalValue(int)));

  double r1 = za.real();
  double r2 = za.imag();

  mainRealSlider->setValue(int(r1));
  otherRealSlider->setValue(int(100.0 * (r1 - int(r1))));

  mainRealSlider->setValue(int(r2));
  otherRealSlider->setValue(int(100.0 * (r2 - int(r2))));

  labelRealSlider->setText(QString("re=%1").arg(r1));
  labelImagSlider->setText(QString("im=%1").arg(r2));

  connect(mainRealSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(mainImagSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(otherRealSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
  connect(otherImagSlider, SIGNAL(valueChanged(int)), this,
          SLOT(externalValue(int)));
}
