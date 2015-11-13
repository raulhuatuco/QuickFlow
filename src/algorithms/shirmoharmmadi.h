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

#ifndef ALGORITHMS_SHIRMOHARMMADI_H
#define ALGORITHMS_SHIRMOHARMMADI_H


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
 * First, it is created a layered structure of the network using RadialLayer
 * class. This class will help us travel trough the bars in the network.
 * Second, we do a backward sweep calculating the line currents, going from the
 * latest layer until we reach the slack bar.
 * Tird, we do a forward sweep, calculating every bar voltage.
 * Fourth, we measure the error and check for convergence. If the error criteria
 * isn't meet, we do another iteration (until max. iterations).
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
 * \warning Network must be completelly radial (No loops allowed).
 *
 * \todo Add support for PV bars.
 * \todo Add support for non-radial networks.
 */
class Shirmoharmmadi
{
public:
  /*****************************************************************************
   * Public data.
   ****************************************************************************/
  /*!
   * \brief Total time used to calculate the solution, in milliseconds.
   */
  double duration;

  /*!
   * \brief Number of used iterations to obtain the solution.
   */
  int32_t usedIterations;

  /*!
   * \brief Total power generated at slark (Real power only).
   */
  double slackGeneration;

  /*!
   * \brief Total loss of the system.
   */
  double totalLoss;

  /*****************************************************************************
   * Constructor.
   ****************************************************************************/
  /*!
   * \brief Shirmoharmmadi class constructor.
   *
   * Results are altomatically stored inside the elements.
   *
   * \param[in,out] network Network object to be solved (must be valid).
   */
  Shirmoharmmadi(Network *network);

  /*****************************************************************************
   * Destructor.
   ****************************************************************************/
  /*!
   * \brief Destructor.
   */
  ~Shirmoharmmadi();

  /*****************************************************************************
   * Solve network.
   ****************************************************************************/
  /*!
   * \brief Solve the power flow for the especified network.
   *
   * \return True if the solution converges, false otherwise.
   */
  bool solve();

private:
  /*****************************************************************************
   * Constants.
   ****************************************************************************/
  /*!
   * \brief Internal pointer for the Network object that will be solved.
   */
  Network *network_;

  /*****************************************************************************
   * Backward sweep.
   ****************************************************************************/
  /*!
   * \brief Execute the backward sweep.
   *
   * The backward sweep will start at the farest layer and will moving in
   * direction to the slack bar, calculating every line current.
   */
  void doBackwardSweep(RadialLayer &radLayer);
  
  /*****************************************************************************
   * Forward sweep.
   ****************************************************************************/
  /*!
   * \brief Execute the forward sweep.
   *
   * The forward sweep will start at the slack bar and will moving in
   * direction to the farest layer, calculating every bar voltage.
   */
  void doForwardSweep(RadialLayer &radLayer);
  
  /*****************************************************************************
   * Compute slack current.
   ****************************************************************************/
  /*!
   * \brief Compute slack bar current.
   * 
   * Will sum the current of every line connected to the slack bar.
   */
  void computeSlackCurrent();
  
  /*****************************************************************************
   * Compute maximum error.
   ****************************************************************************/
  /*!
   * \brief Compute iteration max. error.
   * 
   * Will get the greatest error produced by the iteration.
   *
   * \return Returns the max. error found.
   */
  double maxError();
  
  /*****************************************************************************
   * Compute absolute bar error.
   ****************************************************************************/
  /*!
   * \brief Compute the error for a bar.
   * 
   * Will return the error in bar \b bar.
   *
   * \return Returns bar error.
   */
  double absError(Bar *bar);
};

#endif // ALGORITHMS_SHIRMOHARMMADI_H

/*!
 * \}
 */
