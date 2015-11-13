#include "algorithms/import.h"
#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include "models/network.h"
#include "algorithms/redrawnetwork.h"

static int32_t getNextLineInt(QTextStream *stream, QString keyWord,
                              int32_t *intVal)
{
  QString lineTxt; // Line text.
  int32_t lineCnt = 0; // Count the amount of processed lines.

  // Loop until keyWord is found or EoF.
  do {
    if(stream->atEnd())
      return -1;

    lineTxt = stream->readLine();
    lineCnt++;
  } while(!lineTxt.contains(keyWord));

  // Get integer value.
  *stream >> *intVal;
  return lineCnt;
}

bool importTxtType1(QString &fileName, Network *network)
{
  // Create project with file settings.
  QFile file(fileName);

  if(!file.open(QFile::ReadOnly)) {
    QMessageBox::critical(NULL, "File read error.",
                          "Can\'t read " + file.fileName() + ".",
                          QMessageBox::Ok);
    return false;
  }

  // Auxiliaty variables.
  QTextStream stream(&file); // Text stream used to parse lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  int32_t processedLines = 0; // Number of lines parsed during get methods.

  // Read Number of bars.
  int32_t numBar;
  processedLines = getNextLineInt(&stream, "NumBarras", &numBar);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide NumBarras.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Read Number of lines.
  int32_t numLine;
  processedLines = getNextLineInt(&stream, "NumLinhas", &numLine);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide NumLinhas.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Get Length Unit.
  int32_t LenUn;
  processedLines = getNextLineInt(&stream, "comprimento_das_linhas", &LenUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Length Unit.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Cast to Unit enum.
  Unit::LengthUnit lengthUnit;

  switch (LenUn) {
  case 1:
    lengthUnit = Unit::kMeter;
    break;

  case 2:
    lengthUnit = Unit::kKiloMeter;
    break;

  case 3:
    lengthUnit = Unit::kFeet;
    break;

  case 4:
    lengthUnit = Unit::kMile;
    break;

  default:
    lengthUnit = Unit::kMeter;
    break;
  }

  // Get Impedance Unit.
  int32_t ImpUn;
  processedLines = getNextLineInt(&stream, "impedancia", &ImpUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Impedance Unit.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

// Convert to enum.
  Unit::ImpedanceUnit impedanceUnit;

  switch (ImpUn) {
  case 1:
    impedanceUnit = Unit::kOhm;
    break;

  case 2:
    impedanceUnit = Unit::kOhmPerMeter;
    break;

  case 3:
    impedanceUnit = Unit::kOhmPerKilometer;
    break;

  case 4:
    impedanceUnit = Unit::kOhmPerFeet;
    break;

  case 5:
    impedanceUnit = Unit::kOhmPerMile;
    break;

  default:
    impedanceUnit = Unit::kOhm;
    break;
  }

  // Get power Unit.
  int32_t powerUn;
  processedLines = getNextLineInt(&stream, "Demanda", &powerUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Power Init.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Convert to enum.
  Unit::PowerUnit powerUnit;

  switch (powerUn) {
  case 1:
    powerUnit = Unit::kVA;
    break;

  case 2:
    powerUnit = Unit::kKiloVA;
    break;

  case 3:
    powerUnit = Unit::kMegaVa;
    break;

  default:
    powerUnit = Unit::kKiloVA;
    break;
  }

  // Get Bars.
  QString lineTxt;

  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide bars.",
                            QMessageBox::Ok);
      file.close();
      return false;
    }
  } while(!lineTxt.contains("[n"));

  int32_t id;
  int32_t barType;
  double Sia, Siai;
  double Sib, Sibi;
  double Sic, Sici;
  double Sha, Shai;
  double Shb, Shbi;
  double Shc, Shci;

  int32_t idBase = 0;

  Bar *bar;

  for(int32_t i=0; i<numBar; i++) {
    stream >> id >> barType >> Sia >> Siai >> Sib >> Sibi >> Sic >> Sici;
    stream >> Sha >> Shai >> Shb >> Shbi >> Shc >>Shci;

    if(barType == 3)
      idBase = id;

    bar = new Bar;
    bar->id = id - idBase;

    bar->setSh(0, complex<double>(Sha, Shai), powerUnit);
    bar->setSh(1, complex<double>(Shb, Shbi), powerUnit);
    bar->setSh(2, complex<double>(Shc, Shci), powerUnit);

    bar->setSi(0, complex<double>(Sia, Siai), powerUnit);
    bar->setSi(1, complex<double>(Sib, Sibi), powerUnit);
    bar->setSi(2, complex<double>(Sic, Sici), powerUnit);


    if (!network->addBar(bar)) {
      QMessageBox::critical(NULL, "Invalid Bar.",
                            "Invalid Bar " + QString::number(id) + "at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete bar;
      file.close();
      return false;
    }

    // Set Initial voltage to 1pu.
    bar->setV(0, 1, Unit::kVoltsPerUnit);
    bar->setV(1, 1, Unit::kVoltsPerUnit);
    bar->setV(2, 1, Unit::kVoltsPerUnit);

    lineTxt = stream.readLine();
    lineCnt++;
  }

  // Get Lines.
  do {
    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide lines.",
                            QMessageBox::Ok);
      file.close();
      return false;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  } while(!lineTxt.contains("Tipo_Cir"));

  int32_t noI, noF;
  double Zaa, Zaai;
  double Zab, Zabi;
  double Zac, Zaci;
  double Zbb, Zbbi;
  double Zbc, Zbci;
  double Zcc, Zcci;
  double length;
  QString dummy;

  Line *line;

  for(int32_t i=0; i<numLine; i++) {
    stream >> dummy >> noI >> noF >> dummy >> dummy >> dummy >> dummy >> Zaa >> Zaai
           >> Zbb >> Zbbi >> Zcc;
    stream >> Zcci >> dummy >> dummy >> Zab >> Zabi >> Zac >> Zaci >> dummy >>
           dummy;
    stream >> Zbc >> Zbci >> dummy >> dummy >> dummy >> dummy >> dummy >>   length;

    line = new Line;
    line->nodes.first = noI - idBase;
    line->nodes.second = noF - idBase;

    line->setLength(length, lengthUnit);

    line->setZ(0, complex<double>(Zaa, Zaai), impedanceUnit);
    line->setZ(1, complex<double>(Zab, Zabi), impedanceUnit);
    line->setZ(2, complex<double>(Zac, Zaci), impedanceUnit);
    line->setZ(3, complex<double>(Zbb, Zbbi), impedanceUnit);
    line->setZ(4, complex<double>(Zbc, Zbci), impedanceUnit);
    line->setZ(5, complex<double>(Zcc, Zcci), impedanceUnit);

    if (!network->addLine(line)) {
      QMessageBox::critical(NULL, "Invalid Line.",
                            "Invalid Line at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete line;
      file.close();
      return false;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

  RedrawNetwork redraw(network);
  redraw.sugiyamaFast();

  file.close();
  return true;
}

bool importTxtType2(QString &fileName, Network *network)
{
  QFile file(fileName);

  if(!file.open(QFile::ReadOnly)) {
    QMessageBox::critical(NULL, "File read error.",
                          "Can\'t read " + file.fileName() + ".",
                          QMessageBox::Ok);
    return false;
  }

  // Auxiliaty variables.
  QTextStream stream(&file); // Text stream used to parse lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  int32_t processedLines = 0; // Number of lines parsed during get methods.

  // Read Number of bars.
  int32_t numBar;
  processedLines = getNextLineInt(&stream, "Numero_barras", &numBar);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_barras.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Read Number of lines.
  int32_t numLine;
  processedLines = getNextLineInt(&stream, "Numero_linhas", &numLine);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_linhas.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Get Length Unit.
  int32_t LenUn;
  processedLines = getNextLineInt(&stream, "comprimento_das_linhas", &LenUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Length Unit.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Cast to Unit enum.
  Unit::LengthUnit lengthUnit;

  switch (LenUn) {
  case 1:
    lengthUnit = Unit::kMeter;
    break;

  case 2:
    lengthUnit = Unit::kKiloMeter;
    break;

  case 3:
    lengthUnit = Unit::kFeet;
    break;

  case 4:
    lengthUnit = Unit::kMile;
    break;

  default:
    lengthUnit = Unit::kMeter;
    break;
  }

  // Get Impedance Unit.
  int32_t ImpUn;
  processedLines = getNextLineInt(&stream, "impedancia", &ImpUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Impedance Unit.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Convert to enum.
  Unit::ImpedanceUnit impedanceUnit;

  switch (ImpUn) {
  case 1:
    impedanceUnit = Unit::kOhm;
    break;

  case 2:
    impedanceUnit = Unit::kOhmPerMeter;
    break;

  case 3:
    impedanceUnit = Unit::kOhmPerKilometer;
    break;

  case 4:
    impedanceUnit = Unit::kOhmPerFeet;
    break;

  case 5:
    impedanceUnit = Unit::kOhmPerMile;
    break;

  default:
    impedanceUnit = Unit::kOhm;
    break;
  }

  // Get power Unit.
  int32_t powerUn;
  processedLines = getNextLineInt(&stream, "Demanda", &powerUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Power Init.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Convert to enum.
  Unit::PowerUnit powerUnit;

  switch (powerUn) {
  case 1:
    powerUnit = Unit::kVA;
    break;

  case 2:
    powerUnit = Unit::kKiloVA;
    break;

  case 3:
    powerUnit = Unit::kMegaVa;
    break;

  default:
    powerUnit = Unit::kKiloVA;
    break;
  }

  // Get Bars.
  QString lineTxt;

  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide bars.",
                            QMessageBox::Ok);
      file.close();
      return false;
    }
  } while(!lineTxt.contains("Barra"));

  int32_t id;
  int32_t barType;
  double Sia, Siai;
  double Sib, Sibi;
  double Sic, Sici;
  double Sha, Shai;
  double Shb, Shbi;
  double Shc, Shci;

  int32_t idBase = 0;

  Bar *bar;

  for(int32_t i=0; i<numBar; i++) {
    stream >> id >> barType >> Sia >> Siai >> Sib >> Sibi >> Sic >> Sici;
    stream >> Sha >> Shai >> Shb >> Shbi >> Shc >>Shci;

    if(barType == 3)
      idBase = id;

    bar = new Bar;
    bar->id = id - idBase;

    bar->setSh(0, complex<double>(Sha, Shai), powerUnit);
    bar->setSh(1, complex<double>(Shb, Shbi), powerUnit);
    bar->setSh(2, complex<double>(Shc, Shci), powerUnit);

    bar->setSi(0, complex<double>(Sia, Siai), powerUnit);
    bar->setSi(1, complex<double>(Sib, Sibi), powerUnit);
    bar->setSi(2, complex<double>(Sic, Sici), powerUnit);

    if (!network->addBar(bar)) {
      QMessageBox::critical(NULL, "Invalid Bar.",
                            "Invalid Bar " + QString::number(id) + "at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete bar;
      file.close();
      return false;
    }

    // Set Initial voltage to 1pu.
    bar->setV(0, 1, Unit::kVoltsPerUnit);
    bar->setV(1, 1, Unit::kVoltsPerUnit);
    bar->setV(2, 1, Unit::kVoltsPerUnit);

    lineTxt = stream.readLine();
    lineCnt++;
  }

  // Get Lines.
  do {
    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide lines.",
                            QMessageBox::Ok);
      file.close();
      return false;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  } while(!lineTxt.contains("Tipo_Cir"));

  int32_t noI, noF;
  double Zaa, Zaai;
  double Zab, Zabi;
  double Zac, Zaci;
  double Zbb, Zbbi;
  double Zbc, Zbci;
  double Zcc, Zcci;
  double length;
  QString lineType;
  QString dummy;

  Line *line;

  for(int32_t i=0; i<numLine; i++) {
    line = new Line;
    stream >> lineType >> noI >> noF;
    line->nodes.first = noI - idBase;
    line->nodes.second = noF - idBase;

    if (lineType.contains("CP")) {
      stream >> Zaa >> Zaai >> Zbb >> Zbbi >> Zcc;
      stream >> Zcci >> dummy >> dummy >> Zab >> Zabi >> Zac >> Zaci >> dummy;
      stream >> dummy >> Zbc >> Zbci >> dummy >> dummy >> dummy >> dummy;
      stream >> dummy >> length;

      line->setLength(length, lengthUnit);

      line->setZ(0, complex<double>(Zaa, Zaai), impedanceUnit);
      line->setZ(1, complex<double>(Zab, Zabi), impedanceUnit);
      line->setZ(2, complex<double>(Zac, Zaci), impedanceUnit);
      line->setZ(3, complex<double>(Zbb, Zbbi), impedanceUnit);
      line->setZ(4, complex<double>(Zbc, Zbci), impedanceUnit);
      line->setZ(5, complex<double>(Zcc, Zcci), impedanceUnit);
    }

    if (!network->addLine(line)) {
      QMessageBox::critical(NULL, "Invalid Line.",
                            "Invalid Line at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete line;
      file.close();
      return false;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

  RedrawNetwork redraw(network);
  redraw.sugiyamaFast();

  file.close();
  return true;
}

bool importTxtType3(QString &fileName, Network *network)
{
  QFile file(fileName);

  if(!file.open(QFile::ReadOnly)) {
    QMessageBox::critical(NULL, "File read error.",
                          "Can\'t read " + file.fileName() + ".",
                          QMessageBox::Ok);
    return false;
  }

  // Auxiliaty variables.
  QTextStream stream(&file); // Text stream used to parse lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  int32_t processedLines = 0; // Number of lines parsed during get methods.

  // Read Number of bars.
  int32_t numBar;
  processedLines = getNextLineInt(&stream, "Numero_barras", &numBar);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_barras.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Read Number of lines.
  int32_t numLine;
  processedLines = getNextLineInt(&stream, "Numero_linhas", &numLine);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_linhas.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Get Length Unit.
  int32_t LenUn;
  processedLines = getNextLineInt(&stream, "comprimento_das_linhas", &LenUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Length Unit.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Cast to Unit enum.
  Unit::LengthUnit lengthUnit;

  switch (LenUn) {
  case 1:
    lengthUnit = Unit::kMeter;
    break;

  case 2:
    lengthUnit = Unit::kKiloMeter;
    break;

  case 3:
    lengthUnit = Unit::kFeet;
    break;

  case 4:
    lengthUnit = Unit::kMile;
    break;

  default:
    lengthUnit = Unit::kMeter;
    break;
  }

  // Get Impedance Unit.
  int32_t ImpUn;
  processedLines = getNextLineInt(&stream, "impedancia", &ImpUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Impedance Unit.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Convert to enum.
  Unit::ImpedanceUnit impedanceUnit;

  switch (ImpUn) {
  case 1:
    impedanceUnit = Unit::kOhm;
    break;

  case 2:
    impedanceUnit = Unit::kOhmPerMeter;
    break;

  case 3:
    impedanceUnit = Unit::kOhmPerKilometer;
    break;

  case 4:
    impedanceUnit = Unit::kOhmPerFeet;
    break;

  case 5:
    impedanceUnit = Unit::kOhmPerMile;
    break;

  default:
    impedanceUnit = Unit::kOhm;
    break;
  }

  // Get power Unit.
  int32_t powerUn;
  processedLines = getNextLineInt(&stream, "Demanda", &powerUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Power Init.",
                          QMessageBox::Ok);
    file.close();
    return false;
  }

  lineCnt += processedLines;

  // Convert to enum.
  Unit::PowerUnit powerUnit;

  switch (powerUn) {
  case 1:
    powerUnit = Unit::kVA;
    break;

  case 2:
    powerUnit = Unit::kKiloVA;
    break;

  case 3:
    powerUnit = Unit::kMegaVa;
    break;

  default:
    powerUnit = Unit::kKiloVA;
    break;
  }

  // Get Bars.
  QString lineTxt;

  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide bars.",
                            QMessageBox::Ok);
      file.close();
      return false;
    }
  } while(!lineTxt.contains("Barra"));

  int32_t id;
  int32_t barType;
  double Sia, Siai;
  double Sib, Sibi;
  double Sic, Sici;
  double Sha, Shai;
  double Shb, Shbi;
  double Shc, Shci;

  int32_t idBase = 0;

  Bar *bar;

  for(int32_t i=0; i<numBar; i++) {
    stream >> id >> barType >> Sia >> Siai >> Sib >> Sibi >> Sic >> Sici;
    stream >> Sha >> Shai >> Shb >> Shbi >> Shc >>Shci;

    if(barType == 3)
      idBase = id;

    bar = new Bar;
    bar->id = id - idBase;

    bar->setSh(0, complex<double>(Sha, Shai), powerUnit);
    bar->setSh(1, complex<double>(Shb, Shbi), powerUnit);
    bar->setSh(2, complex<double>(Shc, Shci), powerUnit);

    bar->setSi(0, complex<double>(Sia, Siai), powerUnit);
    bar->setSi(1, complex<double>(Sib, Sibi), powerUnit);
    bar->setSi(2, complex<double>(Sic, Sici), powerUnit);


    if (!network->addBar(bar)) {
      QMessageBox::critical(NULL, "Invalid Bar.",
                            "Invalid Bar " + QString::number(id) + "at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete bar;
      file.close();
      return false;
    }

    // Set Initial voltage to 1pu.
    bar->setV(0, 1, Unit::kVoltsPerUnit);
    bar->setV(1, 1, Unit::kVoltsPerUnit);
    bar->setV(2, 1, Unit::kVoltsPerUnit);

    lineTxt = stream.readLine();
    lineCnt++;
  }

  // Get Lines.
  do {
    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide lines.",
                            QMessageBox::Ok);
      file.close();
      return false;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  } while(!lineTxt.contains("Tipo_Cir"));

  int32_t noI, noF;
  double Zaa, Zaai;
  double Zab, Zabi;
  double Zac, Zaci;
  double Zbb, Zbbi;
  double Zbc, Zbci;
  double Zcc, Zcci;
  double length;
  QString lineType;
  QString dummy;

  Line *line;

  for(int32_t i=0; i<numLine; i++) {
    line = new Line;
    stream >> lineType >> noI >> noF;
    line->nodes.first = noI - idBase;
    line->nodes.second = noF - idBase;

    if (lineType.contains("CP")) {
      stream >> dummy >> dummy >> dummy >> dummy >> Zaa >> Zaai >> Zbb >> Zbbi;
      stream >> Zcc >> Zcci >> dummy >> dummy >> Zab >> Zabi >> Zac >> Zaci;
      stream >> dummy >> dummy >> Zbc >> Zbci >> dummy >> dummy >> dummy;
      stream >> dummy >> dummy >> length;

      line->setLength(length, lengthUnit);

      line->setZ(0, complex<double>(Zaa, Zaai), impedanceUnit);
      line->setZ(1, complex<double>(Zab, Zabi), impedanceUnit);
      line->setZ(2, complex<double>(Zac, Zaci), impedanceUnit);
      line->setZ(3, complex<double>(Zbb, Zbbi), impedanceUnit);
      line->setZ(4, complex<double>(Zbc, Zbci), impedanceUnit);
      line->setZ(5, complex<double>(Zcc, Zcci), impedanceUnit);
    }

    if (!network->addLine(line)) {
      QMessageBox::critical(NULL, "Invalid Line.",
                            "Invalid Line at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete line;
      file.close();
      return false;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

  RedrawNetwork redraw(network);
  redraw.sugiyamaFast();

  file.close();
  return true;
}
