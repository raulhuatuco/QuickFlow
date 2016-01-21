#include "barreduction.h"
#include <complex>

using std::complex;

BarReduction::BarReduction()
{
}

BarReduction::~BarReduction()
{
}

Network *BarReduction::getNetwork()
{
  return network_;
}

void BarReduction::setNetwork(Network *network)
{
  network_ = network;
}

int32_t BarReduction::reduce()
{

  if (network_.isNull())
    return -1;

  foreach(Bar *bar, network_->bars) {
    if(barAbsPower(bar) < 1e-9) {
      if(bar->lines.size() == 2) {
        Droped *drop = new Droped;
        drop->bar = bar;

        drop->l1 = bar->lines[0];
        drop->l2 = bar->lines[1];

        network_->removeBar(drop->bar);
        network_->removeLine(drop->l1);
        network_->removeLine(drop->l2);

        drop->equivalentLine = new Line;

        if(drop->l1->pNoI() != bar) {
          drop->l1->pNoI()->removeLine(drop->l1);
          drop->equivalentLine->nodes.first = drop->l1->pNoI()->id();
        } else {
          drop->l1->pNoF()->removeLine(drop->l1);
          drop->equivalentLine->nodes.first = drop->l1->pNoF()->id();
        }

        if(drop->l2->pNoI() != bar) {
          drop->l2->pNoI()->removeLine(drop->l2);
          drop->equivalentLine->nodes.second = drop->l2->pNoI()->id();
        } else {
          drop->l2->pNoF()->removeLine(drop->l2);
          drop->equivalentLine->nodes.second = drop->l2->pNoF()->id();
        }

        if(drop->equivalentLine->nodes.first > drop->equivalentLine->nodes.second) {
          int32_t temp = drop->equivalentLine->nodes.second;
          drop->equivalentLine->nodes.second = drop->equivalentLine->nodes.first;
          drop->equivalentLine->nodes.first = temp;
          Line *ltemp = drop->l2;
          drop->l2 = drop->l1;
          drop->l1 = ltemp;
        }

        drop->bar->removeLine(drop->l1);
        drop->bar->removeLine(drop->l2);

        drop->equivalentLine->setZ(0, drop->l1->z(0) + drop->l2->z(0));
        drop->equivalentLine->setZ(1, drop->l1->z(1) + drop->l2->z(1));
        drop->equivalentLine->setZ(2, drop->l1->z(2) + drop->l2->z(2));
        drop->equivalentLine->setZ(3, drop->l1->z(3) + drop->l2->z(3));
        drop->equivalentLine->setZ(4, drop->l1->z(4) + drop->l2->z(4));
        drop->equivalentLine->setZ(5, drop->l1->z(5) + drop->l2->z(5));

        network_->addLine(drop->equivalentLine);
        droped.push(drop);

      } else if((bar->lines.size() == 1) && (bar->id() != 0)) {
        Droped *drop = new Droped;
        drop->bar = bar;
        drop->equivalentLine = NULL;
        drop->l1 = bar->lines[0];
        drop->l2 = NULL;

        network_->removeBar(drop->bar);
        network_->removeLine(drop->l1);

        if(drop->l1->pNoI() != bar) {
          drop->l1->pNoI()->removeLine(drop->l1);
        } else {
          drop->l1->pNoF()->removeLine(drop->l1);
        }

        drop->bar->removeLine(drop->l1);

        droped.push(drop);
      }
    }
  }

  return droped.size();
}

