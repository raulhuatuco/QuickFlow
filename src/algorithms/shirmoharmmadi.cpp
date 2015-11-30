/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 David Krepsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \addtogroup Algorithms
 * \{
 */

/*!
 * \file shirmoharmmadi.h
 *
 * \brief Definition of Shirmoharmmadi algorithm.
 *
 * This is the definition of the Shirmoharmmadi class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 11/2015
 * \copyright David Krepsky
 */

#include "shirmoharmmadi.h"
#include <QList>
#include <QElapsedTimer>
#include <cmath>
#include "math_constants.h"
#include "models/bar.h"
#include "models/line.h"

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Shirmoharmmadi::Shirmoharmmadi(Network *network) :
  network_(network),
  radLayer(network)
{

}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Shirmoharmmadi::~Shirmoharmmadi()
{

}

/*******************************************************************************
 * Solve network.
 ******************************************************************************/
bool Shirmoharmmadi::solve()
{
  // Create a timer to measure the duration and start it.
  QElapsedTimer timer;
  timer.start();

  // Initialize data.
  flatStart();

  // Initialize at iteration 0 and error inf;
  uint32_t iteration = 0;
  double iterationError = qInf();

  // Create a radial layer to operate the algorithm.
  radLayer.calculate();

  do {
    // Calculates the backward sweep.
    doBackwardSweep();

    // Calculates the forward sweep.
    doForwardSweep();

    // Calculate Slack current.
    computeSlackCurrent();

    // Calculate the iteration error.
    iterationError = maxError();

    iteration++;
  } while(( iterationError > network_->minError) &&
          (iteration < network_->maxIterations));

  calcLineLoss();

  usedIterations = iteration;
  duration = timer.elapsed();

  if (iteration < network_->maxIterations)
    return true;
  else
    return false;
}

/*******************************************************************************
 * Flat start.
 ******************************************************************************/
void Shirmoharmmadi::flatStart()
{

  double initialVoltage = network_->voltageBase();

  // Initialize bars.
  foreach(Bar *bar, network_->bars) {
    // Set Initial voltage to 1pu.
    bar->setRV(0, std::polar(initialVoltage, 0.0), Unit::kVolts);
    bar->setRV(1, std::polar(initialVoltage, 120.0*kPI/180.0), Unit::kVolts);
    bar->setRV(2, std::polar(initialVoltage, 240.0*kPI/180.0), Unit::kVolts);

    // Set initial current to 0 amps.
    bar->setRI(0, 0.0);
    bar->setRI(1, 120.0);
    bar->setRI(2, -120.0);
  }

  foreach(Line *line, network_->lines) {
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

/*******************************************************************************
 * Bar current.
 ******************************************************************************/
void Shirmoharmmadi::calcBarCurrent(Bar *bar)
{
  complex<double> i[3];

  i[0] = -conj((bar->si(0) - bar->sh(0))/ bar->rV(0));
  i[1] = -conj((bar->si(1) - bar->sh(1))/ bar->rV(1));
  i[2] = -conj((bar->si(2) - bar->sh(2))/ bar->rV(2));

  bar->setRI(0, i[0]);
  bar->setRI(1, i[1]);
  bar->setRI(2, i[2]);
}

void Shirmoharmmadi::calcLineLoss()
{
  complex<double> loss[3];

  foreach(Line *line, network_->lines) {
    loss[0] = (line->pNoI()->rV(0) - line->pNoF()->rV(0))*conj(line->i(0));
    loss[1] = (line->pNoI()->rV(1) - line->pNoF()->rV(1))*conj(line->i(1));
    loss[2] = (line->pNoI()->rV(2) - line->pNoF()->rV(2))*conj(line->i(2));

    line->setLoss(0, loss[0]);
    line->setLoss(1, loss[1]);
    line->setLoss(2, loss[2]);
  }
}

/*******************************************************************************
 * Backward sweep.
 ******************************************************************************/
void Shirmoharmmadi::doBackwardSweep()
{
  // Creates a list to mark lines that has beed processed.
  QList<Line *> processedLines;

  //Backward sweep start here.
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
          calcBarCurrent(bar);
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
}

/*******************************************************************************
 * Forward sweep.
 ******************************************************************************/
void Shirmoharmmadi::doForwardSweep()
{
// Creates a list to mark lines that has beed processed.
  QList<Line *> processedLines;

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

          bVoltage[0] -= line->z(0, Unit::kOhmPerUnit)*line->i(0,Unit::kAmperePerUnit);
          bVoltage[0] -= line->z(1, Unit::kOhmPerUnit)*line->i(1, Unit::kAmperePerUnit);
          bVoltage[0] -= line->z(2,Unit::kOhmPerUnit)*line->i(2, Unit::kAmperePerUnit);

          bVoltage[1] -= line->z(1, Unit::kOhmPerUnit)*line->i(0,Unit::kAmperePerUnit);
          bVoltage[1] -= line->z(3, Unit::kOhmPerUnit)*line->i(1, Unit::kAmperePerUnit);
          bVoltage[1] -= line->z(4,Unit::kOhmPerUnit)*line->i(2, Unit::kAmperePerUnit);

          bVoltage[2] -= line->z(2, Unit::kOhmPerUnit)*line->i(0,Unit::kAmperePerUnit);
          bVoltage[2] -= line->z(4, Unit::kOhmPerUnit)*line->i(1, Unit::kAmperePerUnit);
          bVoltage[2] -= line->z(5,Unit::kOhmPerUnit)*line->i(2, Unit::kAmperePerUnit);

          line->pNoF()->setRV(0, bVoltage[0], Unit::kVoltsPerUnit);
          line->pNoF()->setRV(1, bVoltage[1], Unit::kVoltsPerUnit);
          line->pNoF()->setRV(2, bVoltage[2], Unit::kVoltsPerUnit);

          processedLines.append(line);
        }
      }
    }
  }
}

