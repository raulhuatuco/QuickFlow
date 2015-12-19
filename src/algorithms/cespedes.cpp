#include "cespedes.h"
#include <QList>
#include <QPair>
#include <QElapsedTimer>
#include <cmath>
#include "math_constants.h"
#include "models/bar.h"
#include "models/line.h"
#include "models/network.h"

Cespedes::Cespedes(Network *network) :
  network_(network)
{
  totalLoss = 0.0;
}

bool Cespedes::solve()
{
  // Create a timer to measure the duration and start it.
  QElapsedTimer timer;
  timer.start();

  // Initialize at iteration 0 and error inf;
  uint32_t iteration = 0;
  double iterationError = qInf();

  // Create a radial layer to operate the algorithm.
  RadialLayer radLayer(network_);
  radLayer.calculate();

  flatStart();

  do {
    // Calculates the backward sweep.
    doBackwardSweep(radLayer);

    // Calculates the forward sweep.
    doForwardSweep(radLayer);

    // Calculate Slack current.
    computeLosses();

    // Calculate the iteration error.
    iterationError = maxError();

    iteration++;
  } while(( (network_->powerBase()*iterationError) > network_->minError) &&
          (iteration < network_->maxIterations));

  saveLineLoss();

  calcBarCurrent();

  calcLineCurrent();

  usedIterations = iteration;
  duration = timer.elapsed();

  if (iteration < network_->maxIterations)
    return true;
  else
    return false;
}

void Cespedes::flatStart()
{
  // Set Initial voltage to 1pu. fill barPower.
  double initialVoltage = network_->voltageBase();
  foreach(Bar *bar, network_->bars) {
    bar->setV(0, std::polar(initialVoltage, 0.0), Unit::kVolts);
    bar->setV(1, std::polar(initialVoltage, 0.0), Unit::kVolts);
    bar->setV(2, std::polar(initialVoltage, 0.0), Unit::kVolts);
    barPower[0].insert(bar, 0.0);
    barPower[1].insert(bar, 0.0);
    barPower[2].insert(bar, 0.0);

    bar->setRI(0, 0.0);
    bar->setRI(1, 0.0);
    bar->setRI(2, 0.0);
  }

  foreach (Line *line, network_->lines) {
    lineLoss[0].insert(line, 0.0);
    lineLoss[1].insert(line, 0.0);
    lineLoss[2].insert(line, 0.0);

    // Reset current.
    line->setI(0, 0.0);
    line->setI(1, 0.0);
    line->setI(2, 0.0);

    // Reset loss.
    line->setLoss(0, 0.0);
    line->setLoss(1, 0.0);
    line->setLoss(2, 0.0);
  }
}

void Cespedes::doBackwardSweep(RadialLayer &radLayer)
{
  for (int i = (radLayer.width()-1); i>0; i--) {

// Process every bar in the layer.
    foreach(Bar *bar, *radLayer.layers.at(i)) {
      complex<double> barS[3];

      barS[0] = bar->si(0) - bar->sh(0);
      barS[1] = bar->si(1) - bar->sh(1);
      barS[2] = bar->si(2) - bar->sh(2);

      foreach (Line *line, bar->lines) {
        if(radLayer.layers.at(i-1)->indexOf(line->pNoI()) == -1) {
          barS[0] += barPower[0].value(line->pNoF());
          barS[1] += barPower[1].value(line->pNoF());
          barS[2] += barPower[2].value(line->pNoF());

          barS[0] += lineLoss[0][line];
          barS[1] += lineLoss[1][line];
          barS[2] += lineLoss[2][line];
        }
      }
      barPower[0][bar] = barS[0];
      barPower[1][bar] = barS[1];
      barPower[2][bar] = barS[2];
    }
  }
}

