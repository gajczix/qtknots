#include "GLpart.h"
#include <GL/glut.h>
#include <QGLWidget>
#include <QtCore/qglobal.h>
#include <QtGui>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

int lwidth = 2.0;

void Knot::draw(bool isProj) {
  glLineWidth(lwidth);
  glBegin(GL_LINE_STRIP);
  float k, l, m, e;
  double color;
  double siz = double(wek.size());

  for (uint i = 0; i < siz; i++) {
    fourvector drugi = wek[i];
    if (this->func != NULL) {
      drugi = drugi - this->func->centerPoint();
    }

    k = drugi.x();
    l = drugi.y();
    m = drugi.z();
    e = drugi.u();
    color = 3.0 * double(i) / siz;
    if (color < 1.0)
      glColor3f(color, 1.0 - color, 0);
    else if (color < 2.0)
      glColor3f(2.0 - color, 0.0, color - 1.0);
    else
      glColor3f(0.0, color - 2.0, 3.0 - color);

    if (isProj) {
      glVertex2f(k / (m + R), l / (m + R));
    } else {
      glVertex3f(k / (m + R), l / (m + R), (-1 * (R - e)) / (m + R));
    }
  }
  glEnd();
}

void GLpart::makeList() {
  if (glIsList(knot2) == GL_TRUE)
    glDeleteLists(knot2, 1);
  knot2 = glGenLists(1);

  rysuje = true;
  glNewList(knot2, GL_COMPILE);
  Knot newKnot;

  if (param_or_map != NULL) {
    newKnot = Knot(radius, param_or_map);
  } else {
    newKnot = Knot(userPoints, userName);
  }

  newKnot.draw();
  newKnot.dumpPoints();
  newKnot.giveIntersectionNumber();

  std::map<int, double> angle_map;
  auto PDCode = newKnot.givePDCode(angle_map);

  std::string knotName = newKnot.getName();
  printPDCode(PDCode, knotName);

  auto graph = generateGraphFromPDCode(PDCode, angle_map);

  printGraph(graph, knotName);
  generatePictures(knotName);

  this->DNindex = doubleNegativeIndex(graph);

  glEndList();
  rysuje = false;

  GLuint knot3 = knot2;
  knot = knot3;
}

GLpart::~GLpart() {
  makeCurrent();

  if (glIsList(knot2) == GL_TRUE)
    glDeleteLists(knot2, 1);
  if (glIsList(knot) == GL_TRUE)
    glDeleteLists(knot, 1);
}

void GLpart::xRotated(int s) {
  xRotCur = s;
  updateGL();
}

void GLpart::yRotated(int s) {
  yRotCur = s;
  updateGL();
}

void GLpart::zRotated(int s) {
  zRotCur = s;
  updateGL();
}

void GLpart::xCentered(float f) {
  xCenCur = f;
  updateGL();
}

void GLpart::yCentered(float f) {
  yCenCur = f;
  updateGL();
}
void GLpart::zCentered(float f) {
  zCenCur = f;
  updateGL();
}

void GLpart::rChanged(double r) {
  radius = r;
  emit parameterChanged();
}

void GLpart::z1Changed(complex z1) {
  param_or_map->centerX = z1;
  emit parameterChanged();
}

void GLpart::z2Changed(complex z2) {
  param_or_map->centerX = z2;
  emit parameterChanged();
}

void GLpart::recomputeLists() {
  makeList();
  // TODO: added updating pictures here.
  updateGL();
}

void GLpart::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (rysuje) {
    glColor3f(1.0, 1.0, 0.0);
  } else {
    glColor3f(1.0, 1.0, 1.0);
  }

  glLoadIdentity();

  gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glTranslatef(xCenCur, yCenCur, zCenCur);

  glRotated((GLdouble)xRotCur, 1.0, 0.0, 0.0);
  glRotated((GLdouble)yRotCur, 0.0, 1.0, 0.0);
  glRotated((GLdouble)zRotCur, 0.0, 0.0, 1.0);

  glCallList(knot);
}

void GLpart::resizeGL(int width, int height) {
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, 1.0, 0.01, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void GLpart::initializeGL() {
  knot = glGenLists(1);
  knot2 = glGenLists(1);
  makeList();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glColor3f(1.0, 1.0, 1.0);
  glLineWidth(lwidth);
}

GLpart::GLpart(QWidget *parent)
    : QGLWidget(parent), param_or_map(&swallowtail_p) {
  xRotCur = 0;
  yRotCur = 0;
  zRotCur = 0;

  xCenCur = 0;
  yCenCur = 0;
  zCenCur = 0;

  radius = 1.0;
  lwidth = 2.0;
  rysuje = false;

  initialize_maps(ParamVector);

  connect(this, SIGNAL(parameterChanged()), this, SLOT(recomputeLists()));
}

void GLpart::mousePressEvent(QMouseEvent *event) {
  mousePosition = event->pos();
}

void GLpart::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - mousePosition.x();
  int dy = event->y() - mousePosition.y();

  if (event->buttons() & Qt::LeftButton) {
    xRotCur += dy;
    yRotCur += dx;
    emit xRotChanged(xRotCur);
    emit yRotChanged(yRotCur);
    updateGL();
  }
  if (event->buttons() & Qt::RightButton) {
    xRotCur += dy;
    zRotCur += dx;
    emit xRotChanged(xRotCur);
    emit zRotChanged(zRotCur);
    updateGL();
  }
  mousePosition = event->pos();
}

void GLpart::wheelEvent(QWheelEvent *event) {
  zCenCur += float(event->delta()) / 120.0;
  if (zCenCur > 30.99) {
    zCenCur = 30.99;
  }
  if (zCenCur < -10) {
    zCenCur = 10.0;
  }
  emit zCenChanged(zCenCur);
  updateGL();
}

void GLpart::functionChanged(int newIndex) {

  if (newIndex == (int)ParamVector.size() - 1) {
    write_log("random function changed:");
    map *random = new map();
    param_or_map = random;
    write_log(random->get_name()); // TODO: log into file
  } else {
    param_or_map = ParamVector[newIndex];
  }
  emit parameterChanged();
}

void GLpart::functionChanged(map newMap) {
  param_or_map = &newMap;
  write_log(newMap.get_name());
  emit parameterChanged();
}

void GLpart::functionChanged(std::vector<fourvector> points, std::string name) {
  param_or_map = NULL;
  userPoints = points;
  userName = name;
  emit parameterChanged();
}

void GLpart::widthChanged(double d) {
  lwidth = d;
  emit parameterChanged();
}

void GLpart::parametricCentreChecked(complex z) {
  param_or_map->centerX = param_or_map->x(z);
  param_or_map->centerY = param_or_map->y(z);
  emit xCenChanged(param_or_map->centerX);
  emit yCenChanged(param_or_map->centerY);
  emit parameterChanged();
}

void GLpart::generatePictures(std::string name) {
  std::string command = "exec bash -c 'neato -T png -s400 graphs/" + name +
                        ".dot > pictures/" + name + ".png'";
  std::system(command.c_str());

  command = "exec bash -c 'rm -f pdpictures/" + name + ".svg'";
  std::system(command.c_str());

  command = "pyenv/bin/python2.7 ./pdcodes/" + name + ".py";
  write_log(command);
  std::system(command.c_str());
}

#pragma clang diagnostic pop