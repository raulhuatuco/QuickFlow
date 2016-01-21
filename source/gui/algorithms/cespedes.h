#ifndef CESPEDES_H
#define CESPEDES_H

#include <QMap>
#include <complex>
#include "radiallayer.h"
#include "models/bar.h"
#include "models/line.h"
#include "models/network.h"

class Cespedes
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
  complex<double> totalLoss;

  Cespedes(Network *network);

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

  QMap<Bar *, complex<double> > barPower[3];
  QMap<Line *, complex<double> > lineLoss[3];

  /*****************************************************************************
   * Private methods.
   ****************************************************************************/
  /*!
    * \brief flatStart
    *
    * \todo add comment to flatstart cespedes.
    */
  void flatStart();

  /*!
   * \brief Execute the backward sweep.
   *
   * The backward sweep will start at the farest layer and will moving in
   * direction to the slack bar, calculating every line current.
   */
  void doBackwardSweep(RadialLayer &radLayer);

  /*!
   * \brief Execute the forward sweep.
   *
   * The forward sweep will start at the slack bar and will moving in
   * direction to the farest layer, calculating every bar voltage.
   */
  void doForwardSweep(RadialLayer &radLayer);

  /*!
   * \brief Compute slack bar current.
   *
   * Will sum the current of every line connected to the slack bar.
   */
  void computeLosses();

  /*!
   * \brief Compute iteration max. error.
   *
   * Will get the greatest error produced by the iteration.
   *
   * \return Returns the max. error found.
   */
  double maxError();

  /*!
   * \brief solveBiquadratic
   * \param b
   * \param c
   * \return
   */
  double solveBiquadratic(double b, double c);

  /*!
   * \brief calcLineCurrent
   */
  void calcLineCurrent();

  /*!
   * \brief calcBarCurrent
   */
  void calcBarCurrent();

  /*!
   * \brief saveLineLoss
   */
  void saveLineLoss();

  void calcVoltageAngle(RadialLayer &radLayer);
};

#endif // CESPEDES_H
