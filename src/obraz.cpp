#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QWidget>
#include <cstdio>

#include "obraz.h"

controlPanel::controlPanel(QWidget *parent) : QWidget(parent) {

  xRotSlider = new RotSlider(QString("Rotation around Ox="), this);
  yRotSlider = new RotSlider(QString("Rotation around Oy="), this);
  zRotSlider = new RotSlider(QString("Rotation around  Oz="), this);

  xCenSlider = new CenSlider(QString("Translation along Ox="), this);
  yCenSlider = new CenSlider(QString("Translation along Oy="), this);
  zCenSlider = new CenSlider(QString("Translation along Oz="), this);

  rDoubleSlider = new DoubleSlider(QString("Radius of sphere="), this);

  z1Slider = new ComplexSlider(QString("Central point z1"), this);
  z2Slider = new ComplexSlider(QString("Central point z2"), this);

  kontrolLayout = new QVBoxLayout(this);
  kontrolLayout->addWidget(xRotSlider);
  kontrolLayout->addWidget(yRotSlider);
  kontrolLayout->addWidget(zRotSlider);

  kontrolLayout->addWidget(xCenSlider);
  kontrolLayout->addWidget(yCenSlider);
  kontrolLayout->addWidget(zCenSlider);

  kontrolLayout->addWidget(rDoubleSlider);
  kontrolLayout->addWidget(z1Slider);
  kontrolLayout->addWidget(z2Slider);

  setLayout(kontrolLayout);
}

