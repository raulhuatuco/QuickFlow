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
 * \file line.h
 *
 * \brief Line class definition.
 *
 * This file defines the Line class.
 *
 * \author David Krepsky
 * \version 0.3
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef MODELS_LINE_H
#define MODELS_LINE_H

#include <QJsonObject>
#include <QGraphicsObject>
#include <QPointer>
#include <complex>
#include "models/bar.h"
#include "models/network.h"
#include "graphics/infoline.h"

QT_BEGIN_NAMESPACE
class Bar;
class InfoLine;
class Network;
QT_END_NAMESPACE

using std::complex;

/*!
 * \class Line
 *
 * \brief Represents a line in power distribution systems.
 *
 * ### Overview
 *
 * Line class represents a line in a power system, providing the data structure
 * and methods that model a real line.
 *
 * The class also provides the graphical structure to be displayed in a
 * SystemView widget.
 *
 * When selected, the line will show an information window with all its
 * resulting values, in the current project units.
 *
 * Lines connects bars, from an initial node to a final node. The model used
 * is the 3 wire PI model, where a line is represented by an 3x3 impedance
 * matrix.
 * Since the impedance matrix is simetric, we store only 6 values (Zaa, Zab, Zac
 * Zbb, Zbc, Zcc) instead of 9 (3x3) values.
 *
 * Line properties are:
 *  - Impedance (\e z): The impedance for each phase, as well as the inter-phase
 *    impedance.
 *
 *  - Current (\e i): This is the current obtained from the calculations.
 *
 *  - Loss (\e loss): The power that is lost due to joule effect.
 *
 *  - Length (\e length): Line length. Used to convert impedance input that is
 *    in per length units.
 *
 * All line properties are stored in per unit. Except the length, which is stored
 * in meters.
 *
 * The parameter \b unit is used to get the properties converted in any unit of
 * the propertie type.
 *
 * To be displayed in a systemView, lines must have a network and must be
 * connected to an initial bar and a final bar.
 *
 * Since line selection only with boundingRect() is really bad, the selection
 * area is reimplemented with \b shape.
 *
 * ### Example
 * \code
 * // Code to add a line. //
 *
 * // External objects where the line will be put in.
 * extern SystemView *systemView;
 * extern Network *network;
 *
 * QPointer<Line> line = new Line;
 * // Line initial and final nodes must be defined before we add it to a network.
 * // The nodes this line is connecting are the slack and bar number 1.
 * line->nodes = QPair<int32_t, int32_t> (0, 1);
 *
 * if(!network.addLine(line)){
 *   return;
 * }
 *
 * systemView.addLine(line);
 *
 * \endcode
 *
 * \note Nodes and bars are interchangeable words in this documentation.
 *
 * \note Length does not determine the line length when drawn. The nodes
 * position does.
 *
 * \warning In order to prevent memory corruption, always use QPointer object as
 * the pointer when creating a new line or when deleting one.
 */
class Line : public QGraphicsObject
{
  Q_OBJECT

public:
  /*****************************************************************************
   * Constants.
   ****************************************************************************/
  /*!
   * \brief Indicates an invalid bar.
   *
   * This value is used to indicate that an invalid bar is used as a node.
   */
  static const int32_t kInvalidNode;

  /*****************************************************************************
   * Public properties.
   ****************************************************************************/
  /*!
   * \brief Initial and final nodes.
   *
   * The initial and final nodes for this line. The values are bar ids.
   */
  QPair<int32_t, int32_t> nodes;

  /*****************************************************************************
   * Public methods.
   ****************************************************************************/
  /*!
   * \brief Constructor.
   *
   * Default line constructor.
   */
  Line();

  /*!
   * \brief Destructor.
   *
   * When deleted, lines will remove itself from the scene, the network and from
   * the initial and final nodes.
   */
  ~Line();

  /*!
   * \brief Network.
   *
   * Network that contains the line.
   *
   * \return The network that contains the line.
   */
  Network *network();

  /*!
   * \brief Set network.
   *
   * Set the network that contains the line.
   *
   * \param[in] network New network.
   */
  void setNetwork(Network *network);

