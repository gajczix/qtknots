#include <complex>

#include "functions.h"
/// here we declare all functions. The way to add a new function is the
/// following: first one need to define three functions of type
/// complex(complex, complex) : complex means complex in Polish: first
/// one is the function two next must be partial derivatives then one declares a
/// map by map new_function(f,fx,fy,QString("name")); and then one adds
/// MapsVector.push_back(new_function).

complex jaskolkax(complex z) { return z * z * z - z * 3.0; }
complex jaskolkay(complex z) { return z * z * z * z - z * z * 2.0; }
complex jaskolkadx(complex z) { return z * z * 3.0 - 3.0; }
complex jaskolkady(complex z) { return z * z * z * 4.0 - z * 4.0; }

complex t23_x(complex z) { return z * z; }
complex t23_y(complex z) { return z * z * z; }
complex t23_dx(complex z) { return z * 2.0; }
complex t23_dy(complex z) { return z * z * 3.0; }

complex t34_x(complex z) { return z * z * z; }
complex t34_y(complex z) { return z * z * z * z; }
complex t34_dx(complex z) { return z * z * 3.0; }
complex t34_dy(complex z) { return z * z * z * 4.0; }

complex t35_x(complex z) { return z * z * z; }
complex t35_y(complex z) { return z * z * z * z * z; }
complex t35_dx(complex z) { return z * z * 3.0; }
complex t35_dy(complex z) { return z * z * z * z * 5.0; }

complex t25_x(complex z) { return z * z; }
complex t25_y(complex z) { return z * z * z * z * z; }
complex t25_dx(complex z) { return z * 2.0; }
complex t25_dy(complex z) { return z * z * z * z * 5.0; }

complex petlax(complex z) { return z * z + 1; }
complex petlay(complex z) { return z * z * z + z; }
complex petladx(complex z) { return z * 2.0; }
complex petlady(complex z) { return z * z * 3.0 + 1.0; }

complex T234_x(complex t) { return t * t + t * t * t; }
complex T234_y(complex t) { return t * t * t * t; }
complex T234_dx(complex t) { return 2 * t + 3 * t * t; }
complex T234_dy(complex t) { return 4 * t * t * t; }

complex T235_x(complex t) { return t * t; }
complex T235_y(complex t) { return t * t * t + t * t * t * t * t; }
complex T235_dx(complex t) { return 2 * t; }
complex T235_dy(complex t) { return 3 * t * t + 5 * t * t * t * t; }

parametr jaskp(jaskolkax, jaskolkay, jaskolkadx, jaskolkady,
               QString("Swallowtail"));
map jask({{{4, 0}, 1},
          {{2, 1}, 6},
          {{0, 3}, -1},
          {{2, 0}, -2},
          {{0, 2}, 6},
          {{0, 1}, -9}},
         QString("map:Swallowtail"));

parametr Petlap(petlax, petlay, petladx, petlady, QString("y^2=x^3-x^2"));
map Petla({{{3, 0}, 1}, {{2, 0}, -1}, {{0, 2}, -1}},
          QString("map:y^2=x^3-x^2"));

parametr T23p(t23_x, t23_y, t23_dx, t23_dy, QString("x^2-y^3"));
map T23({{{2, 0}, 1}, {{0, 3}, -1}}, QString("map:x^2-y^3"));

parametr T25p(t25_x, t25_y, t25_dx, t25_dy, QString("x^2-y^5"));

map T25({{{2, 0}, 1}, {{0, 5}, -1}}, QString("map:x^2-y^5"));

parametr T34p(t34_x, t34_y, t34_dx, t34_dy, QString("x^3-y^4"));
map T34({{{3, 0}, 1}, {{0, 4}, -1}}, QString("map:x^3-y^4"));

parametr T35p(t35_x, t35_y, t35_dx, t35_dy, QString("x^3-y^5"));
map T35({{{3, 0}, 1}, {{0, 5}, -1}}, QString("map:x^3-y^5"));

parametr T234p(T234_x, T234_y, T234_dx, T234_dy, QString("T234"));
map T234({{{4, 0}, 1}, {{2, 1}, -2}, {{1, 2}, -4}, {{0, 3}, -1}, {{0, 2}, 1}},
         QString("map:T234"));

parametr T235p(T235_x, T235_y, T235_dx, T235_dy, QString("x^3-y^5"));
// map T235(, QString("x^3-y^5"));

map RandomFunc;

using namespace std::complex_literals;

