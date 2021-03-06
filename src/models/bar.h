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
 * \file bar.h
 *
 * \brief Bar class definition.
 *
 * This file defines the Bar class.
 *
 * \author David Krepsky
 * \version 0.4
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef MODELS_BAR_H
#define MODELS_BAR_H

#include <QJsonObject>
#include <QGraphicsObject>
#include <QPointer>
#include <complex>
#include <QTextStream>
#include "unit.h"
#include "models/line.h"
#include "models/network.h"
#include "graphics/infobar.h"

QT_BEGIN_NAMESPACE
class Line;
class Network;
class InfoBar;
QT_END_NAMESPACE

using std::complex;

/*!
 * \class Bar
 * \brief Represents a busbar in power distribution systems.
 *
 * ### Overview
 *
 * Bar class represents a bar in a power system, providing the data structure
 * and methods that model a real bar.
 *
 * The class also provides the graphical structure to be displayed in a
 * SystemView widget.
 *
 * When selected, the bar will show an information window with all its resulting
 * values, in the current project units.
 *
 * A bar with id equals to 0 is an alimentator (Slack bar).
 *
 * Bar properties are:
 *  - Voltage (\e v): is the line voltage at the bar.
 *
 *  - Shunt elements (\e sh) power: shunt elements are represented by the power
 *    they inject in the bar.
 *
 *  - Injected power (\e si): the power injected in the bar (here, power that is
 *    consumed is represented as a positive value and power that is generated at
 *    the bar is represented with a negative sign).
 *
 *  - Resulting Current (\e rI): current at bar, this is used as a result value.
 *
 *  - Resulting Voltage (\e rV): the resulting voltage after calculations, used
 *    in InfoBar class.
 *
 * The bar properties are stored in the following units:
 *  - Power: VA per unit.
 *  - Voltage: Volts per unit.
 *
 * Also, bars are connected through lines.
 *
 * In order to use diferent units, use the option parameter \b unit with the
 * desired unit.
 *
 * Bars must be added to a network in order to be displayed by a SystemView.
 *
 * ### Example
 * \code
 * // Code to add a bar. //
 *
 * // External objects where the bar will be put in.
 * extern SystemView *systemView;
 * extern Network *network;
 *
 * QPointer<Bar> bar = new Bar;
 * // Bar id must be defined.
 * bar->id = 20;
 *
 * if(!network.addBar(bar)){
 *   return;
 * }
 *
 * systemView.addBar(bar);
 *
 * \endcode
 *
 * \note The values v, sh and si are the initial conditions. Results of the load
 * flow calculation have a "r" in the name, like rV and rI.
 *
 * \warning When bar is deleted, all lines connecting to it will also be deleted.
 *
 * \warning In order to prevent memory corruption, always use QPointer object as
 * the pointer when creating a new bar or when deleting one.
 */
class Bar : public QGraphicsObject
{
  Q_OBJECT

public:
  /*****************************************************************************
   * Public Constants.
   ****************************************************************************/
  /*!
   * \brief Indicates an invalid bar.
   *
   * This value is used to determine if a bar has been correctly initialized
   * and to check if bar data is valid;
   */
  static const int32_t kInvalidId;

  /*****************************************************************************
   * Public data.
   ****************************************************************************/
  /*!
   * \brief Lines connected to this bar.
   *
   * All lines that connect to this bar are listed here.
   *
   * \warning Don't use the lines vector to manipulate lines directly. Instead,
   * use the provided add and remove functions.
   *
   * \note In order to prevent memory corruption, a QPointer object is used as
   * the pointer for lines here, since when delete, the bar will try to remove
   * every line connected to it.
   */
  QVector<QPointer<Line>> lines;

  /*****************************************************************************
   * Public methods.
   ****************************************************************************/
  /*!
   * \brief Constructor.
   *
   * By default, bars are created filled with zeros.
   */
  Bar();

  /*!
   * \brief Destructor.
   *
   * When deleted, a bar will try to delete all the lines connected to it, also
   * it will try to remove itself from the network and the SystemView scene.
   */
  ~Bar();

