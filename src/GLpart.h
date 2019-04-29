#ifndef GLCZESC_H
#define GLCZESC_H
#include "src/math_basics/sample-func.h"
#include "src/math_basics/matbase.h"
#include <QGLWidget>

class GLpart : public QGLWidget {
  Q_OBJECT

public:
  GLpart(QWidget *parent = 0);
  ~GLpart();
  double getLWidth();
  int DNindex = 0;

protected:
  void paintGL();
  void initializeGL();
  void resizeGL(int width, int height);
  QPoint mousePosition;
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  int xRotCur;
  int yRotCur;
  int zRotCur;

  float xCenCur;
  float yCenCur;
  float zCenCur;

  double radius;
  double lwidth;
  bool rysuje;

  /// currently used function
  function *param_or_map;
  std::vector<function *> ParamVector;

  /// pointers to the list
  GLuint knot;
  GLuint knot2;

  /// Creation of the list.
  void makeList();
  /// list of fun_matrix
signals:

  void parameterChanged();
  void xRotChanged(int);
  void yRotChanged(int);
  void zRotChanged(int);
  void zCenChanged(float);
  void xCenChanged(complex);
  void yCenChanged(complex);

public slots:

  void xRotated(int);
  void yRotated(int);
  void zRotated(int);

  void xCentered(float);
  void yCentered(float);
  void zCentered(float);

  void rChanged(double);
  void z1Changed(complex);
  void z2Changed(complex);

  void functionChanged(int);
  void functionChanged(map);
  void widthChanged(double);

  void recomputeLists();
  void parametricCentreChecked(complex);

  void generatePictures(std::string name);
};
#endif