map Interesting({{{0, 0}, (-0.445727 + 1.305957i)},
                 {{0, 2}, -0.126224 + 2.484937i},
                 {{0, 4}, -6.283594 + 2.806313i},
                 {{0, 6}, 4.895018 + -1.599020i},
                 {{0, 8}, 9.697923 + -3.086152i},
                 {{2, 0}, -9.298484 + -9.303625i},
                 {{2, 2}, -9.813268 + -6.921233i},
                 {{2, 4}, 1.963476 + -8.754081i},
                 {{2, 6}, -7.807782 + 5.255985i},
                 {{2, 8}, 9.265095 + 2.338173i},
                 {{4, 0}, -3.345158 + -3.025733i},
                 {{4, 2}, -3.145766 + -1.906441i},
                 {{4, 4}, -3.164222 + -9.985651i},
                 {{4, 6}, -9.193600 + 8.403399i},
                 {{8, 8}, 4.594428 + -7.025386i}},
                // (-6.136970 + -2.485943i)*x^4*y^8 + (6.717219 + 1.018566i)*x^6
                // + (1.520688 + 1.075249i)*x^6*y^2 + (9.310661 +
                // -4.923976i)*x^6*y^4 + (4.684237 + -0.851471i)*x^6*y^6 +
                // (-9.028730 + 9.113870i)*x^6*y^8 + (-5.260118 + 3.021248i)*x^8
                // + (9.528650 + 8.086544i)*x^8*y^2 + (-5.772620 +
                // 0.817794i)*x^8*y^4 + (-1.306335 + -8.883338i)*x^8*y^6

                QString("Interesting1"));

map Interesting2(
    {{{0, 0}, 1.092680 + 1.609801i},   {{0, 2}, -4.442457 + 1.562065i},
     {{0, 4}, 9.310276 + -9.916809i},  {{0, 6}, 8.181128 + -5.232525i},
     {{0, 8}, -5.658343 + -9.787264i}, {{0, 10}, -4.989713 + -4.944224i},
     {{2, 0}, 7.981720 + -6.608460i},  {{2, 2}, -2.594808 + 2.386617i},
     {{2, 4}, -5.189628 + 6.640252i},  {{2, 6}, -4.080489 + 7.778512i},
     {{2, 8}, -7.426485 + -7.251356i}, {{2, 10}, -5.921270 + -6.466096i},
     {{4, 0}, 6.408915 + -5.207812i},  {{4, 2}, 1.675021 + -3.505232i},
     {{4, 4}, -9.034220 + 8.369648i},  {{4, 6}, 6.424003 + -9.330741i},
     {{4, 8}, 6.898724 + 2.143633i},   {{4, 10}, -8.806820 + 7.749968i},
     {{6, 0}, 6.756754 + -0.234435i},  {{6, 2}, 9.153172 + -8.956835i},
     {{6, 4}, -6.601955 + -0.648699i}, {{6, 6}, 4.461275 + 9.924258i},
     {{6, 8}, -0.123056 + -8.681718i}, {{6, 10}, 5.801778 + -8.813896i},
     {{8, 0}, -3.798988 + -6.980100i}, {{8, 2}, 0.420889 + 6.597741i},
     {{8, 4}, -9.886540 + 7.141485i},  {{8, 6}, 2.001604 + 9.288251i},
     {{8, 8}, 4.982076 + 1.294225i},   {{8, 10}, -4.032252 + 2.452472i}},
    QString("Interesting2"));

map Interesting3(
    {{{0, 0}, -3.529319 + -5.840687i}, {{0, 2}, -5.017438 + 1.529616i},
     {{0, 4}, 0.813702 + -5.649347i},  {{0, 6}, -8.913331 + -5.576684i},
     {{0, 8}, 8.265174 + -8.529051i},  {{2, 0}, -9.428959 + 4.095269i},
     {{2, 2}, 0.266663 + -8.503811i},  {{2, 4}, -9.102416 + -5.250801i},
     {{2, 6}, 9.301825 + -3.103411i},  {{2, 8}, 1.273012 + 4.504673i},
     {{4, 0}, 9.176727 + -1.100263i},  {{4, 2}, -4.355078 + 9.850358i},
     {{4, 4}, 3.375454 + -7.338168i},  {{4, 6}, -4.677304 + 5.850884i},
     {{4, 8}, 0.453919 + 3.374598i},   {{6, 0}, -0.246426 + 0.888249i},
     {{6, 2}, 1.722071 + -7.298277i},  {{6, 4}, 0.053659 + -2.163003i},
     {{6, 6}, 1.228273 + 4.197258i},   {{6, 8}, -3.600711 + -3.138550i},
     {{8, 0}, -9.481358 + 9.511357i},  {{8, 2}, -3.370252 + 4.729921i},
     {{8, 4}, -6.140815 + -2.676954i}, {{8, 6}, -3.845788 + 1.023939i},
     {{8, 8}, 5.920479 + -9.497867i}},
    QString("Interesting3"));

