#include "unit.h"

/*******************************************************************************
 * lengthUnitToStr.
 ******************************************************************************/
QString Unit::lengthUnitToStr(LengthUnit lengthUnit)
{
  switch (lengthUnit) {
  case kMeter:
    return "m";
    break;

  case kKiloMeter:
    return "km";
    break;

  case kFeet:
    return "ft";
    break;

  case kMile:
    return "mi";
    break;

  default:
    return "Unknow";
    break;
  }
}

/*******************************************************************************
 * impedanceUnitToStr.
 ******************************************************************************/
QString Unit::impedanceUnitToStr(ImpedanceUnit impedanceUnit)
{
  switch (impedanceUnit) {
  case kOhm:
    return "ohm";
    break;

  case kOhmPerMeter:
    return "ohm/m";
    break;

  case kOhmPerKilometer:
    return "ohm/km";
    break;

  case kOhmPerFeet:
    return "ohm/ft";
    break;

  case kOhmPerMile:
    return "ohm/mi";
    break;

  default:
    return "Unknow";
    break;
  }
}

/*******************************************************************************
 * powerUnitToStr.
 ******************************************************************************/
QString Unit::powerUnitToStr(PowerUnit powerUnit)
{
  switch (powerUnit) {
  case kVA:
    return "VA";
    break;

  case kKiloVA:
    return "kVA";
    break;

  case kMegaVa:
    return "MVA";
    break;

  default:
    return "Unknow";
    break;
  }
}


/*******************************************************************************
 * voltageUnitToStr.
 ******************************************************************************/
QString Unit::voltageUnitToStr(VoltageUnit voltageUnit)
{
  switch (voltageUnit) {
  case kVolts:
    return "V";
    break;

  case kKiloVolts:
    return "kV";
    break;

  default:
    return "Unknow";
    break;
  }
}

QString Unit::currentUnitToStr(Unit::CurrentUnit currentUnit)
{
  switch (currentUnit) {
  case kAmpere:
    return "A";
    break;

  case kKiloAmpere:
    return "kA";
    break;

  default:
    return "Unknow";
    break;
  }
}
