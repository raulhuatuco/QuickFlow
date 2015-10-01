#include "project.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include "models/bar.h"
#include "models/line.h"

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Project::Project() :
  network(new Network),
  name(""),
  filePath("")
{
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Project::~Project()
{
  delete network;
}

/*******************************************************************************
 * maxIterations.
 ******************************************************************************/
uint32_t Project::maxIterations()
{
  return network->maxIterations;
}

/*******************************************************************************
 * setMaxIterations.
 ******************************************************************************/
void Project::setMaxIterations(uint32_t maxIterations)
{
  network->maxIterations = maxIterations;
}

/*******************************************************************************
 * minError.
 ******************************************************************************/
double Project::minError()
{
  return network->minError;
}

/*******************************************************************************
 * setMinError.
 ******************************************************************************/
void Project::setMinError(double minError)
{
  network->minError = minError;
}

/*******************************************************************************
 * voltageBase.
 ******************************************************************************/
double Project::voltageBase()
{
  return network->voltageBase;
}

/*******************************************************************************
 * setVoltageBase.
 ******************************************************************************/
void Project::setVoltageBase(double voltageBase)
{
  network->voltageBase = voltageBase;
}

/*******************************************************************************
 * powerBase.
 ******************************************************************************/
double Project::powerBase()
{
  return network->powerBase;
}

/*******************************************************************************
 * setPowerBase.
 ******************************************************************************/
void Project::setPowerBase(double powerBase)
{
  network->powerBase = powerBase;
}

/*******************************************************************************
 * lengthUn.
 ******************************************************************************/
Unit::LengthUnit Project::lengthUnit()
{
  return network->lengthUnit;
}

/*******************************************************************************
 * setLengthUn.
 ******************************************************************************/
void Project::setLengthUn(Unit::LengthUnit lengthUn)
{
  network->lengthUnit = lengthUn;
}

/*******************************************************************************
 * impedanceUn.
 ******************************************************************************/
Unit::ImpedanceUnit Project::impedanceUnit()
{
  return network->impedanceUnit;
}

/*******************************************************************************
 * setImpedanceUn.
 ******************************************************************************/
void Project::setImpedanceUn(Unit::ImpedanceUnit impedanceUn)
{
  network->impedanceUnit = impedanceUn;
}

/*******************************************************************************
 * voltageUn.
 ******************************************************************************/
Unit::VoltageUnit Project::voltageUnit()
{
  return network->voltageUnit;
}

/*******************************************************************************
 * setVoltageUn.
 ******************************************************************************/
void Project::setVoltageUn(Unit::VoltageUnit voltageUn)
{
  network->voltageUnit = voltageUn;
}

/*******************************************************************************
 * powerUn.
 ******************************************************************************/
Unit::PowerUnit Project::powerUnit()
{
  return network->powerUnit;
}

/*******************************************************************************
 * setPowerUn.
 ******************************************************************************/
void Project::setPowerUn(Unit::PowerUnit powerUn)
{
  network->powerUnit = powerUn;
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
  projectJson.insert("voltageBase", network->voltageBase);
  projectJson.insert("powerBase", network->powerBase);
  projectJson.insert("lengthUnit", network->lengthUnit);
  projectJson.insert("impedanceUnit", network->impedanceUnit);
  projectJson.insert("voltageUnit", network->voltageUnit);
  projectJson.insert("powerUnit", network->powerUnit);
  projectJson.insert("maxIterations",
                     static_cast<int32_t> (network->maxIterations));
  projectJson.insert("minError", network->minError);

  // Creates a QJsonArray filled with Bar data.
  QJsonArray barArray;
  foreach (Bar *bar, network->bars) {
    barArray << bar->toJson();
  }
  projectJson.insert("barArray", barArray);

  // Creates a QJsonArray filled with Line data.
  QJsonArray lineArray;
  foreach (Line *line, network->lines) {
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
  network->voltageBase = projectJson.value("voltageBase").toDouble();
  network->powerBase = projectJson.value("powerBase").toDouble();
  network->lengthUnit =
    static_cast<Unit::LengthUnit> (projectJson.value("lengthUnit").toInt());
  network->impedanceUnit =
    static_cast<Unit::ImpedanceUnit> (projectJson.value("impedanceUnit").toInt());
  network->voltageUnit =
    static_cast<Unit::VoltageUnit> (projectJson.value("voltageUnit").toInt());
  network->powerUnit =
    static_cast<Unit::PowerUnit> (projectJson.value("powerUnit").toInt());
  network->maxIterations = projectJson.value("maxIterations").toDouble();
  network->minError = projectJson.value("minError").toDouble();

  // Extract Bar data from a QJsonArray.
  QJsonArray barArray(projectJson.value("barArray").toArray());

  foreach (QJsonValue arrayValue, barArray) {
    QJsonObject barJson(arrayValue.toObject());

    Bar *bar = new Bar;
    bar->fromJson(barJson);

    if(!network->addBar(bar)) {
      return false;
    }
  }

// Extract Lines data from a QJsonArray.
  QJsonArray lineArray(projectJson.value("lineArray").toArray());

  foreach (QJsonValue arrayValue, lineArray) {
    QJsonObject lineJson(arrayValue.toObject());

    Line *line = new Line;
    line->fromJson(lineJson);

    if(!network->addLine(line))
      return false;
  }

// Close file
  file.close();

  return true;
}