Obraz::Obraz(QWidget *parent) : QWidget(parent) {

  initialize_maps(Parameters);

  rysunekGL = new GLpart(this);

  rysunekGL->setMinimumSize(600, 600);

  cowidac = new controlPanel(this);
  cowidac->setMinimumSize(300, 300);
  cowidac->setMaximumWidth(500);

  QHBoxLayout *layout = new QHBoxLayout();

  layout->addWidget(cowidac);
  layout->addWidget(rysunekGL);

  picture1 = new QLabel(this);
  picture2 = new QLabel(this);

  DNindex = new QLabel(this);
  DNindex->setText("Double negative index is: " +
                   QString(std::to_string(rysunekGL->DNindex).c_str()));

  reloadPictures(swallowtail_p.NameOfFunction);

  QVBoxLayout *Hlayout = new QVBoxLayout();
  QWidget *wid = new QWidget(this);

  Hlayout->addWidget(picture1);
  Hlayout->addWidget(picture2);
  Hlayout->addWidget(DNindex);

  wid->setLayout(Hlayout);
  wid->setMinimumWidth(350);

  layout->addWidget(wid);
  setLayout(layout);

  connect(cowidac->xRotSlider, SIGNAL(RotValueChanged(int)), rysunekGL,
          SLOT(xRotated(int)));
  connect(cowidac->yRotSlider, SIGNAL(RotValueChanged(int)), rysunekGL,
          SLOT(yRotated(int)));
  connect(cowidac->zRotSlider, SIGNAL(RotValueChanged(int)), rysunekGL,
          SLOT(zRotated(int)));

  connect(cowidac->xCenSlider, SIGNAL(CenValueChanged(float)), rysunekGL,
          SLOT(xCentered(float)));
  connect(cowidac->yCenSlider, SIGNAL(CenValueChanged(float)), rysunekGL,
          SLOT(yCentered(float)));
  connect(cowidac->zCenSlider, SIGNAL(CenValueChanged(float)), rysunekGL,
          SLOT(zCentered(float)));

  connect(cowidac->rDoubleSlider, SIGNAL(DoubleValueChanged(double)), rysunekGL,
          SLOT(rChanged(double)));
  connect(cowidac->z1Slider, SIGNAL(ComplexValueChanged(complex)), rysunekGL,
          SLOT(z1Changed(complex)));
  connect(cowidac->z2Slider, SIGNAL(ComplexValueChanged(complex)), rysunekGL,
          SLOT(z2Changed(complex)));

  connect(rysunekGL, SIGNAL(xRotChanged(int)), cowidac->xRotSlider,
          SLOT(externalValue(int)));
  connect(rysunekGL, SIGNAL(yRotChanged(int)), cowidac->yRotSlider,
          SLOT(externalValue(int)));
  connect(rysunekGL, SIGNAL(zRotChanged(int)), cowidac->zRotSlider,
          SLOT(externalValue(int)));
  connect(rysunekGL, SIGNAL(zCenChanged(float)), cowidac->zCenSlider,
          SLOT(externalValue(float)));
  connect(rysunekGL, SIGNAL(xCenChanged(complex)), cowidac->z1Slider,
          SLOT(externalComplex(complex)));
  connect(rysunekGL, SIGNAL(yCenChanged(complex)), cowidac->z2Slider,
          SLOT(externalComplex(complex)));

  // dodajemy menu

  QMenuBar *pasekmenu = new QMenuBar(rysunekGL);
  QMenu *funkcjeMenu = pasekmenu->addMenu(tr("&Change curve"));
  {
    QAction *auxAction;
    QString stringAction;

    for (unsigned int wal = 0; wal < Parameters.size(); wal++) {
      auxAction =
          new QAction(QString(Parameters[wal]->NameOfFunction.c_str()), this);
      if (wal == Parameters.size() - 1) {
        auxAction->setShortcut(Qt::Key_R | Qt::CTRL);
      }
      stringAction.setNum(wal);
      auxAction->setObjectName(stringAction);
      funkcjeMenu->addAction(auxAction);
      connect(auxAction, SIGNAL(triggered()), this, SLOT(f_global()));
    }
    QString funcButton = "insert function from file";

    QAction *functionFromFileAction = new QAction(funcButton, this);
    connect(functionFromFileAction, SIGNAL(triggered()), this,
            SLOT(read_func_from_file()));

    funkcjeMenu->addAction(functionFromFileAction);

    QString knotButton = "insert knot from file";

    QAction *knotFromFileAction = new QAction(knotButton, this);
    connect(knotFromFileAction, SIGNAL(triggered()), this,
            SLOT(read_knot_from_file()));

    funkcjeMenu->addAction(knotFromFileAction);
  }

  connect(this, SIGNAL(changeFunction(int)), rysunekGL,
          SLOT(functionChanged(int)));
  connect(this, SIGNAL(changeFunction(map)), rysunekGL,
          SLOT(functionChanged(map)));
  connect(this, SIGNAL(changeFunction(std::vector<fourvector>, std::string)),
          rysunekGL,
          SLOT(functionChanged(std::vector<fourvector>, std::string)));

  QMenu *gruboscMenu = pasekmenu->addMenu(tr("&Adjust width"));

  {
    QAction *G05A = new QAction(tr("0.5"), this);
    gruboscMenu->addAction(G05A);
    connect(G05A, SIGNAL(triggered()), this, SLOT(wWidth05()));

    QAction *G07A = new QAction(tr("0.7"), this);
    gruboscMenu->addAction(G07A);
    connect(G07A, SIGNAL(triggered()), this, SLOT(wWidth07()));

    QAction *G10A = new QAction(tr("1.0"), this);
    gruboscMenu->addAction(G10A);
    connect(G10A, SIGNAL(triggered()), this, SLOT(wWidth10()));

    QAction *G12A = new QAction(tr("1.2"), this);
    gruboscMenu->addAction(G12A);
    connect(G12A, SIGNAL(triggered()), this, SLOT(wWidth12()));

    QAction *G15A = new QAction(tr("1.5"), this);
    gruboscMenu->addAction(G15A);
    connect(G15A, SIGNAL(triggered()), this, SLOT(wWidth15()));

    QAction *G17A = new QAction(tr("1.7"), this);
    gruboscMenu->addAction(G17A);
    connect(G17A, SIGNAL(triggered()), this, SLOT(wWidth17()));

    QAction *G20A = new QAction(tr("2.0"), this);
    gruboscMenu->addAction(G20A);
    connect(G20A, SIGNAL(triggered()), this, SLOT(wWidth20()));

    QAction *G25A = new QAction(tr("2.5"), this);
    gruboscMenu->addAction(G25A);
    connect(G25A, SIGNAL(triggered()), this, SLOT(wWidth25()));
  }
  connect(this, SIGNAL(changeWidth(double)), rysunekGL,
          SLOT(widthChanged(double)));

  QAction *paramA = new QAction(tr("P&arametric Centre"), this);
  pasekmenu->addAction(paramA);
  connect(paramA, SIGNAL(triggered()), this, SLOT(askForCentre()));

  connect(this, SIGNAL(changeCentre(complex)), rysunekGL,
          SLOT(parametricCentreChecked(complex)));
}

