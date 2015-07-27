#include "pnbar.h"

#include <QGraphicsScene>

PnBar::PnBar()
  : id_(0xFFFFFFFF), v_(1,0), sg_(0,0), sl_(0,0){
setFlag(ItemIsSelectable);
setZValue(1);
}

PnBar::~PnBar()
{

}

u_int32_t PnBar::getId()
{
  return id_;
}

complex<double> PnBar::getV()
{
  return v_;
}

complex<double> PnBar::getSg()
{
  return sg_;
}

complex<double> PnBar::getSl()
{
  return sl_;
}

complex<double> PnBar::getI()
{
  complex<double> I;
  I = conj(sg_-sl_)/conj(v_);
  return I;
}

void PnBar::setId(unsigned int id)
{
  id_ = id;
}

void PnBar::setV(complex<double> v)
{
  v_ = v;
}

void PnBar::setSl(complex<double> sl)
{
  sl_=sl;
}

void PnBar::addLine(PnLine *line)
{
  lines_.append(line);
}

void PnBar::removeLine(PnLine *line)
{
  int index = lines_.indexOf(line);

      if (index != -1)
        lines_.removeAt(index);
}

void PnBar::removeLines()
{
  foreach (PnLine *line, lines_) {
          line->getNoI ()->removeLine(line);
          line->getNoF()->removeLine(line);
          scene()->removeItem(line);
          delete line;
      }
}

