#include "projectsettings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pncable.h"

ProjectSettings::ProjectSettings() : name_("") {
  pnNetwork_ = new PnNetwork();
}

ProjectSettings::~ProjectSettings() {
  delete pnNetwork_;
}

PnNetwork *ProjectSettings::pnNetwork() {
  return pnNetwork_;
}

QString ProjectSettings::name() {
  return name_;
}

void ProjectSettings::setName(QString name) {
  name_ = name;
}

bool ProjectSettings::save(QString fileName) {
  // Try to open file
  QFile file;
  file.setFileName(fileName);

  if (!file.open(QIODevice::WriteOnly)) return false;

  // Base Json object
  QJsonObject projectJson;

  // Add project settings
  projectJson.insert("name", name_);

  // Add Bars Array
  QJsonArray barArray;

  foreach (PnBar *bar, pnNetwork_->barMap) {
    barArray << barToJson(bar);
  }

  projectJson.insert("barArray", barArray);

  // Add Lines Array
  QJsonArray lineArray;

  foreach (PnLine *line, pnNetwork_->lineMap) {
    lineArray << lineToJson(line);
  }

  projectJson.insert("lineArray", lineArray);

  // Create a Json object to conver Json data to text
  QJsonDocument jsonDoc(projectJson);

  // Write to file
  file.write(jsonDoc.toJson());
  file.flush();
  file.close();

  return true;
}

bool ProjectSettings::load(QString fileName) {
  // Try to open file
  QFile file;
  file.setFileName(fileName);

  if (!file.open(QIODevice::ReadOnly)) return false;

  // Create Doc to transform text to json
  QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());

  if (jsonDoc.isNull() || !jsonDoc.isObject()) return false;

  // Fill base Json object
  QJsonObject projectJson;
  projectJson = jsonDoc.object();

  // Extract name
  name_ = projectJson.value("name").toString();

  // Extract Bars
  QJsonArray barArray(projectJson.value("barArray").toArray());

  foreach (QJsonValue arrayValue, barArray) {
    QJsonObject barJson(arrayValue.toObject());

    if(barJson.value("type").toString() == "Slack") {
      PnSlack *slack = dynamic_cast<PnSlack *> (barFromJson(barJson));
      pnNetwork_->addSlack(slack);
    }

    if(barJson.value("type").toString() == "PQ") {
      PnPq *pq = dynamic_cast<PnPq *> (barFromJson(barJson));
      pnNetwork_->addPq(pq);
    }

    if(barJson.value("type").toString() == "PV") {
      PnPv *pv = dynamic_cast<PnPv *> (barFromJson(barJson));
      pnNetwork_->addPv(pv);
    }
  }

  // Extract Lines
  QJsonArray lineArray(projectJson.value("lineArray").toArray());

  foreach (QJsonValue arrayValue, lineArray) {
    QJsonObject lineJson(arrayValue.toObject());

    if(lineJson.value("type").toString() == "Cable") {
      PnCable *cable = dynamic_cast<PnCable *> (lineFromJson(lineJson));
      pnNetwork_->addCable(cable);
    }
  }

  // Close file
  file.close();
  return true;
}

QJsonObject ProjectSettings::barToJson(PnBar *bar) {
  // Save bar data
  QJsonObject jsonBar;

  jsonBar.insert("id", static_cast<int>(bar->Id()));
  jsonBar.insert("vreal", bar->inputV().real());
  jsonBar.insert("vimag", bar->inputV().imag());
  jsonBar.insert("sgreal", bar->inputSg().real());
  jsonBar.insert("sgimag", bar->inputSg().imag());
  jsonBar.insert("slreal", bar->inputSl().real());
  jsonBar.insert("slimag", bar->inputSl().imag());
  jsonBar.insert("type", bar->barType());
  jsonBar.insert("x", bar->x());
  jsonBar.insert("y", bar->y());

  if (bar->barType() == "Slack") {
    PnSlack *slack = dynamic_cast<PnSlack *> (bar);
    jsonBar.insert("maxGen", slack->maxGeneration());
  } else if (bar->barType() == "PV") {
    PnPv *pv = dynamic_cast<PnPv *> (bar);
    jsonBar.insert("maxQGen", pv->maxQGenerated());
    jsonBar.insert("minQGen", pv->minQGenerated());
  }

  return jsonBar;
}

// Load bar data
PnBar *ProjectSettings::barFromJson(QJsonObject &jsonBar) {
  // Check for type
  QString type;
  type = jsonBar.value("type").toString();

  // Create Bar according to type
  PnBar *bar;

  if (type == "Slack") {
    PnSlack *slack = new PnSlack;
    slack->setMaxGeneration(jsonBar.value("maxGen").toDouble());
    bar = slack;
  } else if (type == "PQ") {
    bar = new PnPq;
  } else if (type == "PV") {
    PnPv *pv = new PnPv;
    pv->setMaxQGenerated(jsonBar.value("maxQGen").toDouble());
    pv->setMinQGenerated(jsonBar.value("minQGen").toDouble());
    bar = pv;
  }

  // Get Id
  bar->setId(static_cast<unsigned int>(jsonBar.value("id").toInt()));

  // Get V
  double vreal, vimag;
  vreal = jsonBar.value("vreal").toDouble();
  vimag = jsonBar.value("vimag").toDouble();
  bar->setInputV(std::complex<double>(vreal, vimag));

  // Get Sg
  double sgreal, sgimag;
  sgreal = jsonBar.value("sgreal").toDouble();
  sgimag = jsonBar.value("sgimag").toDouble();
  bar->setInputSg(std::complex<double>(sgreal, sgimag));

  // Get Sl
  double slreal, slimag;
  slreal = jsonBar.value("slreal").toDouble();
  slimag = jsonBar.value("slimag").toDouble();
  bar->setInputSl(std::complex<double>(slreal, slimag));

// Get Position
  bar->setX(jsonBar.value("x").toDouble());
  bar->setY(jsonBar.value("y").toDouble());

  return bar;
}

QJsonObject ProjectSettings::lineToJson(PnLine *line) {

}

PnLine *ProjectSettings::lineFromJson(QJsonObject &jsonline) {

}