  /*!
   * \brief Line impedance.
   *
   * This function returns the line impedance for the 3x3 matrix of the 3 wire,
   * PI model.
   * It's represented as:
   *
   * |Zaa Zab Zac|
   * |Zba Zbb Zbc|
   * |Zca Zcb Zcc|
   *
   * Since it is a simetric matrix, only Zaa, Zab, Zac, Zbb. Zbc and Zcc are
   * stored.
   *
   * To select the desired Zxx value, use the folowing map table for index.
   *  0 -> Zaa
   *  1 -> Zab, Zba
   *  2 -> Zac, Zca
   *  3 -> Zbb
   *  4 -> Zbc, Zcb
   *  5 -> Zcc
   *
   * In order to facilitate the use, we also provide an overloaded version where
   * the access is made by means of the line and colum in the impedance matrix.
   * Please check \b z(int8_t line, int8_t, colum, Unit::ImpedanceUnit unit =
   * Unit::kOhmPerUnit) for more info.
   *
   * \param[in] index Maped index of the required impedance.
   * \param[in] unit Impedance output unit.
   *
   * \return The line impedance.
   */
  complex<double>  z(int8_t index, Unit::ImpedanceUnit unit = Unit::kOhmPerUnit);

  /*!
   * \brief Line impedance.
   *
   * Returns the line impedance according to \b line and \b colum from the
   * impedance matrix.
   *
   * Please, refer to \b z(int8_t index, Unit::ImpedanceUnit unit =
   * Unit::kOhmPerUnit) for more info about impedance matrix.
   *
   * \param[in] line Lines represents the phases (0, 1 and 2).
   * \param[in] colum Colum represents the "in relation to" of the impedance.
   * \param unit Impedance output unit.
   *
   * \return The line impedance.
   */
  complex<double>  z(int8_t line, int8_t colum,
                     Unit::ImpedanceUnit unit = Unit::kOhmPerUnit);

  /*!
   * \brief Set line impedance.
   *
   * Set line impedance according to \b z(int8_t index, Unit::ImpedanceUnit
   * unit = Unit::kOhmPerUnit) scheme.
   *
   * \param[in] index Impedance index to be modified.
   * \param[in] newImpedance New impedance value.
   * \param[in] unit newImpedance unit.
   */
  void setZ(int8_t index, complex<double> newImpedance,
            Unit::ImpedanceUnit unit = Unit::kOhmPerUnit);

  /*!
   * \brief Set line impedance.
   *
   * Overloaded version of setZ(int8_t index, complex<double> newImpedance,
   * Unit::ImpedanceUnit unit = Unit::kOhmPerUnit) to work with line and colum
   * from the impedance matrix.
   *
   * \param[in] line Value line in the impedance matrix.
   * \param[in] colum Value colum in the impedance matrix.
   * \param[in] newImpedance New impedance value.
   * \param[in] unit newImpedance unit.
   */
  void setZ(int8_t line, int8_t colum, complex<double> newImpedance,
            Unit::ImpedanceUnit unit = Unit::kOhmPerUnit);

  /*!
   * \brief Line current.
   *
   * Returns line current for phase \b phase.
   *
   * \param[in] phase Line phase.
   * \param[in] unit Current output unit.
   *
   * \return Line current.
   */
  complex<double> i(int8_t phase, Unit::CurrentUnit unit = Unit::kAmperePerUnit);

  /*!
   * \brief Set line current.
   *
   * Sets the line current for phase \b phase.
   *
   * \param[in] phase Line phase.
   * \param[in] newCurrent New current value.
   * \param[in] unit newCurrent unit.
   */
  void setI(int8_t phase, complex<double> newCurrent,
            Unit::CurrentUnit unit = Unit::kAmperePerUnit);

  /*!
   * \brief Line loss.
   *
   * Returns all losses (active and reactive) in the line.
   *
   * \param[in] phase line phase.
   * \param[in] unit Loss output unit.
   *
   * \return Line loss.
   */
  complex<double> loss(int8_t phase, Unit::PowerUnit unit = Unit::kVaPerUnit);

  /*!
   * \brief Set power loss in the line.
   *
   * Sets the loss value at phase \b phase for the line.
   *
   * \param[in] phase Line phase.
   * \param[in] newLoss New loss value.
   * \param[in] unit newLoss unit.
   */
  void setLoss(int8_t phase, complex<double> newLoss,
               Unit::PowerUnit unit = Unit::kVaPerUnit);

  /*!
   * \brief Line length.
   *
   * Returns the line length.
   *
   * \param[in] unit Length output unit.
   *
   * \return Line length.
   */
  double length(Unit::LengthUnit unit = Unit::kMeter);

  /*!
   * \brief Set line length.
   *
   * Sets line length.
   * The length property is used to convert impedance units. It won't chage
   * the length of the drawing.
   *
   * Since all phases have the same length, only onde value is stored.
   *
   * \param[in] newLength New line length.
   * \param[in] unit newLength unit.
   */
  void setLength(double newLength, Unit::LengthUnit unit = Unit::kMeter);