map Interesting4(
    {{{0, 0}, -1.372483 + -5.411576i}, {{0, 2}, -8.494251 + -0.237448i},
     {{0, 4}, 1.458648 + -9.356816i},  {{0, 6}, -7.472253 + -3.374977i},
     {{0, 8}, -8.798095 + -1.536817i}, {{0, 10}, 2.880959 + -7.552239i},
     {{2, 0}, 5.213842 + 9.634104i},   {{2, 2}, -1.952278 + -0.463829i},
     {{2, 4}, -2.280932 + -5.104348i}, {{2, 6}, 1.117925 + -3.592869i},
     {{2, 8}, -0.180352 + -5.294336i}, {{2, 10}, 7.747806 + -8.106320i},
     {{4, 0}, -8.621670 + 5.719436i},  {{4, 2}, 3.117388 + -6.732934i},
     {{4, 4}, 9.899774 + -3.599045i},  {{4, 6}, 3.399345 + 7.650476i},
     {{4, 8}, 9.637333 + 9.373734i},   {{4, 10}, 2.877956 + 6.410430i},
     {{6, 0}, -1.231304 + 5.151193i},  {{6, 2}, 8.966407 + 7.403212i},
     {{6, 4}, -1.485948 + -1.168142i}, {{6, 6}, -7.025048 + -3.394820i},
     {{6, 8}, -0.162226 + -1.305423i}, {{6, 10}, 4.960398 + 2.020938i},
     {{8, 0}, -9.355703 + -4.624793i}, {{8, 2}, 7.946058 + -4.682592i},
     {{8, 4}, 2.887755 + -0.704872i},  {{8, 6}, 5.247914 + 2.465825i},
     {{8, 8}, -5.383826 + 6.921710i},  {{8, 10}, 1.922615 + -8.315015i}},
    QString("Interesting4"));