map parseMapFromString(std::string input) {
  std::string line;
  std::istringstream f(input);
  std::vector<std::pair<std::pair<int, int>, complex>> coefficient;
  std::string name;
  std::getline(f, name);
  while (std::getline(f, line)) {
    int i, j;
    double x, y;
    sscanf(line.c_str(), "%d %d %lf %lf", &i, &j, &x, &y);
    coefficient.emplace_back(
        std::make_pair(std::make_pair(i, j), complex(x, y)));
    std::cout << line << std::endl;
  }
  return map(coefficient, name);
}

std::vector<fourvector> parseKnotFromString(std::string input) {
  std::string line;
  std::istringstream f(input);
  std::vector<fourvector> points;
  while (std::getline(f, line)) {
    double x, y, z, u;
    sscanf(line.c_str(), "%lf %lf %lf %lf", &x, &y, &z, &u);
    points.emplace_back(x, y, z, u);
    std::cout << line << std::endl;
  }
  return points;
}

void Obraz::read_func_from_file() {
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Function from file"));
  QFile file(fileName);
  file.open(QIODevice::ReadWrite);
  QByteArray fileContent = file.readAll();
  std::string fileString = fileContent.toStdString();
  map userMap = parseMapFromString(fileString);

  emit changeFunction(userMap);
  reloadPictures(userMap.NameOfFunction);
}

void Obraz::read_knot_from_file() {
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Function from file"));
  QFile file(fileName);
  file.open(QIODevice::ReadWrite);
  QByteArray fileContent = file.readAll();
  std::string fileString = fileContent.toStdString();
  std::vector<fourvector> points = parseKnotFromString(fileString);

  QFileInfo fileInfo(file);
  QString functionName(fileInfo.fileName());
  emit changeFunction(points, functionName.toStdString());
  reloadPictures(functionName.toStdString());
}

void Obraz::f_global() {
  QObject *obj = QObject::sender();
  obj->dumpObjectInfo();
  QString nnm = obj->objectName();
  emit changeFunction(nnm.toInt());

  reloadPictures(Parameters[nnm.toInt()]->NameOfFunction);
}

void Obraz::wWidth05() { emit changeWidth(0.5); }

void Obraz::wWidth07() { emit changeWidth(0.7); }

void Obraz::wWidth10() { emit changeWidth(1.0); }

void Obraz::wWidth12() { emit changeWidth(2.0); }

void Obraz::wWidth15() { emit changeWidth(3.0); }

void Obraz::wWidth17() { emit changeWidth(5.0); }

void Obraz::wWidth20() { emit changeWidth(8.0); }

void Obraz::wWidth25() { emit changeWidth(10.0); }

void Obraz::askForCentre() {
  bool ok1, ok2;
  double r1 = QInputDialog::getDouble(this, tr("Parametric centre"),
                                      tr("Give real part"), 0.0, -20.01, 20.99,
                                      2, &ok1);
  double r2 = QInputDialog::getDouble(this, tr("Srodek sfery"),
                                      tr("Give imaginary part"), 0.0, -20,
                                      20.99, 2, &ok2);
  if (ok1 && ok2) {
    emit changeCentre(complex(r1, r2));
  }
}

void setupDNIndexLabel(QLabel *label, int fontSize) {
  QFont font("Arial", fontSize, QFont::Bold);
  label->setFont(font);
  label->setWordWrap(true);
}

void Obraz::reloadPictures(std::string name) {
  QString qname = QString(name.c_str());
  QPixmap pix("./pictures/" + qname + ".png");
  setupDNIndexLabel(DNindex, 20);
  DNindex->setText("Double negative index for " + qname + " is: " +
                   QString(std::to_string(rysunekGL->DNindex).c_str()));
  picture1->setPixmap(pix);
  QPixmap pix2("./pdpictures/" + qname + ".svg");
  picture2->setPixmap(pix2.scaled(300, 300, Qt::KeepAspectRatio));
}
