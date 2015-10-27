#include "infobar.h"

#include <QPainter>
#include <QString>
#include <complex>
#include "math_constants.h"
#include "datatable.h"
#include "models/network.h"
#include "customtypes.h"

/*******************************************************************************
 * Static constants.
 ******************************************************************************/
const int InfoBar::kTableRows  = 4;
const int InfoBar::kTableColums = 3;
const double InfoBar::kBoxWidth= 135.0*kTableColums;
const double InfoBar::kBoxHeight = 37.5*kTableRows;
const double InfoBar::kLineWidth = 2.0;
const double InfoBar::kMarginHeadTop = 5.0;
const double InfoBar::kMarginHeadBot = 5.0;
const double InfoBar::kMarginTop = 5.0;
const double InfoBar::kMarginBot = 5.0;
const double InfoBar::kMarginLeft = 5.0;
const double InfoBar::kMarginRight = 5.0;
const double InfoBar::kBoxBaseHeight = 30.0;
const double InfoBar::kBoxBaseWidth = 60.0;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
InfoBar::InfoBar(Bar *bar) :
  bar_(bar)
{
  setZValue(2);
  double px = bar->x() + bar->network->xOffset;
  double py = bar->y() + bar->network->yOffset - bar->boundingRect().height() / 2;

  setPos(px,py);
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
InfoBar::~InfoBar()
{

}

/*******************************************************************************
 * Paint.
 ******************************************************************************/
void InfoBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  // Draw main rectangle.
  QRectF mainRect(-kBoxWidth / 2, -kBoxHeight - kBoxBaseHeight, kBoxWidth,
                  kBoxHeight);
  drawMainRectangle(mainRect, kLineWidth, painter);


  double headHeight = kMarginHeadTop +kMarginHeadBot + painter->font().pointSize()
                      *1.25;

  QRectF headRect(mainRect.x(), mainRect.y(), mainRect.width(), headHeight);

  // Draw Head separation.
  drawHeadSeparator(headRect, painter);

  // Draw head text.
  setHeadFont(painter);
  QString headText = "(" + bar_->network->name + ") " + "Bar " +
                     QString::number(bar_->id);
  drawHeadText(headRect, kMarginHeadTop, headText, painter);

  // Revert font size.
  setNormalFont(painter);

  // Draw data table.
  QRectF tableRect;
  tableRect.setX(mainRect.x() + kMarginLeft);
  tableRect.setY(mainRect.y() + headRect.height() + kMarginTop);
  tableRect.setWidth(mainRect.width() - kMarginRight);
  tableRect.setHeight(mainRect.height() - headRect.height() - kMarginTop -
                      kMarginBot);

  DataTable dataTable(kTableRows, kTableColums);
  dataTable.setSize(tableRect);

  // Voltage head.
  QString table0x1 = "Voltage [";
  table0x1.append(Unit::voltageUnitToStr(Network::voltageUnit));
  table0x1.append("]");
  dataTable.setText(table0x1, 0, 1);

  // Current head.
  QString table0x2 = "Current [";
  table0x2.append(Unit::currentUnitToStr(Network::currentUnit));
  table0x2.append("]");
  dataTable.setText(table0x2, 0, 2);

  // Phase A head.
  QString table1x0 = "Phase A";
  dataTable.setText(table1x0, 1, 0);

  // Phase B head.
  QString table2x0 = "Phase B";
  dataTable.setText(table2x0, 2, 0);

  // Phase C head.
  QString table3x0 = "Phase C";
  dataTable.setText(table3x0, 3, 0);

  // Voltage Phase A.
  QString table1x1 = QString::number(abs(bar_->rV(0, Network::voltageUnit)));
  table1x1.append(" / ");
  table1x1.append(QString::number(arg(bar_->rV(0,
                                      Network::voltageUnit))*180.0/kPI));
  table1x1.append("°");
  dataTable.setText(table1x1, 1, 1);

  // Current Phase A.
  QString table1x2 = QString::number(abs(bar_->rI(0, Network::currentUnit)));
  table1x2.append(" / ");
  table1x2.append(QString::number(std::arg(bar_->rI(0,
                                  Network::currentUnit))*180.0/kPI));
  table1x2.append("°");
  dataTable.setText(table1x2, 1, 2);

  // Voltage Phase B.
  QString table2x1 = QString::number(abs(bar_->rV(1, Network::voltageUnit)));
  table2x1.append(" / ");
  table2x1.append(QString::number(arg(bar_->rV(1,
                                      Network::voltageUnit))*180.0/kPI));
  table2x1.append("°");
  dataTable.setText(table2x1, 2, 1);

  // Current Phase B.
  QString table2x2 = QString::number(abs(bar_->rI(1, Network::currentUnit)));
  table2x2.append(" / ");
  table2x2.append(QString::number(arg(bar_->rI(1,
                                      Network::currentUnit))*180.0/kPI));
  table2x2.append("°");
  dataTable.setText(table2x2, 2, 2);

  // Voltage Phase C.
  QString table3x1 = QString::number(abs(bar_->rV(2, Network::voltageUnit)));
  table3x1.append(" / ");
  table3x1.append(QString::number(arg(bar_->rV(2,
                                      Network::voltageUnit))*180.0/kPI));
  table3x1.append("°");
  dataTable.setText(table3x1, 3, 1);

  // Current Phase C.
  QString table3x2 = QString::number(abs(bar_->rI(2, Network::currentUnit)));
  table3x2.append(" / ");
  table3x2.append(QString::number(arg(bar_->rI(2,
                                      Network::currentUnit))*180.0/kPI));
  table3x2.append("°");
  dataTable.setText(table3x2, 3, 2);

  dataTable.drawTable(painter);

  // Draw base.
  painter->setPen(QPen(Qt::black, kLineWidth, Qt::SolidLine));
  painter->setBrush(Qt::black);
  QPolygonF pol_base;
  pol_base << QPointF(-kBoxBaseWidth/2, -kBoxBaseHeight) << QPointF(0.0, 0.0)
           << QPointF(kBoxBaseWidth/2, -kBoxBaseHeight);
  painter->drawPolygon(pol_base);

}