map Intersting5(
    {{{0, 0}, 2.791791 + -2.950850i},  {{0, 1}, 9.869611 + 4.844128i},
     {{0, 2}, 2.086545 + -1.915346i},  {{0, 3}, -2.515778 + -0.795698i},
     {{0, 4}, 3.459814 + -6.834822i},  {{0, 5}, -6.015559 + -2.039722i},
     {{0, 6}, -9.843064 + 7.695824i},  {{0, 7}, -1.301179 + -9.603149i},
     {{0, 8}, 2.672426 + 9.511181i},   {{0, 9}, -6.328956 + -7.966485i},
     {{1, 0}, 0.815058 + -4.328849i},  {{1, 1}, -0.290573 + -9.473793i},
     {{1, 2}, 2.196230 + -5.270614i},  {{1, 3}, -1.574231 + -3.178319i},
     {{1, 4}, 9.319356 + -9.115336i},  {{1, 5}, -2.041510 + 3.055021i},
     {{1, 6}, -2.669460 + -6.756427i}, {{1, 7}, 1.602669 + 4.538198i},
     {{1, 8}, -7.203805 + -1.294344i}, {{1, 9}, -2.100310 + 2.650100i},
     {{2, 0}, -0.647328 + 6.034550i},  {{2, 1}, -5.042861 + 3.747867i},
     {{2, 2}, 2.639626 + -4.885490i},  {{2, 3}, 7.272000 + 5.143133i},
     {{2, 4}, 2.341762 + -7.453547i},  {{2, 5}, -9.631450 + -6.152184i},
     {{2, 6}, -0.487082 + -6.550239i}, {{2, 7}, 9.541763 + -7.734029i},
     {{2, 8}, -0.717691 + 4.549867i},  {{2, 9}, -4.353593 + -8.142097i},
     {{3, 0}, -8.633817 + -5.166875i}, {{3, 1}, 2.496144 + -9.738506i},
     {{3, 2}, -1.554936 + -1.002070i}, {{3, 3}, -7.582189 + -4.240116i},
     {{3, 4}, 9.285443 + 0.733155i},   {{3, 5}, -6.488918 + 9.371996i},
     {{3, 6}, -4.861807 + -2.705342i}, {{3, 7}, -8.522552 + -3.450155i},
     {{3, 8}, -6.759099 + -0.362181i}, {{3, 9}, 9.608621 + -0.750347i},
     {{4, 0}, -7.421218 + -4.639158i}, {{4, 1}, 2.734351 + -1.980986i},
     {{4, 2}, -9.033577 + -7.912040i}, {{4, 3}, -7.535711 + 3.827568i},
     {{4, 4}, -9.273763 + -7.464012i}, {{4, 5}, 6.279129 + 3.910190i},
     {{4, 6}, 3.486459 + -9.988578i},  {{4, 7}, -9.835356 + 2.771492i},
     {{4, 8}, -0.516406 + -5.476796i}, {{4, 9}, -9.437694 + -8.257660i},
     {{5, 0}, 3.314171 + -6.529399i},  {{5, 1}, -5.258795 + 3.789131i},
     {{5, 2}, 8.969411 + -6.526010i},  {{5, 3}, 8.739194 + -8.837056i},
     {{5, 4}, 9.358864 + -9.342920i},  {{5, 5}, -8.654747 + 2.529654i},
     {{5, 6}, -6.351143 + 5.688032i},  {{5, 7}, -9.290371 + -1.808041i},
     {{5, 8}, -2.927579 + -7.033728i}, {{5, 9}, 1.041587 + 1.884049i},
     {{6, 0}, 9.231196 + -9.060773i},  {{6, 1}, -8.423339 + -9.536142i},
     {{6, 2}, 6.550969 + -1.418628i},  {{6, 3}, -7.882529 + -5.460866i},
     {{6, 4}, 0.598770 + -8.773855i},  {{6, 5}, -2.169971 + -8.499645i},
     {{6, 6}, -4.988337 + -3.296157i}, {{6, 7}, 4.488944 + 2.510234i},
     {{6, 8}, -9.140825 + -6.552156i}, {{6, 9}, -1.197545 + -7.291459i},
     {{7, 0}, 7.277166 + 6.065219i},   {{7, 1}, -4.979843 + -9.377443i},
     {{7, 2}, -9.062391 + 1.723393i},  {{7, 3}, -6.553419 + -0.949614i},
     {{7, 4}, -6.423164 + -2.192863i}, {{7, 5}, -6.698600 + 7.120139i},
     {{7, 6}, 8.317988 + -2.826444i},  {{7, 7}, -3.774402 + -1.774503i},
     {{7, 8}, 2.413957 + 3.339665i},   {{7, 9}, -7.539226 + 9.165220i},
     {{8, 0}, -1.221018 + 6.222412i},  {{8, 1}, -4.437440 + -0.014377i},
     {{8, 2}, -7.476546 + -9.637326i}, {{8, 3}, 2.461041 + 0.114361i},
     {{8, 4}, 8.735171 + 7.969363i},   {{8, 5}, 0.458327 + -8.430416i},
     {{8, 6}, -1.104259 + -7.085122i}, {{8, 7}, -3.748450 + 4.892603i},
     {{8, 8}, 0.765109 + -6.302566i},  {{8, 9}, 6.079230 + -5.730587i},
     {{9, 0}, -1.411414 + 5.985458i},  {{9, 1}, 7.382652 + -3.444518i},
     {{9, 2}, 2.067488 + -0.087832i},  {{9, 3}, 3.502087 + -5.026988i},
     {{9, 4}, -8.652202 + 7.966708i},  {{9, 5}, -1.213913 + -3.203459i},
     {{9, 6}, -5.197824 + -2.262414i}, {{9, 7}, 5.475765 + 9.442405i},
     {{9, 8}, -2.615598 + -3.443613i}, {{9, 9}, -4.197854 + -9.089752i}},
    QString("Interesting5"));

void initialize_maps(std::vector<function *> &pv) {
  std::vector<function *> ParametrVector;

  ParametrVector.push_back(&jaskp);
  ParametrVector.push_back(&Petlap);
  ParametrVector.push_back(&T23p);
  ParametrVector.push_back(&T25p);
  ParametrVector.push_back(&T34p);
  ParametrVector.push_back(&T35p);
  ParametrVector.push_back(&T234p);
  ParametrVector.push_back(&T235p);
  ParametrVector.push_back(&jask);
  ParametrVector.push_back(&Petla);
  ParametrVector.push_back(&T23);
  ParametrVector.push_back(&T25);
  ParametrVector.push_back(&T34);
  ParametrVector.push_back(&T35);
  ParametrVector.push_back(&T234);
  ParametrVector.push_back(&Interesting);
  ParametrVector.push_back(&Interesting2);
  ParametrVector.push_back(&Interesting3);
  ParametrVector.push_back(&Interesting4);
  ParametrVector.push_back(&Intersting5);
  ParametrVector.push_back(&RandomFunc);
  write_log(RandomFunc.get_name());

  Interesting4.printFriendlyCoefMatrix();
  Interesting4.printCoefMatrix();
  RandomFunc.printFriendlyCoefMatrix();
  std::complex<double> liczba_std = 8.283527 + -2.195920i;
  complex liczba = complex(liczba_std.real(), liczba_std.imag());
  write_log("ostatni test" +  liczba.to_string());

  pv = ParametrVector;
}
