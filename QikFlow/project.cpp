#include "project.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>

#include "pn/bar.h"
#include "pn/line.h"

/*******************************************************************************
 * Project.
 ******************************************************************************/
Project::Project() :
  pnNetwork(new PnNetwork),
  name(""),
  filePath("")
{
}

/*******************************************************************************
 * ~Project.
 ******************************************************************************/
Project::~Project()
{
  delete pnNetwork;
}

/*******************************************************************************
 * maxIterations.
 ******************************************************************************/
uint32_t Project::maxIterations()
{
  return pnNetwork->maxIterations;
}

/*******************************************************************************
 * setMaxIterations.
 ******************************************************************************/
void Project::setMaxIterations(uint32_t maxIterations)
{
  pnNetwork->maxIterations = maxIterations;
}

/*******************************************************************************
 * minError.
 ******************************************************************************/
double Project::minError()
{
  return pnNetwork->minError;
}

/*******************************************************************************
 * setMinError.
 ******************************************************************************/
void Project::setMinError(double minError)
{
  pnNetwork->minError = minError;
}

/*******************************************************************************
 * voltageBase.
 ******************************************************************************/
double Project::voltageBase()
{
  return pnNetwork->voltageBase;
}

/*******************************************************************************
 * setVoltageBase.
 ******************************************************************************/
void Project::setVoltageBase(double voltageBase)
{
  pnNetwork->voltageBase = voltageBase;
}

/*******************************************************************************
 * powerBase.
 ******************************************************************************/
double Project::powerBase()
{
  return pnNetwork->powerBase;
}

/*******************************************************************************
 * setPowerBase.
 ******************************************************************************/
void Project::setPowerBase(double powerBase)
{
  pnNetwork->powerBase = powerBase;
}

/*******************************************************************************
 * lengthUn.
 ******************************************************************************/
Unit::LengthUnit Project::lengthUnit()
{
  return pnNetwork->lengthUnit;
}

/*******************************************************************************
 * setLengthUn.
 ******************************************************************************/
void Project::setLengthUn(Unit::LengthUnit lengthUn)
{
  pnNetwork->lengthUnit = lengthUn;
}

/*******************************************************************************
 * impedanceUn.
 ******************************************************************************/
Unit::ImpedanceUnit Project::impedanceUnit()
{
  return pnNetwork->impedanceUnit;
}

/*******************************************************************************
 * setImpedanceUn.
 ******************************************************************************/
void Project::setImpedanceUn(Unit::ImpedanceUnit impedanceUn)
{
  pnNetwork->impedanceUnit = impedanceUn;
}

/*******************************************************************************
 * voltageUn.
 ******************************************************************************/
Unit::VoltageUnit Project::voltageUnit()
{
  return pnNetwork->voltageUnit;
}

/*******************************************************************************
 * setVoltageUn.
 ******************************************************************************/
void Project::setVoltageUn(Unit::VoltageUnit voltageUn)
{
  pnNetwork->voltageUnit = voltageUn;
}

/*******************************************************************************
 * powerUn.
 ******************************************************************************/
Unit::PowerUnit Project::powerUnit()
{
  return pnNetwork->powerUnit;
}

/*******************************************************************************
 * setPowerUn.
 ******************************************************************************/
void Project::setPowerUn(Unit::PowerUnit powerUn)
{
  pnNetwork->powerUnit = powerUn;
}

/*******************************************************************************
 * save.
 ******************************************************************************/
