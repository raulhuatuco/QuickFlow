#include "projectsettings.h"

#include <QFile>
#include <QJsonDocument>

#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pncable.h"

ProjectSettings::ProjectSettings() : name_("") { pnNetwork_ = new PnNetwork(); }

ProjectSettings::~ProjectSettings() { delete pnNetwork_; }

PnNetwork *ProjectSettings::pnNetwork() { return pnNetwork_; }

QString ProjectSettings::name() { return name_; }

void ProjectSettings::setName(QString name) { name_ = name; }

bool ProjectSettings::save(QString fileName) {
  // Try to open file
  QFile file;
  file.setFileName(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

  // Base Json object
  QJsonObject projectJson;

  // Add project settings
  projectJson.insert("name", name_);

  // Add Bars Array

  // Add Lines Array

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

  // Extract Lines

  // Close file
  file.close();
  return true;
}

QJsonObject ProjectSettings::barToJson(PnBar *bar) {
  // Save bar data
  QJsonObject jsonBar;

  jsonBar.insert("id", static_cast<int>(bar->Id()));
  jsonBar.insert("vreal", QString::number(bar->inputV().real()));
  jsonBar.insert("vimag", QString::number(bar->inputV().imag()));
  jsonBar.insert("sgreal", QString::number(bar->inputSg().real()));
  jsonBar.insert("sgimag", QString::number(bar->inputSg().imag()));
  jsonBar.insert("slreal", QString::number(bar->inputSl().real()));
  jsonBar.insert("slimag", QString::number(bar->inputSl().imag()));
  jsonBar.insert("type", bar->barType());

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
    bar = new PnSlack;
  } else if (type == "PQ") {
    bar = new PnPq;
  } else if (type == "PV") {
    bar = new PnPv;
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
  bar->setInputV(std::complex<double>(sgreal, sgimag));

  // Get Sl
  double slreal, slimag;
  slreal = jsonBar.value("slreal").toDouble();
  slimag = jsonBar.value("slimag").toDouble();
  bar->setInputV(std::complex<double>(slreal, slimag));

  return bar;
}