void Cespedes::doForwardSweep(RadialLayer &radLayer)
{
  for (int i = 1; i< radLayer.width(); i++) {
    foreach(Bar *bar, *radLayer.layers.at(i)) {

      Line *line;

      foreach(Line *montanteLine, bar->lines) {
        if(radLayer.layers.at(i-1)->indexOf(montanteLine->pNoI()) != -1) {
          line = montanteLine;
          break;
        }
      }

      double P[3];
      P[0] = barPower[0][bar].real();
      P[1] = barPower[1][bar].real();
      P[2] = barPower[2][bar].real();


      double Q[3];
      Q[0] = barPower[0][bar].imag();
      Q[1] = barPower[1][bar].imag();
      Q[2] = barPower[2][bar].imag();

      double R[6];
      R[0] = line->z(0).real();
      R[1] = line->z(1).real();
      R[2] = line->z(2).real();
      R[3] = line->z(3).real();
      R[4] = line->z(4).real();
      R[5] = line->z(5).real();

      double X[6];
      X[0] = line->z(0).imag();
      X[1] = line->z(1).imag();
      X[2] = line->z(2).imag();
      X[3] = line->z(3).imag();
      X[4] = line->z(4).imag();
      X[5] = line->z(5).imag();

      double Vs[3];
      Vs[0] = abs(line->pNoI()->rV(0));
      Vs[1] = abs(line->pNoI()->rV(1));
      Vs[2] = abs(line->pNoI()->rV(2));

      double b[3];
      double c[3];

      b[0] = 2*(P[0]*R[0] + Q[0]*X[0] + P[1]*R[1] + Q[1]*X[1] + P[2]*R[2] + Q[2]*X[2])
             - Vs[0]*Vs[0];

      c[0] = (P[0]*P[0] + Q[0]*Q[0])*(R[0]*R[0] + X[0]*X[0]) +
             (P[1]*P[1] + Q[1]*Q[1])*(R[1]*R[1] + X[1]*X[1]) + (P[2]*P[2] + Q[2]*Q[2])*
             (R[2]*R[2] + X[2]*X[2]);

      b[1] = 2*(P[0]*R[1] + Q[0]*X[1] + P[1]*R[3] + Q[1]*X[3] + P[2]*R[4] + Q[2]*X[4])
             - Vs[1]*Vs[1];

      c[1] = (P[0]*P[0] + Q[0]*Q[0])*(R[1]*R[1] + X[1]*X[1]) +
             (P[1]*P[1] + Q[1]*Q[1])*(R[3]*R[3] + X[3]*X[3]) + (P[2]*P[2] + Q[2]*Q[2])*
             (R[4]*R[4] + X[4]*X[4]);

      b[2] = 2*(P[0]*R[2] + Q[0]*X[2] + P[1]*R[4] + Q[1]*X[4] + P[2]*R[5] + Q[2]*X[5])
             - Vs[2]*Vs[2];

      c[2] = (P[0]*P[0] + Q[0]*Q[0])*(R[2]*R[2] + X[2]*X[2]) +
             (P[1]*P[1] + Q[1]*Q[1])*(R[4]*R[4] + X[4]*X[4]) + (P[2]*P[2] + Q[2]*Q[2])*
             (R[5]*R[5] + X[5]*X[5]);

      double Vr[3];
      Vr[0] = solveBiquadratic(b[0], c[0]);
      Vr[1] = solveBiquadratic(b[1], c[1]);
      Vr[2] = solveBiquadratic(b[2], c[2]);

      bar->setRV(0, std::polar(Vr[0], 0.0));
      bar->setRV(1, std::polar(Vr[1], 0.0));
      bar->setRV(2, std::polar(Vr[2], 0.0));
    }
  }
}

void Cespedes::computeLosses()
{
  foreach(Line *line, network_->lines) {


    double P[3];
    P[0] = barPower[0][line->pNoF()].real();
    P[1] = barPower[1][line->pNoF()].real();
    P[2] = barPower[2][line->pNoF()].real();


    double Q[3];
    Q[0] = barPower[0][line->pNoF()].imag();
    Q[1] = barPower[1][line->pNoF()].imag();
    Q[2] = barPower[2][line->pNoF()].imag();

    double R[3];
    R[0] = line->z(0).real();
    R[1] = line->z(3).real();
    R[2] = line->z(5).real();

    double X[3];
    X[0] = line->z(0).imag();
    X[1] = line->z(3).imag();
    X[2] = line->z(5).imag();

//    double R[3];
//    R[0] = line->z(0).real();
//    R[1] = line->z(1).real();
//    R[2] = line->z(2).real();
//    R[3] = line->z(3).real();
//    R[4] = line->z(4).real();
//    R[5] = line->z(5).real();

//    double X[3];
//    X[0] = line->z(0).imag();
//    X[1] = line->z(1).imag();
//    X[2] = line->z(2).imag();
//    X[3] = line->z(3).imag();
//    X[4] = line->z(4).imag();
//    X[5] = line->z(5).imag();


    double Vr[3];
    Vr[0] = abs(line->pNoF()->rV(0));
    Vr[1] = abs(line->pNoF()->rV(1));
    Vr[2] = abs(line->pNoF()->rV(2));

    double Lp[3], Lq[3];

    Lp[0] = R[0]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]);
    Lp[1] = R[1]*(P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]);
    Lp[2] = R[2]*(P[2]*P[2] + Q[2]*Q[2])/(Vr[2]*Vr[2]);

    Lq[0] = X[0]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]);
    Lq[1] = X[1]*(P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]);
    Lq[2] = X[2]*(P[2]*P[2] + Q[2]*Q[2])/(Vr[2]*Vr[2]);


