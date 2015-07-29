#include "pnbar.h"

#include <QGraphicsScene>

PnBar::PnBar(uint32_t id)
    : id_(id), v_(1,0), sg_(0,0), sl_(0,0), infobox_(NULL){
    setFlag(ItemIsSelectable);
    setZValue(1);
}

PnBar::~PnBar()
{
    removeLines();
}

uint32_t PnBar::getId()
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

QVariant PnBar::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedChange) {
        if(value == true) {
            qreal px = pos().x();
            qreal py = pos().y() - boundingRect().height()/2;

            QString txt;
            txt.append(tr("Id: ")).append(QString::number(id_)).append(tr("\n"));

            txt.append("Voltage: ").append(QString::number(abs(v_))).append(" | ").append(QString::number(arg(v_))).append("º [V]\n");

            txt.append(tr("Current: ")).append(QString::number(abs(getI()))).append(" | ").append(QString::number(arg(getI()))).append("º [A]\n");

            txt.append("Genetared: ").append(QString::number(sg_.real()));
            if(sg_.imag()>=0)
                txt.append(" + ");
            else
                txt.append(" - ");
            txt.append(QString::number(sg_.imag())).append("i [VA]\n");

            txt.append("Consumed: ").append(QString::number(sl_.real()));
            if(sl_.imag()>=0)
                txt.append(" + ");
            else
                txt.append(" - ");
            txt.append(QString::number(sl_.imag())).append("i [VA]\n");



            infobox_ = new PnInfoBox(px, py, txt);
            scene()->addItem(infobox_);

        } else{
            scene()->removeItem(infobox_);
            infobox_ = NULL;
        }
    }

    return QGraphicsObject::itemChange(change, value);
}
