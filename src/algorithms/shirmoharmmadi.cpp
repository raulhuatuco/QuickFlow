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
  network_(network)
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

  // Initialize the results with the current bar voltage.
  foreach(Bar *bar, network_->bars) {
    bar->setRV(0, bar->v(0));
    bar->setRV(1, bar->v(1));
    bar->setRV(2, bar->v(2));
  }

  // Initialize at iteration 0 and error inf;
  uint32_t iteration = 0;
  double iterationError = qInf();

  // Create a radial layer to operate the algorithm.
  RadialLayer radLayer(network_);
  radLayer.calculate();

  do {
    // Calculates the backward sweep.
    doBackwardSweep(radLayer);

    // Calculates the forward sweep.
    doForwardSweep(radLayer);

    // Calculate Slack current.
    computeSlackCurrent();

    // Calculate the iteration error.
    iterationError = maxError();

    iteration++;
  } while(( iterationError > network_->minError) &&
          (iteration < network_->maxIterations));

  duration = timer.elapsed();
  usedIterations = iteration;

  if (iteration < network_->maxIterations)
    return true;
  else
    return false;
}

/*******************************************************************************
 * Backward sweep.
 ******************************************************************************/
void Shirmoharmmadi::doBackwardSweep(RadialLayer &radLayer)
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
void Shirmoharmmadi::doForwardSweep(RadialLayer &radLayer)
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
}

/*******************************************************************************
 * Compute slack current.
 ******************************************************************************/
void Shirmoharmmadi::computeSlackCurrent()
{
  Bar *slack = network_->getBarById(0);

  complex<double> iSlack[3];

  foreach(Line *sline, slack->lines) {
    iSlack[0] += sline->i(0);
    iSlack[1] += sline->i(1);
    iSlack[2] += sline->i(2);
  }

  complex<double> siSlack[3];
  siSlack[0] = slack->v(0)*iSlack[0];
  siSlack[1] = slack->v(1)*iSlack[1];
  siSlack[2] = slack->v(2)*iSlack[2];

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

  // Get the highest error in the network.
  foreach(Bar *bar, network_->bars) {
    double currentError = absError(bar);

    if(currentError > maxError) {
      maxError = currentError;
    }
  }

  return maxError;
}

/*******************************************************************************
 * Compute absolute bar error.
 ******************************************************************************/
double Shirmoharmmadi::absError(Bar *bar)
{
  // Compute max error.
  complex<double> sFinal, sInitial, v, i, error;

  /* This line should be removed, it is here because there is a problem with
   * the slack bar current. */
  if(bar->id == 0)
    return 0;

  sInitial = bar->si(0, Unit::kVaPerUnit);

  v = bar->rV(0, Unit::kVoltsPerUnit);
  i = bar->rI(0, Unit::kAmperePerUnit);
  sFinal = v*conj(i)*kSQRT3;


  error = sFinal - sInitial;

  double errorReal = fabs(error.real());
  double errorImag = fabs(error.imag());

  return (errorReal > errorImag) ? errorReal : errorImag;
}

/*!
 * \}
 */