//    Lp[0] = R[0]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]) + R[1]*
//            (P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]) + R[2]*(P[2]*P[2] + Q[2]*Q[2])/
//            (Vr[2]*Vr[2]);
//    Lp[1] = R[1]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]) + R[3]*
//            (P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]) + R[4]*(P[2]*P[2] + Q[2]*Q[2])/
//            (Vr[2]*Vr[2]);
//    Lp[2] = R[2]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]) + R[4]*
//            (P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]) + R[5]*(P[2]*P[2] + Q[2]*Q[2])/
//            (Vr[2]*Vr[2]);

//    Lq[0] = X[0]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]) + X[1]*
//            (P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]) + X[2]*(P[2]*P[2] + Q[2]*Q[2])/
//            (Vr[2]*Vr[2]);
//    Lq[1] = X[1]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]) + X[3]*
//            (P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]) + X[4]*(P[2]*P[2] + Q[2]*Q[2])/
//            (Vr[2]*Vr[2]);
//    Lq[2] = X[2]*(P[0]*P[0] + Q[0]*Q[0])/(Vr[0]*Vr[0]) + X[4]*
//            (P[1]*P[1] + Q[1]*Q[1])/(Vr[1]*Vr[1]) + X[5]*(P[2]*P[2] + Q[2]*Q[2])/
//            (Vr[2]*Vr[2]);


    lineLoss[0][line] = complex<double> (Lp[0], Lq[0]);
    lineLoss[1][line] = complex<double> (Lp[1], Lq[1]);
    lineLoss[2][line] = complex<double> (Lp[2], Lq[2]);



  }
}

double Cespedes::maxError()
{
  complex<double> newTotalLoss;

  foreach(complex<double> loss, lineLoss[0]) {
    newTotalLoss += loss;
  }
  foreach(complex<double> loss, lineLoss[1]) {
    newTotalLoss += loss;
  }
  foreach(complex<double> loss, lineLoss[2]) {
    newTotalLoss += loss;
  }

  double errP =  fabs(newTotalLoss.real() - totalLoss.real());
  double errQ =  fabs(newTotalLoss.imag() - totalLoss.imag());

  totalLoss = newTotalLoss;

  return (errP > errQ) ? errP : errQ;
}

double Cespedes::solveBiquadratic(double b, double c)
{
  double d, x1, x2, r;

  d = sqrt(b*b - 4*c);
  x1 = (-b + d) / 2;
  x2 = (-b - d) / 2;

  r = (x1>0) ? x1 : x2;

  return sqrt(r);
}

void Cespedes::calcLineCurrent()
{
  complex<double> i[3];

  foreach(Line *line, network_->lines) {
    if((abs(line->loss(0))  > 0.0) || (abs(line->z(0)) > 0.0))
      i[0] = sqrt(abs(line->loss(0)/line->z(0)));
    else
      i[0] = 0.0;

    if((abs(line->loss(1))  > 0.0) || (abs(line->z(3)) > 0.0))
      i[1] = sqrt(abs(line->loss(1)/line->z(3)));
    else
      i[1] = 0.0;

    if((abs(line->loss(2))  > 0.0) || (abs(line->z(5)) > 0.0))
      i[2] = sqrt(abs(line->loss(2)/line->z(5)));
    else
      i[2] = 0.0;

    line->setI(0, i[0]);
    line->setI(1, i[1]);
    line->setI(2, i[2]);
  }
}

void Cespedes::calcBarCurrent()
{
  complex<double> i[3];

  foreach(Bar *bar, network_->bars) {
    i[0] = (abs(bar->si(0) - bar->sh(0))/ abs(bar->rV(0)));
    i[1] = (abs(bar->si(1) - bar->sh(1))/ abs(bar->rV(1)));
    i[2] = (abs(bar->si(2) - bar->sh(2))/ abs(bar->rV(2)));

    bar->setRI(0, i[0]);
    bar->setRI(1, i[1]);
    bar->setRI(2, i[2]);
  }
}

void Cespedes::saveLineLoss()
{
  foreach(Line *line, network_->lines) {
    line->setLoss(0, lineLoss[0][line]);
    line->setLoss(1, lineLoss[1][line]);
    line->setLoss(2, lineLoss[2][line]);
  }
}

