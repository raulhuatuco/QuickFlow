#include "shirmoharmmadi.h"
#include "radiallayer.h"

Shirmoharmmadi::Shirmoharmmadi(Network *network) :
  network_(network)
{

}

Shirmoharmmadi::~Shirmoharmmadi()
{

}

bool Shirmoharmmadi::solve()
{
  RadialLayer radLayer(network_);

  radLayer.calculate();

  double error = 10000;
  double iteration = 0;

  do {
  
  
  } while((error > network_->minError) && (iteration < network_->maxIterations));


}

