#include "infoline.h"
#include <QPainter>
#include <QString>
#include <complex>
#include "math_constants.h"
#include "datatable.h"
#include "models/network.h"
#include "unit.h"

/*******************************************************************************
 * Static constants.
 ******************************************************************************/
const int InfoLine::kTableRows  = 4;
const int InfoLine::kTableColums = 3;
const double InfoLine::kBoxWidth= 135.0*kTableColums;
const double InfoLine::kBoxHeight = 37.5*kTableRows;
const double InfoLine::kLineWidth = 2.0;
const double InfoLine::kMarginHeadTop = 5.0;
const double InfoLine::kMarginHeadBot = 5.0;
const double InfoLine::kMarginTop = 5.0;
const double InfoLine::kMarginBot = 5.0;
const double InfoLine::kMarginLeft = 5.0;
const double InfoLine::kMarginRight = 5.0;
const double InfoLine::kBoxBaseHeight = 30.0;
const double InfoLine::kBoxBaseWidth = 60.0;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
InfoLine::InfoLine(Line *line) :
  line_(line)
{
  setZValue(2);
  double px = (line->pNoI()->x() + line->pNoF()->x()) / 2 +
              line->network()->xOffset;
  double py = (line->pNoI()->y() + line->pNoF()->y()) / 2 +
              line->network()->yOffset;

  setPos(px,py);
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
InfoLine::~InfoLine()
{

}

/*******************************************************************************
 * Paint.
 ******************************************************************************/
void InfoLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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
  QString headText = "(" + line_->network()->name + ") " + "Line " +
                     QString::number(line_->nodes.first) + " -> " +
                     QString::number(line_->nodes.second);
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

  // Current head.
  QString table0x1 = "Current [";
  table0x1.append(Unit::currentUnitToStr(Network::currentUnit));
  table0x1.append("]");
  dataTable.setText(table0x1, 0, 1);

  // Loss head.
  QString table0x2 = "Loss [W , VAR]";
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

  // Current Phase A.
  QString table1x1 = QString::number(abs(line_->i(0, Network::currentUnit))*3.0,
                                     'g', 6);
  table1x1.append(" / ");
  table1x1.append(QString::number(arg(line_->i(0,
                                      Network::currentUnit))*180.0/kPI, 'g', 6));
  table1x1.append("°");
  dataTable.setText(table1x1, 1, 1);

  // Loss Phase A.
  QString table1x2 = QString::number(line_->loss(0, Network::powerUnit).real(),
                                     'g', 6);
  table1x2.append(" , ");
  table1x2.append(QString::number(line_->loss(0, Network::powerUnit).imag(), 'g',
                                  6));
  dataTable.setText(table1x2, 1, 2);

  // Current Phase B.
  QString table2x1 = QString::number(abs(line_->i(1, Network::currentUnit))*3.0,
                                     'g', 6);
  table2x1.append(" / ");
  table2x1.append(QString::number(arg(line_->i(1,
                                      Network::currentUnit))*180.0/kPI, 'g', 6));
  table2x1.append("°");
  dataTable.setText(table2x1, 2, 1);

  // Loss Phase B.
  QString table2x2 = QString::number(line_->loss(1, Network::powerUnit).real(),
                                     'g', 6);
  table2x2.append(" , ");
  table2x2.append(QString::number(line_->loss(1, Network::powerUnit).imag(), 'g',
                                  6));
  dataTable.setText(table2x2, 2, 2);

  // Current Phase C.
  QString table3x1 = QString::number(abs(line_->i(2, Network::currentUnit))*3.0,
                                     'g', 6);
  table3x1.append(" / ");
  table3x1.append(QString::number(arg(line_->i(2,
                                      Network::currentUnit))*180.0/kPI, 'g', 6));
  table3x1.append("°");
  dataTable.setText(table3x1, 3, 1);

  // Loss Phase C.
  QString table3x2 = QString::number(line_->loss(2, Network::powerUnit).real(),
                                     'g', 6);
  table3x2.append(" / ");
  table3x2.append(QString::number(line_->loss(2, Network::powerUnit).imag(), 'g',
                                  6));
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
void InfoLine::setHeadFont(QPainter *painter)
{
  // Set head font size.
  QFont font;
  font.setPointSize(font.pointSize() * 1.25);
  painter->setFont(font);
}

/*******************************************************************************
 * Set normal font size.
 ******************************************************************************/
void InfoLine::setNormalFont(QPainter *painter)
{
  // Set normal font size.
  QFont font;
  font.setPointSize(painter->font().pointSize() / 1.25);
  painter->setFont(font);
}

/*******************************************************************************
 * Draw main rectangle.
 ******************************************************************************/
void InfoLine::drawMainRectangle(QRectF mainRect,double lineWidth,
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
void InfoLine::drawHeadSeparator(QRectF headRect, QPainter *painter)
{

  QPointF p1(headRect.x(), headRect.y() + headRect.height());
  QPointF p2(headRect.x() + headRect.width(), headRect.y() + headRect.height());

  painter->drawLine(p1, p2);
}

/*******************************************************************************
 * Draw head text.
 ******************************************************************************/
void InfoLine::drawHeadText(QRectF headRect, double marginTop, QString &text,
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
QRectF InfoLine::boundingRect() const
{
  return QRectF(-kBoxWidth / 2, -kBoxHeight - kBoxBaseHeight, kBoxWidth,
                kBoxHeight + kBoxBaseHeight);
}
