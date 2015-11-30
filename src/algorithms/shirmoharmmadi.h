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
 * \version 0.2
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef ALGORITHMS_SHIRMOHARMMADI_H
#define ALGORITHMS_SHIRMOHARMMADI_H


#include <complex>
#include "radiallayer.h"
#include "models/bar.h"
#include "models/line.h"
#include "models/network.h"

/*!
 * \class Shirmoharmmadi
 * \brief Solves a network with the Shirmoharmmadi algorithm.
 *
 * ### Overview
 * This class implements the Shirmoharmmadi algorithm to solve the power flow
 * problem as described in : "A three-phase power flow method   for real-time
 * distribution system analysis". Shirmoharmmadi, Dariush. IEEE Transactions on
 * Power Systems, Vol. 10, No. 2, May 1995.
 * The article is included in the Docummentation folder.
 *
 * Workings:
 *  - First, it is created a layered structure of the network using RadialLayer
 *    class. This class will help us travel trough the bars in the network.
 *    This is refered to as the renumbering process at the article. Here, we use
 *    an Object Oriented approach, creating a navigable structre.
 *
 *  - Second, we do a backward sweep calculating the line currents, going from
 *    the latest layer until we reach the slack bar.
 *
 *  - Tird, we do a forward sweep, calculating every bar voltage.
 *
 *  - Fourth, we measure the error and check for convergence. If the error
 *    criteria isn't meet, we do another iteration (until max. iterations).
 *
 * All the results are altomatically stored inside the bars and lines. Some data
 * aboult time, total generation, used iterations and system loss are stored as
 * public data in this class.
 *
 * Currently, networks must be radial, that is, all bars must connect to one and
 * only one bar from the previously layer (with exception of slack). Also, only
 * VTheta and PQ bars are allowed, support for PV bars is in the TODO list.
 *
 * This implementation is different from what is common in literature because it
 * uses a object oriented approach.
 *
 * There are public fields to gatter calculation time, used iterations, total
 * loss in the system and also the slack total generated power.
 *
 * \warning Only Slack and PQ bars are allowed.
 * \warning Network must be completelly radial (No meshes allowed).
 *
 * \todo Add support for PV bars.
 * \todo Add support for non-radial networks.
 * \todo Compute total loss and total generated power.
 */
class Shirmoharmmadi
{
public:
  /*****************************************************************************
   * Public data.
   ****************************************************************************/
  /*!
   * \brief Total time used to calculate the solution, in milliseconds.
   *
   * This will count the time from the begining of the solve() fucntion till the
   * end of it.
   */
  double duration;

  /*!
   * \brief Number of used iterations to obtain the solution.
   *
   * In case the network don't converge, this value will be set to -1.
   */
  int32_t usedIterations;

  /*!
   * \brief Total power generated at slark (Real power only).
   *
   * This is the total real power generated at the slack bar.
   */
  double slackGeneration;

  /*!
   * \brief Total loss of the system.
   *
   * Sum all the losses in the system, including both the reactive and active
   * losses.
   */
  complex<double> totalLoss;

  /*****************************************************************************
   * Public methods.
   ****************************************************************************/
  /*!
   * \brief Shirmoharmmadi class constructor.
   *
   *  Class constructor.
   *
   * \param[in,out] network Network object to be solved (must be valid).
   */
  Shirmoharmmadi(Network *network);

  /*!
   * \brief Destructor.
   *
   * Class destructor.
   */
  ~Shirmoharmmadi();

  /*!
   * \brief Solve the power flow for the especified network.
   *
   * \return True if the solution converges, false otherwise.
   */
  bool solve();

private:
  /*****************************************************************************
   * Private data.
   ****************************************************************************/
  /*!
   * \brief Internal pointer for the Network object that will be solved.
   */
  Network *network_;

  /*!
   * \brief Holds the power at slack bar from the previous iteration.
   */
  complex<double> oldSlackPower[3];

  /*!
   * \brief RadialLayer object to create a layered structure.
   *
   * This is used to navigate through the net.
   */
  RadialLayer radLayer;

  /*****************************************************************************
   * Private methods.
   ****************************************************************************/
  /*!
    * \brief flatStart
    *
    * \todo Insert comment for flatStart in shirmuhammadi.
    */
  void flatStart();

  /*!
   * \brief Calculate bar current.
   *
   * Used to calculate a bar's current by the following equation:
   *
   * \code
   * I = - conj((Sinjected - Sshunt)/Voltage).
   * \endcode
   *
   * Here, the current is negated (minus signal before the conj() function)
   * because we adopted that power beeing consumed at the bar does have a
   * positive sign, so we need to negate bar current to make current going into
   * the bar have a negative sign. This was make to complain with the article
   * notation.
   *
   * \param[in] bar Bar which the current wil be calculated.
   * \param[in] phase Current phase.
   *
   * \return The bar current.
   */
  void calcBarCurrent(Bar *bar);

  /*!
   * \brief calcLineLoss
   *
   * \todo Calc line loss.
   */
  void calcLineLoss();

  /*!
   * \brief Execute the backward sweep.
   *
   * The backward sweep will start at the farest layer and will move in
   * direction of slack bar, calculating every line current.
   *
   * The line \emph i current, refered to as Jl, is calculated by subtracting
   * the end node (bar) current, then we add all lines current that emanate from
   * the end node to the next layer.
   * The equation that describes the line current calculation is as follows:
   *
   * \code
   * Jl = - Ifinal_bar + sum(Jim)
   * \endcode
   *
   * Where:
   *  - Jl is the line current being calculated;
   *  - Ifinal_bar is the line end node.
   *  - Jim is the current of lines linking bar i (the final bar) to te bars m,
   *    where m is composed of all bars that are one layer further.
   */
  void doBackwardSweep();

  /*!
   * \brief Execute the forward sweep.
   *
   * The forward sweep will start at the slack bar and will moving in
   * direction to the farest layer, calculating every bar voltage.
   *
   * The new voltage is calculated using the following equation:
   *
   * \code
   * Vf = Vi - Jif*Z;
   * \endcode
   *
   * Where:
   *  - Vf is the end node of the line voltage.
   *  - Vi is the initial node of the line voltage.
   *  - Jif is the line current connecting bars i and f.
   *  - Z is the impedance matrix of line Lij (line connecting nodes i and f).
   */
  void doForwardSweep();

  /*!
   * \brief Compute slack bar current.
   *
   * Will sum the current of every line connected to the slack bar, plus, will
   * add the current used at the slack bar (by means of the injected power and
   * shunt power).
   *
   * \todo Add slack comsumed power.
   */
  void computeSlackCurrent();

  /*!
   * \brief Compute iteration maximum error.
   *
   * Will get the greatest error produced by the iteration.
   *
   * This will compare how the power at the slack bar is changing.
   * The error value is calculated as follows:
   *
   * First, we caculate the new slack bar total power, summing up every line
   * current and multipling by the slack voltage.
   *
   * Second, we get the difference of this value with the slack power of the
   * previous iteration.
   *
   * Trid, we compute the absolute value of power for the three phases (Both
   * reactive and active powers).
   *
   * Fourth, we return the greatest value (which can be real or active).
   *
   * \return Returns the max. error found.
   */
  double maxError();
};

#endif // ALGORITHMS_SHIRMOHARMMADI_H

/*!
 * \}
 */
