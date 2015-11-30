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
 * \addtogroup Models
 * \{
 */

/*!
 * \file network.h
 *
 * \brief Network class definition.
 *
 * This file defines the Network class.
 *
 * \author David Krepsky
 * \version 0.4
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef MODELS_NETWORK_H
#define MODELS_NETWORK_H

#include <QGraphicsScene>
#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QMap>
#include <QPointer>
#include <cstdint>
#include "unit.h"
#include "models/bar.h"
#include "models/line.h"

QT_BEGIN_NAMESPACE
class Bar;
class Line;
QT_END_NAMESPACE

/*!
 * \class Network
 * \brief Represents a power distribution system.
 *
 * ### Overview
 * Network class represents a grid in power distribution systems, providing
 * the data structure and methods that models a real system.
 *
 * Each network consists of at least one slack bar, multiple PQ bars and lines.
 *
 * This class also holds some commom network properties, like the base values to
 * convert units to p.u. and the colors used to represent the net.
 *
 * As static data, there are the simulation parametes (which are the same for
 * every net) and the units used.
 *
 * An offset is also provided. This will give an offset to all network elements
 * when drawn at the systemView.
 *
 * Note that, for the base values (Vbase, Sbase, Ibase and Zbase) we don't use
 * the tri-phase set of equations. But, this wont affect the results, since
 * they are converted back to the regular units. It was made this way because
 * the only software we have to compare results do it in this way, but this
 * might change in the future.
 *
 * \warning Don't use the \b bars or \b lines maps to edit the network. They are
 * only provided to make cleaner code when using foreach loops. Use add* and
 * remove* function.
 *
 * \note When deleting a Network object, all bars and all lines inside this
 * Network will also be deleted.
 */
class Network : public QObject
{
  Q_OBJECT

public:
  /*****************************************************************************
   * Constant data.
   ****************************************************************************/
  /*!
   * \brief Size of bar icon.
   *
   * This property determines the size of all bars icons when drawn at
   * SystemView.
   */
  static const double barIconSize;

  /*!
   * \brief Line width.
   *
   * Specifies the width of lines when drawn at SystemView.
   */
  static const double lineWidth;

  /*!
   * \brief Selected item color.
   *
   * All selected items (lines and bars) have the same color.
   */
  static const QColor selectedColor;

  /*****************************************************************************
   * Static data.
   ****************************************************************************/
  /*!
   * \brief Maximum iterations.
   *
   * This is the maximum number of iterations an algorithm can go through.
   * If \b maxIterations is reached, we consider that the network didn't
   * converge.
   */
  static uint32_t maxIterations;

  /*!
   * \brief Minimum computation error.
   *
   * This is the error used by most algorithms stop criterium.
   */
  static double minError;

  /*!
   * \brief Length unit.
   *
   * Length unit used to display and edit data.
   */
  static Unit::LengthUnit lengthUnit;

  /*!
   * \brief Impedance unit.
   *
   * Impedance unit used to display and edit data.
   */
  static Unit::ImpedanceUnit impedanceUnit;

  /*!
   * \brief Voltage unit.
   *
   * Voltage unit used to display and edit data.
   */
  static Unit::VoltageUnit voltageUnit;


  /*!
   * \brief Power unit.
   *
   * Power unit used to display and edit data.
   */
  static Unit::PowerUnit powerUnit;

  /*!
   * \brief Current unit.
   *
   * Current unit used to display and edit data.
   */
  static Unit::CurrentUnit currentUnit;

  /*****************************************************************************
   * Public data.
   ****************************************************************************/
  /*!
   * \brief Network name.
   *
   * Every network is referenced by its name.
   */
  QString name;

  /*!
   * \brief X Offset.
   *
   * This offset will move all elements it the network by \b xOffset positions
   * in the x axis.
   * Usefull to avoid one network overlapping another.
   */
  double xOffset;

  /*!
   * \brief Y Offset.
   *
   * This offset will move all elements it the network by \b yOffset positions
   * in the y axis.
   * Usefull to avoid one network overlapping another.
   */
  double yOffset;

  /*!
   * \brief Bar's contour color.
   *
   * Every bar does hava a border. This is the border color value.
   */
  QColor barContourColor;

  /*!
   * \brief Slack bar color.
   *
   * Color for the slack bar.
   */
  QColor slackBarColor;

  /*!
   * \brief PQ bar Color.
   *
   * Color for PQ bars.
   */
  QColor pqBarColor;

  /*!
   * \brief Line color.
   *
   * Color for lines.
   */
  QColor lineColor;

  /*!
   * \brief Bars.
   *
   * Map with every bar in the network.
   *
   * The key is the bar ID and the value is a QPointer to the bar.
   */
  QMap<int32_t, QPointer<Bar> > bars;

  /*!
   * \brief Lines.
   *
   * Map with every line in the network.
   *
   * The key is a QPair with the initial and final nodes and the value is a
   * QPointer to the line.
   */
  QMap<QPair<int32_t, int32_t>, QPointer<Line> > lines;

  /*****************************************************************************
   * Public methods.
   ****************************************************************************/
  /*!
   * \brief Constructor.
   *
   * Class constructor.
   */
  Network();

  /*!
   * \brief Destructor.
   *
   * Class destructor.
   *
   * \note When deleted, all bars and all lines will also be deleted.
   */
  ~Network();

  /*!
   * \brief Add bar.
   *
   * Inser a bar into this network. Bars are referenced by their ID, so if the
   * bar ID to be inserted is already present in the network, this function will
   * fail. Also, passing a NULL pointer will make the function fail.
   *
   * \param[in] bar Bar to be included in the network.
   *
   * \return True if bar was added. False if fails.
   */
  bool addBar(Bar *bar);

  /*!
   * \brief Add line.
   *
   * Insert a line into this network. Line are referenced by their initial and
   * final nodes.
   *
   * In order to add a line, the line parents (initial and final bars) must
   * already be inside the network.
   *
   * If no bars with the line parents ids is found, this method fails and return
   * false.
   *
   * If there is already a line with the same parents, the method fails too,
   * because only one line can link two bars together.
   *
   * Also, passing a NULL pointer will make the method fail.
   *
   * \param[in] line Line to be inserted.
   *
   * \return True if the line was added. False on fail.
   */
  bool addLine(Line *line);

  /*!
   * \brief Remove bar.
   *
   * Removes the bar with id equals to \b id from the grid.
   *
   * \param[in] id Id of the bar to be removed.
   */
  void removeBar(int32_t id);

  /*!
   * \brief Remove bar by pointer.
   *
   * Remove the bar by it's pointer.
   *
   * \param[in] bar Pointer to the bar to be removed.
   */
  void removeBar(Bar *bar);

  /*!
   * \brief Remove line.
   *
   * Remove line by it's parent IDs.
   *
   * \param[in] nodes Initial and final nodes ID of the line to be removed.
   */
  void removeLine(QPair<int32_t, int32_t> nodes);

  /*!
   * \brief Remove line by pointer.
   *
   * Removes a line based on it's pointer.
   *
   * \param[in] line Pointer of the line to be removed.
   */
  void removeLine(Line *line);

  /*!
   * \brief Get bar pointer by id.
   *
   * Search for the bar with id equals to \b id.
   * If the bar is not present, a NULL pointer is returned.
   *
   * \param[in] id Bar id to search for.
   *
   * \return The bar pointer if found. NULL if no bar with this id is present.
   */
  Bar *getBarById(uint32_t id);

  /*!
   * \brief Get line by nodes.
   *
   * Search for a line by it parent's id.
   *
   * \param[in] nodes If no line is found, returns NULL.
   *
   * \return Pointer to the line found. Return NULL if line is not found.
   */
  Line *getLineByNodes(QPair<int32_t, int32_t> nodes);

  /*!
   * \brief Store network data into Json object.
   *
   * Stores the Network data into a QJsonObject.
   * Used to save lines, bars and options into the project file.
   * To load a network from a Json object, use fromJson().
   *
   * \note Static data members are not saved inside the Json object.
   *
   * \return Json object filled with network data.
   */
  QJsonObject toJson();

  /*!
   * \brief Load from Json object.
   *
   * Load network data from a QJsonObject.
   *
   * \note Static data isn't loaded from the Json object.
   *
   * \param[in] netJson Json object with network data.
   */
  void fromJson(QJsonObject &netJson);

  /*!
   * \brief Current base (Ibase).
   *
   * Gets the current base value used in p.u unit conversion.
   *
   * The current base is given by the following equation:
   * \code
   * Ibase = Sbase/Vbase [Amperes per unit].
   * \endcode
   *
   * \return The current base value.
   */
  double currentBase();

  /*!
   * \brief Impedance base (Zbase).
   *
   * Returns the impedance base, used to convert impedance values to p.u.
   *
   * The impedance base is given by the following equation:
   * \code
   * Zbase = VBase^2/Sbase [Ohms per unit].
   * \endcode
   * \return The impedance base.
   */
  double impedanceBase();

  /*!
   * \brief Voltage base (Vbase).
   *
   * Voltage base used to transform voltage values to per unit.
   * This value must be set with setVoltageBase.
   *
   * \return The voltage base.
   */
  double voltageBase();

  /*!
   * \brief Power base (Sbase).
   *
   * Power base used to convert power values to per unit.
   * This property must be set with setPowerBase.
   *
   * \return The power base.
   */
  double powerBase();

  /*!
   * \brief Set voltage base (Vbase).
   *
   * Set the voltage base value, used in per unit conversions.
   *
   * \param[in] voltage The new voltage base.
   */
  void setVoltageBase(double voltage);

  /*!
   * \brief Set power base (Sbase).
   *
   * Set the power base value, used in per unit conversion.
   *
   * \param[in] power The new power base value.
   */
  void setPowerBase(double power);

private:
  /*****************************************************************************
   * Private data.
   ****************************************************************************/
  /*!
   * \brief Holds the voltage base value.
   */
  double voltageBase_;

  /*!
   * \brief Holds the power base value.
   */
  double powerBase_;
};

#endif  // MODELS_NETWORK_H

/*!
 * \}
 */