void BarReduction::expand()
{
  if(network_.isNull())
    return;

  int32_t ndroped =droped.size();
  complex<double> rV[3];

  for(int i =0; i< ndroped; i++) {
    Droped *drop = droped.pop();

    if(drop->equivalentLine != NULL) {
      network_->removeLine(drop->equivalentLine);
      drop->equivalentLine->pNoI()->removeLine(drop->equivalentLine);
      drop->equivalentLine->pNoF()->removeLine(drop->equivalentLine);

      network_->addBar(drop->bar);
      network_->addLine(drop->l1);
      network_->addLine(drop->l2);

      complex<double> lI[3];

      lI[0] = drop->equivalentLine->i(0);
      lI[1] = drop->equivalentLine->i(1);
      lI[2] = drop->equivalentLine->i(2);

      drop->l1->setI(0, lI[0]);
      drop->l1->setI(1, lI[1]);
      drop->l1->setI(2, lI[2]);
      drop->l2->setI(0, lI[0]);
      drop->l2->setI(1, lI[1]);
      drop->l2->setI(2, lI[2]);

      complex<double> l1L[3];
      l1L[0] = (drop->l1->z(0)/(drop->l1->z(0) + drop->l2->z(0)))
               *drop->equivalentLine->loss(0);
      l1L[1] = (drop->l1->z(3)/(drop->l1->z(3) + drop->l2->z(3)))
               *drop->equivalentLine->loss(1);
      l1L[2] = (drop->l1->z(5)/(drop->l1->z(5) + drop->l2->z(5)))
               *drop->equivalentLine->loss(2);

      if(!qIsNaN(l1L[0].real()) || !qIsNaN(l1L[0].imag()))
        drop->l1->setLoss(0, l1L[0]);
      else
        drop->l1->setLoss(0, 0.0);

      if(!qIsNaN(l1L[1].real()) || !qIsNaN(l1L[1].imag()))
        drop->l1->setLoss(1, l1L[1]);
      else
        drop->l1->setLoss(1, 0.0);

      if(!qIsNaN(l1L[2].real()) || !qIsNaN(l1L[2].imag()))
        drop->l1->setLoss(2, l1L[2]);
      else
        drop->l1->setLoss(2, 0.0);

      complex<double> l2L[3];
      l2L[0] = (drop->l2->z(0)/(drop->l1->z(0) + drop->l2->z(0)))
               *drop->equivalentLine->loss(0);
      l2L[1] = (drop->l2->z(3)/(drop->l1->z(3) + drop->l2->z(3)))
               *drop->equivalentLine->loss(1);
      l2L[2] = (drop->l2->z(5)/(drop->l1->z(5) + drop->l2->z(5)))
               *drop->equivalentLine->loss(2);

      if(!qIsNaN(l2L[0].real()) || !qIsNaN(l2L[0].imag()))
        drop->l2->setLoss(0, l2L[0]);
      else
        drop->l2->setLoss(0, 0.0);

      if(!qIsNaN(l2L[1].real()) || !qIsNaN(l2L[1].imag()))
        drop->l2->setLoss(1, l2L[1]);
      else
        drop->l2->setLoss(1, 0.0);

      if(!qIsNaN(l2L[2].real()) || !qIsNaN(l2L[2].imag()))
        drop->l2->setLoss(2, l2L[2]);
      else
        drop->l2->setLoss(2, 0.0);

      rV[0] = drop->equivalentLine->pNoI()->rV(0);
      rV[0] -= drop->l1->z(0)*drop->l1->i(0);

      rV[1] = drop->equivalentLine->pNoI()->rV(1);
      rV[1] -= drop->l1->z(3)*drop->l1->i(1);

      rV[2] = drop->equivalentLine->pNoI()->rV(2);
      rV[2] -= drop->l1->z(5)*drop->l1->i(2);

      delete drop->equivalentLine;
    } else {
      network_->addBar(drop->bar);
      network_->addLine(drop->l1);

      if(drop->l1->pNoI()!=drop->bar) {
        rV[0] = drop->l1->pNoI()->rV(0);
        rV[1] = drop->l1->pNoI()->rV(1);
        rV[2] = drop->l1->pNoI()->rV(2);
      } else {
        rV[0] = drop->l1->pNoF()->rV(0);
        rV[1] = drop->l1->pNoF()->rV(1);
        rV[2] = drop->l1->pNoF()->rV(2);
      }

      drop->l1->setI(0, 0.0);
      drop->l1->setI(1, 0.0);
      drop->l1->setI(2, 0.0);
    }

    drop->bar->setRV(0, rV[0]);
    drop->bar->setRV(1, rV[1]);
    drop->bar->setRV(2, rV[2]);
    drop->bar->setRI(0, 0.0);
    drop->bar->setRI(1, 0.0);
    drop->bar->setRI(2, 0.0);

    delete drop;
  }
}

double BarReduction::barAbsPower(Bar *bar)
{
  double power;

  power =  abs(bar->sh(0) - bar->si(0));
  power +=  abs(bar->sh(1) - bar->si(1));
  power +=  abs(bar->sh(2) - bar->si(2));

  return power;
}