/*******************************************************************************
 * Set head font size.
 ******************************************************************************/
void InfoBar::setHeadFont(QPainter *painter)
{
  // Set head font size.
  QFont font;
  font.setPointSize(font.pointSize() * 1.25);
  painter->setFont(font);
}

/*******************************************************************************
 * Set normal font size.
 ******************************************************************************/
void InfoBar::setNormalFont(QPainter *painter)
{
  // Set normal font size.
  QFont font;
  font.setPointSize(painter->font().pointSize() / 1.25);
  painter->setFont(font);
}

/*******************************************************************************
 * Draw main rectangle.
 ******************************************************************************/
void InfoBar::drawMainRectangle(QRectF mainRect,double lineWidth,
                                QPainter *painter)
{
  // Draw main rectangle.
  painter->setPen(QPen(Qt::black, lineWidth, Qt::SolidLine));
  painter->setBrush(Qt::white);
  painter->drawRect(mainRect);

}

/*******************************************************************************
 * Draw head separator.
 ******************************************************************************/
void InfoBar::drawHeadSeparator(QRectF headRect, QPainter *painter)
{

  QPointF p1(headRect.x(), headRect.y() + headRect.height());
  QPointF p2(headRect.x() + headRect.width(), headRect.y() + headRect.height());

  painter->drawLine(p1, p2);
}

/*******************************************************************************
 * Draw head text.
 ******************************************************************************/
void InfoBar::drawHeadText(QRectF headRect, double marginTop, QString &text,
                           QPainter *painter)
{
  // Draw Head Text.
  painter->drawText(headRect.x(), headRect.y() + marginTop,
                    headRect.width(), painter->font().pointSize(),
                    Qt::AlignCenter, text);
}

/*******************************************************************************
 * Bounding Rectangle.
 ******************************************************************************/
QRectF InfoBar::boundingRect() const
{
  return QRectF(-kBoxWidth / 2, -kBoxHeight - kBoxBaseHeight, kBoxWidth,
                kBoxHeight + kBoxBaseHeight);
}
