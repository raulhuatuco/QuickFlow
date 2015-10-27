#include "shirmoharmmadi.h"

#include <QList>
#include <QElapsedTimer>

#include "radiallayer.h"
#include "models/bar.h"
#include "models/line.h"

Shirmoharmmadi::Shirmoharmmadi(Network *network) :
  network_(network)
{

}

Shirmoharmmadi::~Shirmoharmmadi()
{

}

bool Shirmoharmmadi::solve()
{
  QElapsedTimer timer;
  timer.start();

  RadialLayer radLayer(network_);

  radLayer.calculate();

  uint32_t iteration = 0;

  foreach(Bar *bar, network_->bars) {
    bar->setRV(0, bar->v(0));
    bar->setRV(1, bar->v(1));
    bar->setRV(2, bar->v(2));
  }

  QList<Line *> processedLines;
  double maxError;

  do {

    //Backward sweep.
    // Calculate line current.
    for(int i = radLayer.width() -1; i>0; i--) {

      // Process every bar in the layer.
      foreach(Bar *bar, *radLayer.layers.at(i)) {

        // Process every line of the bar.
        foreach(Line *line, bar->lines) {

          // Check if line already have I.
          if(processedLines.indexOf(line) == -1) {

            // If not, sum every other lines I.
            complex<double> iLine[3];

            foreach(Line *oline, bar->lines) {
              if(oline != line) {
                iLine[0] += oline->i(0, Unit::kAmperePerUnit);
                iLine[1] += oline->i(1, Unit::kAmperePerUnit);
                iLine[2] += oline->i(2, Unit::kAmperePerUnit);
              }
            }

            // Remove Bar I.
            iLine[0] -= bar->rI(0, Unit::kAmperePerUnit);
            iLine[1] -= bar->rI(1, Unit::kAmperePerUnit);
            iLine[2] -= bar->rI(2, Unit::kAmperePerUnit);

            // Set line current.
            line->setI(0, iLine[0], Unit::kAmperePerUnit);
            line->setI(1, iLine[1], Unit::kAmperePerUnit);
            line->setI(2, iLine[2], Unit::kAmperePerUnit);
            processedLines.append(line);
          }
        }
      }
    }

    processedLines.clear();

    // Forward sweep.
    for(int i=0; i<radLayer.width() -1; i++) {

      // Go trough every bar.
      foreach(Bar *bar, *radLayer.layers[i]) {

        // Process every line of the bar.
        foreach(Line *line, bar->lines) {

          // Check if line has been processed.
          if(processedLines.indexOf(line) == -1) {

            complex<double> bVoltage[3];
            bVoltage[0] = bar->rV(0,Unit::kVoltsPerUnit);
            bVoltage[1] = bar->rV(1,Unit::kVoltsPerUnit);
            bVoltage[2] = bar->rV(2,Unit::kVoltsPerUnit);

            bVoltage[0] += line->z(0, Unit::kOhmPerUnit)*line->i(0,Unit::kAmperePerUnit);
            bVoltage[0] += line->z(1, Unit::kOhmPerUnit)*line->i(1, Unit::kAmperePerUnit);
            bVoltage[0] += line->z(2,Unit::kOhmPerUnit)*line->i(2, Unit::kAmperePerUnit);

            bVoltage[1] += line->z(1, Unit::kOhmPerUnit)*line->i(0,Unit::kAmperePerUnit);
            bVoltage[1] += line->z(3, Unit::kOhmPerUnit)*line->i(1, Unit::kAmperePerUnit);
            bVoltage[1] += line->z(4,Unit::kOhmPerUnit)*line->i(2, Unit::kAmperePerUnit);

            bVoltage[2] += line->z(2, Unit::kOhmPerUnit)*line->i(0,Unit::kAmperePerUnit);
            bVoltage[2] += line->z(4, Unit::kOhmPerUnit)*line->i(1, Unit::kAmperePerUnit);
            bVoltage[2] += line->z(5,Unit::kOhmPerUnit)*line->i(2, Unit::kAmperePerUnit);

            line->pNoF()->setRV(0, bVoltage[0], Unit::kVoltsPerUnit);
            line->pNoF()->setRV(1, bVoltage[1], Unit::kVoltsPerUnit);
            line->pNoF()->setRV(2, bVoltage[2], Unit::kVoltsPerUnit);

            processedLines.append(line);
          }
        }
      }
    }

// Calculate Slack current.
    Bar *slack = network_->getBarById(0);

    complex<double> iSlack[3];

    foreach(Line *sline, slack->lines) {
      iSlack[0] += sline->i(0);
      iSlack[1] += sline->i(1);
      iSlack[2] += sline->i(2);
    }

    complex<double> siSlack[3];
    siSlack[0] = sqrt(3)*slack->v(0)*iSlack[0];
    siSlack[1] = sqrt(3)*slack->v(1)*iSlack[1];
    siSlack[2] = sqrt(3)*slack->v(2)*iSlack[2];

    slack->setSi(0, siSlack[0]);
    slack->setSi(1, siSlack[2]);
    slack->setSi(2, siSlack[1]);
    // Compute max error.
    complex<double> error[3];
    maxError = -qInf();

    foreach(Bar *bar, network_->bars) {

      error[0] = bar->rV(0, Unit::kVoltsPerUnit)*conj(bar->rI(0,
                 Unit::kAmperePerUnit));

      error[0] -= bar->sh(0, Unit::kVaPerUnit);

      error[0] -= bar->si(0, Unit::kVaPerUnit);

      if(abs(error[0].real())>maxError)
        maxError = error[0].real();

      if(abs(error[0].imag()) >maxError)
        maxError = error[0].imag();
    }

    iteration++;
  } while(( maxError > network_->minError) &&
          (iteration < network_->maxIterations));

  duration = timer.elapsed();
  usedIterations = iteration;

  if (iteration < network_->maxIterations)
    return true;
  else
    return false;
}

