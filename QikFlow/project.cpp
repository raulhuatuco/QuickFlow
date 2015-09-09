#include "project.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"

/*******************************************************************************
 * Project.
 ******************************************************************************/
Project::Project() :
  pnNetwork(new PnNetwork),
  name(""),
  filepath("") {
}

/*******************************************************************************
 * ~Project.
 ******************************************************************************/
Project::~Project() {
  delete pnNetwork;
}

/*******************************************************************************
 * maxIterations.
 ******************************************************************************/
uint32_t Project::maxIterations() {
  return pnNetwork->maxIterations;
}

/*******************************************************************************
 * setMaxIterations.
 ******************************************************************************/
void Project::setMaxIterations(uint32_t maxIterations) {
  pnNetwork->maxIterations = maxIterations;
}

/*******************************************************************************
 * minError.
 ******************************************************************************/
double Project::minError() {
  return pnNetwork->minError;
}

/*******************************************************************************
 * setMinError.
 ******************************************************************************/
void Project::setMinError(double minError) {
  pnNetwork->minError = minError;
}

/*******************************************************************************
 * voltageBase.
 ******************************************************************************/
double Project::voltageBase() {
  return pnNetwork->voltageBase;
}

/*******************************************************************************
 * setVoltageBase.
 ******************************************************************************/
void Project::setVoltageBase(double voltageBase) {
  pnNetwork->voltageBase = voltageBase;
}

/*******************************************************************************
 * powerBase.
 ******************************************************************************/
double Project::powerBase() {
  return pnNetwork->powerBase;
}

/*******************************************************************************
 * setPowerBase.
 ******************************************************************************/
void Project::setPowerBase(double powerBase) {
  pnNetwork->powerBase = powerBase;
}

/*******************************************************************************
 * lengthUn.
 ******************************************************************************/
Unit::LengthUnit Project::lengthUn() {
  return pnNetwork->lengthUnit;
}

/*******************************************************************************
 * setLengthUn.
 ******************************************************************************/
void Project::setLengthUn(Unit::LengthUnit lengthUn) {
  pnNetwork->lengthUnit = lengthUn;
}

/*******************************************************************************
 * impedanceUn.
 ******************************************************************************/
Unit::ImpedanceUnit Project::impedanceUn() {
  return pnNetwork->impedanceUnit;
}

/*******************************************************************************
 * setImpedanceUn.
 ******************************************************************************/
void Project::setImpedanceUn(Unit::ImpedanceUnit impedanceUn) {
  pnNetwork->impedanceUnit = impedanceUn;
}

/*******************************************************************************
 * voltageUn.
 ******************************************************************************/
Unit::VoltageUnit Project::voltageUn() {
  return pnNetwork->voltageUnit;
}

/*******************************************************************************
 * setVoltageUn.
 ******************************************************************************/
void Project::setVoltageUn(Unit::VoltageUnit voltageUn) {
  pnNetwork->voltageUnit = voltageUn;
}

/*******************************************************************************
 * powerUn.
 ******************************************************************************/
Unit::PowerUnit Project::powerUn() {
  return pnNetwork->powerUnit;
}

/*******************************************************************************
 * setPowerUn.
 ******************************************************************************/
void Project::setPowerUn(Unit::PowerUnit powerUn) {
  pnNetwork->powerUnit = powerUn;
}

/*******************************************************************************
 * save.
 ******************************************************************************/