  /*!
   * \brief Pointer to initial node (bar).
   *
   * Returns a pointer to the initial node.
   *
   * \return Pointer to the initial bar.
   *
   * \warning This pointer must be checked for NULL before use.
   */
  Bar *pNoI();

  /*!
  * \brief Pointer to final node (bar).
  *
  * Returns a pointer to the final node.
  *
  * \return Pointer to the final bar.
  *
  * \warning This pointer must be checked for NULL before use.
  */
  Bar *pNoF();

  /*!
   * \brief Set nodes pointers.
   *
   * Set the initial and final bar.
   * This function must be called before the line is inserted in a view, beacuse
   * it will use the nodes position to draw itself.
   *
   * \param[in] pNoI Initial node pointer.
   * \param[in] pNoF Final node pointer.
   */
  void setNodes(Bar *pNoI, Bar *pNoF);

  /*!
   * \brief Line data to Json.
   *
   * Convert all line data to a Json object, which can be used to save the data
   * to a file.
   *
   * To load a line with data contained in an Json object, use \b fromJson().
   *
   * \return A Json object filled with line data.
   */
  QJsonObject toJson();

  /*!
   * \brief Load line data from a Json object.
   *
   * Load line data stored with \b toJson();
   *
   * \param[in] lineJson Json object with line data.
   */
  void fromJson(QJsonObject &lineJson);

  /*!
   * \brief Update position.
   *
   * Recalculate line position and redraw. Used when nodes change position.
   */
  void updatePosition();

  /*!
   * \brief Bounding Rect.
   *
   * Reimplementation of the boundingRect() function from QGraphicsObject.
   */
  QRectF boundingRect() const Q_DECL_OVERRIDE;

  /*!
   * \brief Line shape.
   *
   * Reimplementation of the shape() function from QGraphicsObject.
   */
  QPainterPath shape() const Q_DECL_OVERRIDE;

  /*!
   * \brief Item change.
   *
   * Reimplementation of the virtual function itemChange from QGraphicsObject.
   * Used to show the information box when selected.
   */
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) Q_DECL_OVERRIDE;

signals:
  /*****************************************************************************
   * Signals.
   ****************************************************************************/
  /*!
   * \brief Event double click.
   *
   * Emited when a double click happens.
   */
  void eventDoubleClick();

protected:
  /*****************************************************************************
   * Protected methods.
   ****************************************************************************/
  /*!
   * \brief Mouse double click event.
   *
   * Handles the double click event.
   * This just emits a eventDoubleClick() signal.
   *
   * \param[in] event Event information, not used.
   */
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

  /*!
   * \brief Paint line.
   *
   * Reiplementation from QGraphicsObject. Used to draw the line in a systemView
   * object.
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  /*****************************************************************************
   * Static private data.
   ****************************************************************************/
  static const int8_t kImpedanceMatrixMap[3][3];

  /*****************************************************************************
   * Private properties.
   ****************************************************************************/
  /*!
   * \brief Network.
   *
   * Network where the line is.
   */
  QPointer<Network> network_;

  /*!
   * \brief Pointer to the initial node.
   *
   *  Smart pointer for the initial node.
   */
  QPointer<Bar> pNoI_;

  /*!
   * \brief Pointer to the final node.
   *
   * Smart pointer for the final node.
   */
  QPointer<Bar> pNoF_;

  /*!
   * \brief Line impedance.
   *
   * Holds the line impedance values. For mode information check the z functions.
   */
  complex<double> z_[6];

  /*!
   * \brief Line current.
   *
   * Holds the line current.
   */
  complex<double> i_[3];

  /*!
   * \brief Line power loss.
   *
   * Holds the line loss.
   */
  complex<double> loss_[3];

  /*!
   * \brief Length.
   *
   * Holds the line lenght.
   */
  double length_;

  /*!
   * \brief Line coordenates.
   *
   * Holds the line coordenates.
   */
  QLineF coords;

  /*!
   * \brief Selection area.
   *
   * Polygon containing the line selection area.
   */
  QPolygonF selectionArea;

  /*!
   * \brief Line information box pointer.
   *
   * Used to create and delete an information box.
   */
  QPointer<InfoLine> infoLine;

  /*****************************************************************************
   * Private methods.
   ****************************************************************************/
  /*!
   * \brief Show line information.
   *
   * Creates an information box object with the line results (current and loss).
   */
  void showInfo();

  /*!
   * \brief Hode line information.
   *
   * Hides (destroys) the line information box.
   */
  void hideInfo();
};

#endif  // MODELS_LINE_H

/*!
 * \}
 */
