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
Project::Project()
{
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Project::~Project()
{
  foreach(Network *network, networks) {
    delete network;
  }
}

/*******************************************************************************
 * maxIterations.
 ******************************************************************************/
uint32_t Project::maxIterations()
{
  return Network::maxIterations;
}

/*******************************************************************************
 * setMaxIterations.
 ******************************************************************************/
void Project::setMaxIterations(uint32_t maxIterations)
{
  Network::maxIterations = maxIterations;
}

/*******************************************************************************
 * minError.
 ******************************************************************************/
double Project::minError()
{
  return Network::minError;
}

/*******************************************************************************
 * setMinError.
 ******************************************************************************/
void Project::setMinError(double minError)
{
  Network::minError = minError;
}

/*******************************************************************************
 * lengthUn.
 ******************************************************************************/
Unit::LengthUnit Project::lengthUnit()
{
  return Network::lengthUnit;
}

/*******************************************************************************
 * setLengthUn.
 ******************************************************************************/
void Project::setLengthUn(Unit::LengthUnit lengthUn)
{
  Network::lengthUnit = lengthUn;
}

/*******************************************************************************
 * impedanceUn.
 ******************************************************************************/
Unit::ImpedanceUnit Project::impedanceUnit()
{
  return Network::impedanceUnit;
}

/*******************************************************************************
 * setImpedanceUn.
 ******************************************************************************/
void Project::setImpedanceUn(Unit::ImpedanceUnit impedanceUn)
{
  Network::impedanceUnit = impedanceUn;
}

/*******************************************************************************
 * voltageUn.
 ******************************************************************************/
Unit::VoltageUnit Project::voltageUnit()
{
  return Network::voltageUnit;
}

/*******************************************************************************
 * setVoltageUn.
 ******************************************************************************/
void Project::setVoltageUn(Unit::VoltageUnit voltageUn)
{
  Network::voltageUnit = voltageUn;
}

/*******************************************************************************
 * powerUn.
 ******************************************************************************/
Unit::PowerUnit Project::powerUnit()
{
  return Network::powerUnit;
}

/*******************************************************************************
 * setPowerUn.
 ******************************************************************************/
void Project::setPowerUn(Unit::PowerUnit powerUn)
{
  Network::powerUnit = powerUn;
}

/*******************************************************************************
 * currentUnit.
 ******************************************************************************/
Unit::CurrentUnit Project::currentUnit()
{
  return Network::currentUnit;
}

/*******************************************************************************
 * setCurrentUnit.
 ******************************************************************************/
void Project::setCurrentUnit(Unit::CurrentUnit unit)
{
  Network::currentUnit = unit;
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
  projectJson.insert("maxIterations",
                     static_cast<int32_t> (Network::maxIterations));
  projectJson.insert("minError", Network::minError);
  projectJson.insert("lengthUnit", Network::lengthUnit);
  projectJson.insert("impedanceUnit", Network::impedanceUnit);
  projectJson.insert("voltageUnit", Network::voltageUnit);
  projectJson.insert("powerUnit", Network::powerUnit);
  projectJson.insert("currentUnit", Network::currentUnit);

  // Creates a QJsonArray filled with networks data.
  QJsonArray networkArray;
  foreach (Network *network, networks) {
    networkArray << network->toJson();
  }
  projectJson.insert("networkArray", networkArray);

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
  Network::maxIterations = projectJson.value("maxIterations").toDouble();
  Network::minError = projectJson.value("minError").toDouble();
  Network::lengthUnit =
    static_cast<Unit::LengthUnit> (projectJson.value("lengthUnit").toInt());
  Network::impedanceUnit =
    static_cast<Unit::ImpedanceUnit> (projectJson.value("impedanceUnit").toInt());
  Network::voltageUnit =
    static_cast<Unit::VoltageUnit> (projectJson.value("voltageUnit").toInt());
  Network::powerUnit =
    static_cast<Unit::PowerUnit> (projectJson.value("powerUnit").toInt());
  Network::currentUnit =
    static_cast<Unit::CurrentUnit> (projectJson.value("currentUnit").toInt());

  // Extract network data from a QJsonArray.
  QJsonArray networkArray(projectJson.value("networkArray").toArray());

  foreach (QJsonValue arrayValue, networkArray) {
    QJsonObject networkJson(arrayValue.toObject());

    Network *network = new Network;
    network->fromJson(networkJson);
    networks.insert(network->name, network);
  }

// Close file
  file.close();

  return true;
}