bool Project::save() {
  // Try to open file.
  QFile file;
  file.setFileName(filepath);

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
  foreach (PnBar *bar, pnNetwork->barMap) {
    barArray << barToJson(bar);
  }
  projectJson.insert("barArray", barArray);

  // Creates a QJsonArray filled with Line data.
  QJsonArray lineArray;
  foreach (PnLine *line, pnNetwork->lineIMap) {
    lineArray << lineToJson(line);
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
bool Project::saveAs(QString fileName) {
  // Try to open file.
  QFile file;
  file.setFileName(fileName);

  // Check for write permission.
  if (!file.open(QIODevice::WriteOnly))
    return false;

  // Create Json Configuration.
  QJsonObject projectJson;

  // Add project settings.
  projectJson.insert("name", name);
  projectJson.insert("maxIterations",
                     static_cast<int32_t> (pnNetwork->maxIterations));
  projectJson.insert("minError", pnNetwork->minError);

  // Add network settings.
  projectJson.insert("voltageBase", pnNetwork->voltageBase);
  projectJson.insert("powerBase", pnNetwork->powerBase);
  projectJson.insert("lengthUnit", pnNetwork->lengthUnit);
  projectJson.insert("impedanceUnit", pnNetwork->impedanceUnit);
  projectJson.insert("voltageUnit", pnNetwork->voltageUnit);
  projectJson.insert("powerUnit", pnNetwork->powerUnit);

  // Creates a QJsonArray filled with Bar data.
  QJsonArray barArray;
  foreach (PnBar *bar, pnNetwork->barMap) {
    barArray << barToJson(bar);
  }
  projectJson.insert("barArray", barArray);

  // Creates a QJsonArray filled with Line data.
  QJsonArray lineArray;
  foreach (PnLine *line, pnNetwork->lineIMap) {
    lineArray << lineToJson(line);
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
 * load.
 ******************************************************************************/
bool Project::load() {
  // Try to open file.
  QFile file;
  file.setFileName(filepath);

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
  pnNetwork->maxIterations = projectJson.value("maxIterations").toDouble();
  pnNetwork->minError = projectJson.value("minError").toDouble();

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

  // Extract Bar data from a QJsonArray.
  QJsonArray barArray(projectJson.value("barArray").toArray());

  foreach (QJsonValue arrayValue, barArray) {
    QJsonObject barJson(arrayValue.toObject());

    PnBar *bar = barFromJson(barJson);
    pnNetwork->addBar(bar);
  }

// Extract Lines data from a QJsonArray.
  QJsonArray lineArray(projectJson.value("lineArray").toArray());
  foreach (QJsonValue arrayValue, lineArray) {
    QJsonObject lineJson(arrayValue.toObject());

    PnLine *line = lineFromJson(lineJson);
    pnNetwork->addLine(line);
  }

// Close file
  file.close();

  return true;
}

/*******************************************************************************
 * barToJson.
 ******************************************************************************/
QJsonObject Project::barToJson(PnBar *bar) {
  // Transform Bar to Json Object.
  QJsonObject jsonBar;

  // Bar Id
  jsonBar.insert("id", static_cast<int>(bar->id));

  // Voltage
  jsonBar.insert("Va", bar->Va.real());
  jsonBar.insert("Vai", bar->Va.imag());
  jsonBar.insert("Vb", bar->Va.real());
  jsonBar.insert("Vbi", bar->Va.imag());
  jsonBar.insert("Vc", bar->Va.real());
  jsonBar.insert("Vci", bar->Va.imag());

  // Generated Power
  jsonBar.insert("Sga", bar->Va.real());
  jsonBar.insert("Sgai", bar->Va.imag());
  jsonBar.insert("Sgb", bar->Va.real());
  jsonBar.insert("Sgbi", bar->Va.imag());
  jsonBar.insert("Sgc", bar->Va.real());
  jsonBar.insert("Sgci", bar->Va.imag());

  // Load Power
  jsonBar.insert("Sla", bar->Va.real());
  jsonBar.insert("Slai", bar->Va.imag());
  jsonBar.insert("Slb", bar->Va.real());
  jsonBar.insert("Slbi", bar->Va.imag());
  jsonBar.insert("Slc", bar->Va.real());
  jsonBar.insert("Slci", bar->Va.imag());

  // Result Voltage
  jsonBar.insert("rVa", bar->Va.real());
  jsonBar.insert("rVai", bar->Va.imag());
  jsonBar.insert("rVb", bar->Va.real());
  jsonBar.insert("rVbi", bar->Va.imag());
  jsonBar.insert("rVc", bar->Va.real());
  jsonBar.insert("rVci", bar->Va.imag());

  // Result Generated Power
  jsonBar.insert("rSga", bar->Va.real());
  jsonBar.insert("rSgai", bar->Va.imag());
  jsonBar.insert("rSgb", bar->Va.real());
  jsonBar.insert("rSgbi", bar->Va.imag());
  jsonBar.insert("rSgc", bar->Va.real());
  jsonBar.insert("rSgci", bar->Va.imag());

  // Result Load Power
  jsonBar.insert("rSla", bar->Va.real());
  jsonBar.insert("rSlai", bar->Va.imag());
  jsonBar.insert("rSlb", bar->Va.real());
  jsonBar.insert("rSlbi", bar->Va.imag());
  jsonBar.insert("rSlc", bar->Va.real());
  jsonBar.insert("rSlci", bar->Va.imag());

  // Position
  jsonBar.insert("x", bar->x());
  jsonBar.insert("y", bar->y());

  return jsonBar;
}


/*******************************************************************************
 * barFromJson.
 ******************************************************************************/
PnBar *Project::barFromJson(QJsonObject &jsonBar) {
  // Create Bar according to type
  PnBar *bar = new PnBar;

  // Get Id
  bar->id = static_cast<uint32_t> (jsonBar.value("id").toInt());

  // Voltage
  bar->Va.real(jsonBar.value("Va").toDouble());
  bar->Va.imag(jsonBar.value("Vai").toDouble());
  bar->Va.real(jsonBar.value("Vb").toDouble());
  bar->Va.imag(jsonBar.value("Vbi").toDouble());
  bar->Va.real(jsonBar.value("Vc").toDouble());
  bar->Va.imag(jsonBar.value("Vci").toDouble());

  // Generated Power
  bar->Va.real(jsonBar.value("Sga").toDouble());
  bar->Va.imag(jsonBar.value("Sgai").toDouble());
  bar->Va.real(jsonBar.value("Sgb").toDouble()) ;
  bar->Va.imag(jsonBar.value("Sgbi").toDouble());
  bar->Va.real(jsonBar.value("Sgc").toDouble());
  bar->Va.imag(jsonBar.value("Sgci").toDouble());

  // Load Power
  bar->Va.real(jsonBar.value("Sla").toDouble());
  bar->Va.imag(jsonBar.value("Slai").toDouble());
  bar->Va.real(jsonBar.value("Slb").toDouble());
  bar->Va.imag(jsonBar.value("Slbi").toDouble());
  bar->Va.real(jsonBar.value("Slc").toDouble());
  bar->Va.imag(jsonBar.value("Slci").toDouble());

  // Result Voltage
  bar->Va.real(jsonBar.value("rVa").toDouble());
  bar->Va.imag(jsonBar.value("rVai").toDouble());
  bar->Va.real(jsonBar.value("rVb").toDouble());
  bar->Va.imag(jsonBar.value("rVbi").toDouble());
  bar->Va.real(jsonBar.value("rVc").toDouble());
  bar->Va.imag(jsonBar.value("rVci").toDouble());

  // Result Generated Power
  bar->Va.real(jsonBar.value("rSga").toDouble());
  bar->Va.imag(jsonBar.value("rSgai").toDouble());
  bar->Va.real(jsonBar.value("rSgb").toDouble());
  bar->Va.imag(jsonBar.value("rSgbi").toDouble());
  bar->Va.real(jsonBar.value("rSgc").toDouble());
  bar->Va.imag(jsonBar.value("rSgci").toDouble());

  // Result Load Power
  bar->Va.real(jsonBar.value("rSla").toDouble());
  bar->Va.imag(jsonBar.value("rSlai").toDouble());
  bar->Va.real(jsonBar.value("rSlb").toDouble());
  bar->Va.imag(jsonBar.value("rSlbi").toDouble());
  bar->Va.real(jsonBar.value("rSlc").toDouble());
  bar->Va.imag(jsonBar.value("rSlci").toDouble());

  // Get Position
  bar->setX(jsonBar.value("x").toDouble());
  bar->setY(jsonBar.value("y").toDouble());

  return bar;
}

/*******************************************************************************
 * lineToJson.
 ******************************************************************************/
QJsonObject Project::lineToJson(PnLine *line) {
  // Transform line data to Json object.
  QJsonObject jsonLine;

  // Nodes
  jsonLine.insert("noI", static_cast<int>(line->noI()->id));
  jsonLine.insert("noF", static_cast<int>(line->noF()->id));

  // Impedance
  jsonLine.insert("Zaa", line->Zaa.real());
  jsonLine.insert("Zaai", line->Zaa.imag());
  jsonLine.insert("Zab", line->Zab.real());
  jsonLine.insert("Zabi", line->Zab.imag());
  jsonLine.insert("Zac", line->Zac.real());
  jsonLine.insert("Zaci", line->Zac.imag());
  jsonLine.insert("Zan", line->Zan.real());
  jsonLine.insert("Zani", line->Zan.imag());
  jsonLine.insert("Zbb", line->Zbb.real());
  jsonLine.insert("Zbbi", line->Zbb.imag());
  jsonLine.insert("Zbc", line->Zbc.real());
  jsonLine.insert("Zbci", line->Zbc.imag());
  jsonLine.insert("Zbn", line->Zbn.real());
  jsonLine.insert("Zbni", line->Zbn.imag());
  jsonLine.insert("Zcc", line->Zcc.real());
  jsonLine.insert("Zcci", line->Zcc.imag());
  jsonLine.insert("Zcn", line->Zcn.real());
  jsonLine.insert("Zcni", line->Zcn.imag());
  jsonLine.insert("Znn", line->Znn.real());
  jsonLine.insert("Znni", line->Znn.imag());

  // Current
  jsonLine.insert("Ia", line->Ia.real());
  jsonLine.insert("Iai", line->Ia.imag());
  jsonLine.insert("Ib", line->Ib.real());
  jsonLine.insert("Ibi", line->Ib.imag());
  jsonLine.insert("Ic", line->Ic.real());
  jsonLine.insert("Ici", line->Ic.imag());
  jsonLine.insert("In", line->In.real());
  jsonLine.insert("Ini", line->In.imag());

  return jsonLine;
}

/*******************************************************************************
 * lineFromJson.
 ******************************************************************************/
PnLine *Project::lineFromJson(QJsonObject &jsonLine) {
  PnLine *line = new PnLine;

  // Get NoI & NoF
  uint32_t noI = static_cast<uint32_t>(jsonLine.value("noI").toInt());
  uint32_t noF = static_cast<uint32_t>(jsonLine.value("noF").toInt());
  line->setNodes(pnNetwork->getBarById(noI), pnNetwork->getBarById(noF));

  // Impedance
  line->Zaa.real(jsonLine.value("Zaa").toDouble());
  line->Zaa.imag(jsonLine.value("Zaai").toDouble());
  line->Zab.real(jsonLine.value("Zab").toDouble());
  line->Zab.imag(jsonLine.value("Zabi").toDouble());
  line->Zac.real(jsonLine.value("Zac").toDouble());
  line->Zac.imag(jsonLine.value("Zaci").toDouble());
  line->Zan.real(jsonLine.value("Zan").toDouble());
  line->Zan.imag(jsonLine.value("Zani").toDouble());
  line->Zbb.real(jsonLine.value("Zbb").toDouble());
  line->Zbb.imag(jsonLine.value("Zbbi").toDouble());
  line->Zbc.real(jsonLine.value("Zbc").toDouble());
  line->Zbc.imag(jsonLine.value("Zbci").toDouble());
  line->Zbn.real(jsonLine.value("Zbn").toDouble());
  line->Zbn.imag(jsonLine.value("Zbni").toDouble());
  line->Zcc.real(jsonLine.value("Zcc").toDouble());
  line->Zcc.imag(jsonLine.value("Zcci").toDouble());
  line->Zcn.real(jsonLine.value("Zcn").toDouble());
  line->Zcn.imag(jsonLine.value("Zcni").toDouble());
  line->Znn.real(jsonLine.value("Znn").toDouble());
  line->Znn.imag(jsonLine.value("Znni").toDouble());

  // Current
  line->Ia.real(jsonLine.value("Ia").toDouble());
  line->Ia.imag(jsonLine.value("Iai").toDouble());
  line->Ib.real(jsonLine.value("Ib").toDouble());
  line->Ib.imag(jsonLine.value("Ibi").toDouble());
  line->Ic.real(jsonLine.value("Ic").toDouble());
  line->Ic.imag(jsonLine.value("Ici").toDouble());
  line->In.real(jsonLine.value("In").toDouble());
  line->In.imag(jsonLine.value("Ini").toDouble());

  return line;
}
