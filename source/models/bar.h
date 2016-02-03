#ifndef MODELS_BAR_H
#define MODELS_BAR_H

#include <complex>
#include <QGraphicsObject>
#include <QJsonObject>
#include <QPointer>
#include "models/line.h"
#include "models/network.h"

QT_BEGIN_NAMESPACE
class Line;
class Network;
QT_END_NAMESPACE

using std::complex;

class Bar : public QGraphicsObject
{
  Q_OBJECT

public:
  complex<double> v[3];
  complex<double> i[3];
  complex<double> shunt[3];
  complex<double> gen[3];
  complex<double> load[3];

  Bar();
  virtual ~Bar();

  QString name();
  bool setBarName(QString name);

  Network *network();
  void setNetwork(Network *network);

  const QVector<QPointer<Line>> &lines();
  void addLine(QPointer<Line> line);
  void removeLine(QPointer<Line> line);
  void removeAllLines();

  virtual QJsonObject toJson();
  virtual void fromJson(QJsonObject &jsonBar);

  virtual QString model();
  virtual bool loadResult(QString pnr);

  virtual bool edit();

signals:
  void eventSelected();

  void eventEdited();

  void eventDoubleClick();

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) Q_DECL_OVERRIDE;
private:
  QString name_;

  QPointer<Network> network_;

  QVector<QPointer<Line>> lines_;

  QGraphicsTextItem barLabel;

  /*****************************************************************************
  * Private methods.
  *****************************************************************************/
  virtual void adjustLabelPosition();

  virtual void showInfo();

  virtual void hideInfo();
};

#endif  // MODELS_BAR_H

/*!
 * \}
 */