/*******************************************************************************
 * Compute slack current.
 ******************************************************************************/
void Shirmoharmmadi::computeSlackCurrent()
{
  Bar *slack = network_->getBarById(0);

  complex<double> iSlack[3];

  foreach(Line *sline, slack->lines) {
    iSlack[0] -= sline->i(0, Unit::kAmperePerUnit);
    iSlack[1] -= sline->i(1, Unit::kAmperePerUnit);
    iSlack[2] -= sline->i(2, Unit::kAmperePerUnit);
  }

  slack->setRI(0, iSlack[0]);
  slack->setRI(1, iSlack[1]);
  slack->setRI(2, iSlack[2]);

  complex<double> siSlack[3];
  siSlack[0] = slack->rV(0)*conj(iSlack[0]);
  siSlack[1] = slack->rV(1)*conj(iSlack[1]);
  siSlack[2] = slack->rV(2)*conj(iSlack[2]);

  slack->setSi(0, siSlack[0]);
  slack->setSi(1, siSlack[2]);
  slack->setSi(2, siSlack[1]);
}

/*******************************************************************************
 * Compute maximum error.
 ******************************************************************************/
double Shirmoharmmadi::maxError()
{
  // Holds the last maximum error.
  double maxError = 0.0;

  Bar *slack = network_->getBarById(0);

  complex<double> deltaS[3];

  deltaS[0] = slack->si(0, Unit::kVA) - oldSlackPower[0];
  deltaS[1] = slack->si(1, Unit::kVA) - oldSlackPower[1];
  deltaS[2] = slack->si(2, Unit::kVA) - oldSlackPower[2];

  for (int i=0; i<3; i++) {
    double realError;
    double imagError;

    realError = abs(deltaS[i].real());
    imagError = abs(deltaS[i].imag());

    if(realError > maxError) {
      maxError = realError;
    }

    if(imagError > maxError) {
      maxError = imagError;
    }
  }

  oldSlackPower[0] = slack->si(0, Unit::kVA);
  oldSlackPower[1] = slack->si(1, Unit::kVA);
  oldSlackPower[2] = slack->si(2, Unit::kVA);

  return maxError;
}

/*!
 * \}
 */
