#include "GLpart.h"
#include "Knot.h"
#include "graphs.h"
#include <GL/glut.h>
#include <QGLWidget>
#include <QtGui>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
complex function::centerX(0.0, 0.0);
complex function::centerY(0.0, 0.0);

void GLpart::makeList() {
  if (glIsList(knot2) == GL_TRUE)
    glDeleteLists(knot2, 1);
  knot2 = glGenLists(1);

  rysuje = true;
  glNewList(knot2, GL_COMPILE);

  Knot newKnot(radius, param_or_map);
  newKnot.draw();
  newKnot.dumpPoints();

  newKnot.giveIntersectionNumber();

  std::map<int, double> angle_map;
  auto PDCode = newKnot.givePDCode(angle_map);
  printPDCode(PDCode, param_or_map);

  auto graph = generateGraphFromPDCode(PDCode, angle_map);
  std::string name = param_or_map->nameofmap.toUtf8().constData();
  printGraph(graph, name);
  generatePictures(name);

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
  parametr::centerX = z1;
  emit parameterChanged();
}

void GLpart::z2Changed(complex z2) {
  parametr::centerY = z2;
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

void GLpart::widthChanged(double d) {
  lwidth = d;
  emit parameterChanged();
}

void GLpart::parametricCentreChecked(complex z) {
  parametr::centerX = param_or_map->x(z);
  parametr::centerY = param_or_map->y(z);
  emit xCenChanged(function::centerX);
  emit yCenChanged(function::centerY);
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