bool Project::save()
{
  // Try to open file.
  QFile file;
  file.setFileName(filePath);

  // Check for write permission.
  if (!file.open(QIODevice::WriteOnly))
    return false;

  // Create Json Configuration.
  QJsonObject projectJson;

  // Add project settings.
  projectJson.insert("name", name);

  // Add network settings.
  projectJson.insert("voltageBase", pnNetwork->voltageBase);
  projectJson.insert("powerBase", pnNetwork->powerBase);
  projectJson.insert("lengthUnit", pnNetwork->lengthUnit);
  projectJson.insert("impedanceUnit", pnNetwork->impedanceUnit);
  projectJson.insert("voltageUnit", pnNetwork->voltageUnit);
  projectJson.insert("powerUnit", pnNetwork->powerUnit);
  projectJson.insert("maxIterations",
                     static_cast<int32_t> (pnNetwork->maxIterations));
  projectJson.insert("minError", pnNetwork->minError);

  // Creates a QJsonArray filled with Bar data.
  QJsonArray barArray;
  foreach (PnBar *bar, pnNetwork->bars) {
    barArray << bar->toJson();
  }
  projectJson.insert("barArray", barArray);

  // Creates a QJsonArray filled with Line data.
  QJsonArray lineArray;
  foreach (PnLine *line, pnNetwork->lines) {
    lineArray << line->toJson();
  }
  projectJson.insert("lineArray", lineArray);

  // Create a Json document to conver Json data to text.
  QJsonDocument jsonDoc(projectJson);

  // Write to file.
  qint64 err = file.write(jsonDoc.toJson());
  file.close();

  return (err > -1) ? true : false;
}

/*******************************************************************************
 * saveAs.
 ******************************************************************************/
bool Project::saveAs(QString fileName)
{
  // Save old path.
  QString oldpath;
  oldpath = filePath;

  // Set new path.
  filePath = fileName;

  // try to save.
  if(!save()) {
    // Failed to save. Return to old path.
    filePath = oldpath;
    return false;
  }

  return true;
}

/*******************************************************************************
 * load.
 ******************************************************************************/
bool Project::load()
{
  // Try to open file.
  QFile file;
  file.setFileName(filePath);

  // Check for read permission.
  if (!file.open(QIODevice::ReadOnly))
    return false;

  // Create Doc to transform text to json.
  QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());

  // Chech if file is valid.
  if (jsonDoc.isNull() || !jsonDoc.isObject())
    return false;

  // Fill base Json object.
  QJsonObject projectJson;
  projectJson = jsonDoc.object();

  // Extract project info.
  name = projectJson.value("name").toString();

  // Extract network info.
  pnNetwork->voltageBase = projectJson.value("voltageBase").toDouble();
  pnNetwork->powerBase = projectJson.value("powerBase").toDouble();
  pnNetwork->lengthUnit =
    static_cast<Unit::LengthUnit> (projectJson.value("lengthUnit").toInt());
  pnNetwork->impedanceUnit =
    static_cast<Unit::ImpedanceUnit> (projectJson.value("impedanceUnit").toInt());
  pnNetwork->voltageUnit =
    static_cast<Unit::VoltageUnit> (projectJson.value("voltageUnit").toInt());
  pnNetwork->powerUnit =
    static_cast<Unit::PowerUnit> (projectJson.value("powerUnit").toInt());
  pnNetwork->maxIterations = projectJson.value("maxIterations").toDouble();
  pnNetwork->minError = projectJson.value("minError").toDouble();

  // Extract Bar data from a QJsonArray.
  QJsonArray barArray(projectJson.value("barArray").toArray());

  foreach (QJsonValue arrayValue, barArray) {
    QJsonObject barJson(arrayValue.toObject());

    PnBar *bar = new PnBar;
    bar->fromJson(barJson);

    if(!pnNetwork->addBar(bar)) {
      return false;
    }
  }

// Extract Lines data from a QJsonArray.
  QJsonArray lineArray(projectJson.value("lineArray").toArray());

  foreach (QJsonValue arrayValue, lineArray) {
    QJsonObject lineJson(arrayValue.toObject());

    PnLine *line = new PnLine;
    line->fromJson(lineJson);

    if(!pnNetwork->addLine(line))
      return false;
  }

// Close file
  file.close();

  return true;
}


