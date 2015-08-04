#include "pnbar.h"

#include <QGraphicsScene>

PnBar::PnBar(uint32_t id)
    : id_(id),
      inputV_(0, 0),
      inputSg_(0, 0),
      inputSl_(0, 0),
      outputV_(0, 0),
      outputSg_(0, 0),
      infobox_(NULL) {
  setFlag(ItemIsSelectable);
  setZValue(1);
}

PnBar::~PnBar() { removeLines(); }

// Id
uint32_t PnBar::Id() { return id_; }

void PnBar::setId(uint32_t id) { id_ = id; }

// Input Voltage
complex<double> PnBar::inputV() { return inputV_; }

void PnBar::setInputV(complex<double> v) { inputV_ = v; }

// Input Sg
complex<double> PnBar::inputSg() { return inputSg_; }

void PnBar::setInputSg(complex<double> sg) { inputSg_ = sg; }

// Input Sl
complex<double> PnBar::inputSl() { return inputSl_; }

void PnBar::setInputSl(complex<double> sl) { inputSl_ = sl; }

// Output V
complex<double> PnBar::outputV() { return outputV_; }

void PnBar::setOutputV(complex<double> v) { outputV_ = v; }

// Output Sg
complex<double> PnBar::outputSg() { return outputSg_; }

void PnBar::setOutputSg(complex<double> sg) { outputSg_ = sg; }

complex<double> PnBar::outputI() {
  complex<double> I;
  I = conj(outputSg_ - inputSl_) / conj(outputV_);
  return I;
}

void PnBar::setOutputI(complex<double> i) {
  outputSg_ = conj(i * conj(outputV_) - inputSl_);
}

void PnBar::addLine(PnLine *line) { lines_.append(line); }

void PnBar::removeLine(PnLine *line) {
  int index = lines_.indexOf(line);

  if (index != -1) lines_.removeAt(index);
}

void PnBar::removeLines() {
  foreach (PnLine *line, lines_) {
    line->NoI()->removeLine(line);
    line->NoF()->removeLine(line);
    scene()->removeItem(line);
    delete line;
  }
}

QVariant PnBar::itemChange(QGraphicsItem::GraphicsItemChange change,
                           const QVariant &value) {
  if (change == ItemSelectedChange) {
    if (value == true) {
      qreal px = pos().x();
      qreal py = pos().y() - boundingRect().height() / 2;

      QString txt;
      txt.append(tr("Id: ")).append(QString::number(id_)).append(tr("\n"));

      txt.append("Voltage: ")
          .append(QString::number(abs(outputV_)))
          .append(" | ")
          .append(QString::number(arg(outputV_)))
          .append("º [V]\n");

      txt.append(tr("Current: "))
          .append(QString::number(abs(outputI())))
          .append(" | ")
          .append(QString::number(arg(outputI())))
          .append("º [A]\n");

      txt.append("Genetared: ").append(QString::number(outputSg_.real()));

      if (outputSg_.imag() >= 0)
        txt.append(" + ");
      else
        txt.append(" - ");

      txt.append(QString::number(outputSg_.imag())).append("i [VA]\n");

      txt.append("Consumed: ").append(QString::number(inputSl_.real()));

      if (inputSl_.imag() >= 0)
        txt.append(" + ");
      else
        txt.append(" - ");

      txt.append(QString::number(inputSl_.imag())).append("i [VA]\n");

      infobox_ = new PnInfoBox(px, py, txt);
      scene()->addItem(infobox_);

    } else {
      scene()->removeItem(infobox_);
      infobox_ = NULL;
    }
  }

  return QGraphicsObject::itemChange(change, value);
}