  /*!
   * \brief The bar id.
   *
   *  Returns the bar id.
   *  - An id < 0 is an invalid bar.
   *  - An id = 0 is a slack bar.
   *  - Any other id represents a PQ bar (PV bars are not implemented yet).
   *
   * \note Only onde bar in the network can have id = 0.
   * \note Bar ids are unic. Two bars with the same id can't be put in the same
   * network.
   *
   * \return The bar id.
   */
  int32_t id();

  /*!
   * \brief Set the bar id.
   *
   * Set bar id. Check \b id() for more information.
   *
   * \param[in] id The new bar id.
   */
  void setBarId(int32_t id);

  /*!
   * \brief Bar current network.
   *
   * Returns the network object that holds the bar.
   *
   * \return Bar current network.
   */
  Network *network();

  /*!
   * \brief Set bar network.
   *
   * Set the bar current network.
   *
   * \param[in] network Network that holds the bar.
   */
  void setNetwork(Network *network);

  /*!
   * \brief Initial Voltage.
   *
   * Returns the initial voltage in the specified unit.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] unit Unit of the voltage (default is Volts).
   *
   * \return Voltage of phase \b phase.
   */
  complex<double> v(int8_t phase, Unit::VoltageUnit unit = Unit::kVoltsPerUnit);

  /*!
   * \brief Set initial voltage.
   *
   * Set the initial voltage of the selected phase.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] newVoltage New voltage.
   * \param[in] unit Unit of the new voltage.
   */
  void setV(int8_t phase, complex<double> newVoltage,
            Unit::VoltageUnit unit = Unit::kVoltsPerUnit);

  /*!
   * \brief Shutn element.
   *
   * Power injected by the shunt element. Generated power is represented with a
   * negative value, consumed power (load) must be positive.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] unit Unit of the returned value.
   *
   * \return The power of the shunt element.
   */
  complex<double> sh(int8_t phase, Unit::PowerUnit unit = Unit::kVaPerUnit);

  /*!
   * \brief Set Shunt power.
   *
   * Set the shunt element power.Generated power is represented with a negative
   * value, consumed power (load) must be positive.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] newPower Shunt power.
   * \param[in] unit Shunt power unit.
   */
  void setSh(int8_t phase, complex<double> newPower,
             Unit::PowerUnit unit = Unit::kVaPerUnit);

  /*!
   * \brief Injected power.
   *
   * Injected power at the bar. Generated power is represented with a negative
   * value, consumed power (load) must be positive.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] unit Power unit.
   *
   * \return Injected power.
   */
  complex<double> si(int8_t phase, Unit::PowerUnit unit = Unit::kVaPerUnit);

  /*!
   * \brief Set the injected power.
   *
   * Set the injected power at the bar. enerated power is represented with a
   * negative value, consumed power (load) must be positive.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] newPower New injected power.
   * \param[in] unit New injected power unit.
   */
  void setSi(int8_t phase, complex<double> newPower,
             Unit::PowerUnit unit = Unit::kVaPerUnit);

  /*!
   * \brief Resulting bar voltage.
   *
   * Returns the voltage of the bar after the calculations.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] unit Unit of the returned value.
   *
   * \return Resulting bar voltage.
   */
  complex<double> rV(int8_t phase, Unit::VoltageUnit unit = Unit::kVoltsPerUnit);

  /*!
   * \brief Set result voltage.
   *
   * Set the result voltage.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] resultVoltage New result voltage.
   * \param[in] unit Result unit.
   */
  void setRV(int8_t phase, complex<double> resultVoltage,
             Unit::VoltageUnit unit = Unit::kVoltsPerUnit);

  /*!
   * \brief Resulting bar current.
   *
   * Returns the total current injection at the bar.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] unit Unit.
   *
   * \return Result bar current.
   */
  complex<double> rI(int8_t phase,
                     Unit::CurrentUnit unit = Unit::kAmperePerUnit);

  /*!
   * \brief Set resulting current.
   *
   * Set the computed current.
   *
   * \param[in] phase Phase (0, 1 or 2).
   * \param[in] resultCurrent New result current.
   * \param[in] unit Result unit.
   */
  void setRI(int8_t phase, complex<double> resultCurrent,
             Unit::CurrentUnit unit = Unit::kAmperePerUnit);

  /*!
   * \brief Add line to bar.
   *
   * \param[in] line Line to be linked.
   */
  void addLine(QPointer<Line> line);

  /*!
   * \brief Remove line link;
   *
   * \param[in] line Line to be removed.
   */
  void removeLine(QPointer<Line> line);

  /*!
   * \brief Remove all lines from this bar.
   */
  void removeLines();

  /*!
   * \brief Convert bar data to json object.
   *
   * \return Return json object filled with bar data.
   */
  QJsonObject toJson();

  /*!
   * \brief Fill bar data from Json data.
   *
   * \param[in] jsonBar
   */
  void fromJson(QJsonObject &jsonBar);

  /*!
   * \brief Bar bounding rect.
   *
   * Implementation of the virtual boundingRect() function from
   * QGraphicsObject.
   *
   * \return The bar bounding rect.
   */
  QRectF boundingRect() const Q_DECL_OVERRIDE;

  void exportData(QTextStream &stream);
  
signals:
  /*****************************************************************************
  * Signals.
  *****************************************************************************/
  /*!
   * \brief Double click signal.
   *
   * Signal emited when a double click occurs.
   */
  void eventDoubleClick();

protected:
  /*****************************************************************************
  * Protected methods.
  *****************************************************************************/
  /*!
   * \brief Mouse double click event.
   *
   * Handles the double click event.
   * This will eit an eventDoubleClick() signal.
   *
   * \param[in] event Event data.
   */
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

  /*!
   * \brief Item change.
   *
   *  Used to handle item selection and geometry changes.
   *
   * \param[in] change Change that has been made.
   * \param[in] value Change data.
   *
   * \return Item change value;
   */
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) Q_DECL_OVERRIDE;

  /*!
   * \brief Bar paint function.
   *
   * Overrided function that will paint the bar in the scene, accordingly to the
   * bar id (slack or PQ).
   *
   * \param[out] painter Painter used to draw the bar.
   * \param[in] option Slyte options (not used).
   * \param[in] widget Widget that will be drawn (not used).
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  /*****************************************************************************
  * Private data.
  *****************************************************************************/
  /*!
   * \brief Holds the bar id.
   */
  int32_t id_;

  /*!
   * \brief A reference to the Network where the bar is.
   *
   * This option is used internaly to determine color settings and units.
   *
   * \warning This option must be not NULL in order to add the bar to a scene.
   */
  QPointer<Network> network_;

  /*!
  * \brief infoBar
  *
  * Pointer to a information box.
  * This is necessary so we can delete the information box when the item
  * selection changes to unselected.
  */
  QPointer<InfoBar> infoBar;

  /*!
   * \brief Bar voltage.
   *
   * Holds the initial voltage for phases 0, 1 and 2.
   */
  complex<double> v_[3];

  /*!
   * \brief Shunt element power.
   *
   * Holds the value of the shunt elements, in means of power, for phases 0, 1
   * and 2.
   */
  complex<double> sh_[3];

  /*!
   * \brief Injected power.
   *
   * Holds the power injected at the bar, for each phase.
   */
  complex<double> si_[3];

  /*!
   * \brief Result voltage.
   *
   * Stores each phase voltage of the bar after the power flow calculation.
   */
  complex<double> rV_[3];

  /*!
   * \brief Result current.
   *
   * Stores bar current after the power flow calculation.
   */
  complex<double> rI_[3];

  /*!
   * \brief Used to print the bar id in the view.
   */
  QGraphicsTextItem barLabel;

  /*****************************************************************************
  * Private methods.
  *****************************************************************************/
  /*!
   * \brief adjustLabelPosition
   */
  void adjustLabelPosition();

  /*!
   * \brief Show information box.
   *
   * This function will create a window with InfoBar class that will display
   * the resulting current and voltage.
   */
  void showInfo();

  /*!
   * \brief Hide information box.
   *
   * This will delete the InfoBar created with \b showInfo().
   */
  void hideInfo();

  /*!
   * \brief Draw Slack Bar.
   *
   * Draw a slack bar icon.
   * Color properties are provided by the network options.
   *
   * \param[in] painter Painter that will be used to draw the bar.
   */
  void drawSlack(QPainter *painter);

  /*!
   * \brief Draw Pq Bar.
   *
   * Draw a PQ bar. Color properties are provided by the network options.
   *
   * \param[in] painter Painter that will be used to draw the bar.
   */
  void drawPq(QPainter *painter);
};

#endif  // MODELS_BAR_H

/*!
 * \}
 */
