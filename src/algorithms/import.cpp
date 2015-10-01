#include "algorithms/import.h"
#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include "graphics/network.h"
#include "algorithms/redraw.h"

static Project *preprocessProject(QFile &file)
{
  // Open File.
  file.fileName();

  if(!file.open(QFile::ReadOnly)) {
    QMessageBox::critical(NULL, "File read error.",
                          "Can\'t read " + file.fileName() + ".",
                          QMessageBox::Ok);
    return NULL;
  }

  // Create new project.
  Project *project = new Project;

  // Set project name.
  int32_t nameInit = file.fileName().lastIndexOf(QDir::separator());
  int32_t nameFinal = file.fileName().lastIndexOf('.');
  project->name = file.fileName().mid(nameInit+1, nameFinal- nameInit -1);

  // Set project path.
  project->filePath = file.fileName().left(nameFinal) + ".qkf";

  return project;
}

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

static int32_t getNextLineDouble(QTextStream *stream, QString keyWord,
                                 double *doubleVal)
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

  // Get double value.
  *stream >> *doubleVal;
  return lineCnt;
}

Project *importTxtType1(QString &fileName)
{
// Create project with file settings.
//------------------------------------------------------------------------------
  QFile file(fileName);

  Project *project = preprocessProject(file);

  if (project == NULL)
    return NULL;

  // Auxiliaty variables.
  QTextStream stream(&file); // Text stream used to parse lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  int32_t processedLines = 0; // Number of lines parsed during get methods.

// Read Number of bars.
//------------------------------------------------------------------------------
  int32_t numBar;
  processedLines = getNextLineInt(&stream, "NumBarras", &numBar);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide NumBarras.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Read Number of lines.
//------------------------------------------------------------------------------
  int32_t numLine;
  processedLines = getNextLineInt(&stream, "NumLinhas", &numLine);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide NumLinhas.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Get Base Voltage.
//------------------------------------------------------------------------------
  double VBase;
  processedLines = getNextLineDouble(&stream, "Voltagem_base_da_rede", &VBase);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName + " doesn't provide VBase.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  project->setVoltageBase(VBase);

// Get Length Unit.
//------------------------------------------------------------------------------
  int32_t LenUn;
  processedLines = getNextLineInt(&stream, "comprimento_das_linhas", &LenUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Length Unit.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Cast to Unit enum.
  switch (LenUn) {
  case 1:
    project->setLengthUn(Unit::kMeter);
    break;

  case 2:
    project->setLengthUn(Unit::kKiloMeter);
    break;

  case 3:
    project->setLengthUn(Unit::kFeet);
    break;

  case 4:
    project->setLengthUn(Unit::kMile);
    break;

  default:
    project->setLengthUn(Unit::kMeter);
    break;
  }

// Get Impedance Unit.
//------------------------------------------------------------------------------
  int32_t ImpUn;
  processedLines = getNextLineInt(&stream, "impedancia", &ImpUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Impedance Unit.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  switch (ImpUn) {
  case 1:
    project->setImpedanceUn(Unit::kOhm);
    break;

  case 2:
    project->setImpedanceUn(Unit::kOhmPerMeter);
    break;

  case 3:
    project->setImpedanceUn(Unit::kOhmPerKilometer);
    break;

  case 4:
    project->setImpedanceUn(Unit::kOhmPerFeet);
    break;

  case 5:
    project->setImpedanceUn(Unit::kOhmPerMile);
    break;

  default:
    project->setImpedanceUn(Unit::kOhm);
    break;
  }

// Get power Unit.
//------------------------------------------------------------------------------
  int32_t powerUn;
  processedLines = getNextLineInt(&stream, "Demanda", &powerUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Power Init.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  switch (powerUn) {
  case 1:
    project->setPowerUn(Unit::kVA);
    break;

  case 2:
    project->setPowerUn(Unit::kKiloVA);
    break;

  case 3:
    project->setPowerUn(Unit::kMegaVa);
    break;

  default:
    project->setPowerUn(Unit::kKiloVA);
    break;
  }

// Get Voltage Unit.
//------------------------------------------------------------------------------
  project->setVoltageUn(Unit::kKiloVolts);

// Get Base Power.
//------------------------------------------------------------------------------
  double PBase;
  processedLines = getNextLineDouble(&stream, "kVA    kV_hihg", &PBase);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName + " doesn't provide SBase.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;
  project->setPowerBase(PBase);

// Get Bars.
//------------------------------------------------------------------------------
  QString lineTxt;

  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide bars.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
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

    bar->Sha.real(Sha);
    bar->Sha.imag(Shai);
    bar->Shb.real(Shb);
    bar->Shb.imag(Shbi);
    bar->Shc.real(Shc);
    bar->Shc.imag(Shci);
    bar->Sia.real(Sia);
    bar->Sia.imag(Siai);
    bar->Sib.real(Sib);
    bar->Sib.imag(Sibi);
    bar->Sic.real(Sic);
    bar->Sic.imag(Sici);

    if (!project->network->addBar(bar)) {
      QMessageBox::critical(NULL, "Invalid Bar.",
                            "Invalid Bar " + QString::number(id) + "at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete bar;
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

//// Get Lines.
////------------------------------------------------------------------------------
  do {
    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide lines.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
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
    line->noI = noI - idBase;
    line->noF = noF - idBase;

    line->length = length;

    line->Zaa.real(Zaa);
    line->Zaa.imag(Zaai);
    line->Zab.real(Zab);
    line->Zab.imag(Zabi);
    line->Zac.real(Zac);
    line->Zac.imag(Zaci);
    line->Zbb.real(Zbb);
    line->Zbb.imag(Zbbi);
    line->Zbc.real(Zbc);
    line->Zbc.imag(Zbci);
    line->Zcc.real(Zcc);
    line->Zcc.imag(Zcci);

    if (!project->network->addLine(line)) {
      QMessageBox::critical(NULL, "Invalid Line.",
                            "Invalid Line at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete line;
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

  redrawGraph1(project->network);

  file.close();
  return project;
}

Project *importTxtType2(QString &fileName)
{
// Create project with file settings.
//------------------------------------------------------------------------------
  QFile file(fileName);

  Project *project = preprocessProject(file);

  if (project == NULL)
    return NULL;

  // Auxiliaty variables.
  QTextStream stream(&file); // Text stream used to parse lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  int32_t processedLines = 0; // Number of lines parsed during get methods.

// Read Number of bars.
//------------------------------------------------------------------------------
  int32_t numBar;
  processedLines = getNextLineInt(&stream, "Numero_barras", &numBar);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_barras.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Read Number of lines.
//------------------------------------------------------------------------------
  int32_t numLine;
  processedLines = getNextLineInt(&stream, "Numero_linhas", &numLine);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_linhas.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Get Base Voltage.
//------------------------------------------------------------------------------
  double VBase;
  processedLines = getNextLineDouble(&stream, "Voltagem_base", &VBase);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName + " doesn't provide VBase.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  project->setVoltageBase(VBase);

// Get Length Unit.
//------------------------------------------------------------------------------
  int32_t LenUn;
  processedLines = getNextLineInt(&stream, "comprimento_das_linhas", &LenUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Length Unit.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Cast to Unit enum.
  switch (LenUn) {
  case 1:
    project->setLengthUn(Unit::kMeter);
    break;

  case 2:
    project->setLengthUn(Unit::kKiloMeter);
    break;

  case 3:
    project->setLengthUn(Unit::kFeet);
    break;

  case 4:
    project->setLengthUn(Unit::kMile);
    break;

  default:
    project->setLengthUn(Unit::kMeter);
    break;
  }

// Get Impedance Unit.
//------------------------------------------------------------------------------
  int32_t ImpUn;
  processedLines = getNextLineInt(&stream, "impedancia", &ImpUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Impedance Unit.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  switch (ImpUn) {
  case 1:
    project->setImpedanceUn(Unit::kOhm);
    break;

  case 2:
    project->setImpedanceUn(Unit::kOhmPerMeter);
    break;

  case 3:
    project->setImpedanceUn(Unit::kOhmPerKilometer);
    break;

  case 4:
    project->setImpedanceUn(Unit::kOhmPerFeet);
    break;

  case 5:
    project->setImpedanceUn(Unit::kOhmPerMile);
    break;

  default:
    project->setImpedanceUn(Unit::kOhm);
    break;
  }

// Get power Unit.
//------------------------------------------------------------------------------
  int32_t powerUn;
  processedLines = getNextLineInt(&stream, "Demanda", &powerUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Power Init.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  switch (powerUn) {
  case 1:
    project->setPowerUn(Unit::kVA);
    break;

  case 2:
    project->setPowerUn(Unit::kKiloVA);
    break;

  case 3:
    project->setPowerUn(Unit::kMegaVa);
    break;

  default:
    project->setPowerUn(Unit::kKiloVA);
    break;
  }

// Get Voltage Unit.
//------------------------------------------------------------------------------
  project->setVoltageUn(Unit::kKiloVolts);

// Get Base Power.
//------------------------------------------------------------------------------
  double PBase;
  processedLines = getNextLineDouble(&stream, "kVA    kV_hihg", &PBase);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName + " doesn't provide SBase.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;
  project->setPowerBase(PBase);

// Get Bars.
//------------------------------------------------------------------------------
  QString lineTxt;

  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide bars.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
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

    bar->Sha.real(Sha);
    bar->Sha.imag(Shai);
    bar->Shb.real(Shb);
    bar->Shb.imag(Shbi);
    bar->Shc.real(Shc);
    bar->Shc.imag(Shci);
    bar->Sia.real(Sia);
    bar->Sia.imag(Siai);
    bar->Sib.real(Sib);
    bar->Sib.imag(Sibi);
    bar->Sic.real(Sic);
    bar->Sic.imag(Sici);

    if (!project->network->addBar(bar)) {
      QMessageBox::critical(NULL, "Invalid Bar.",
                            "Invalid Bar " + QString::number(id) + "at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete bar;
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

//// Get Lines.
////------------------------------------------------------------------------------
  do {
    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide lines.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  } while(!lineTxt.contains("Tipo_Cir"));

  int32_t noI, noF;
  double Zaa, Zaai;
  double Zab, Zabi;
  double Zac, Zaci;
  double Zan, Zani;
  double Zbb, Zbbi;
  double Zbc, Zbci;
  double Zbn, Zbni;
  double Zcc, Zcci;
  double Zcn, Zcni;
  double Znn, Znni;
  double length;

  Line *line;
  QString lineType;
  QString dummy;

  for(int32_t i=0; i<numLine; i++) {
    line = new Line;
    stream >> lineType >> noI >> noF;

    line->noI = noI - idBase;
    line->noF = noF - idBase;

    if (lineType.contains("CP")) {
      stream >> Zaa >> Zaai >> Zbb >> Zbbi >> Zcc;
      stream >> Zcci >> Znn >> Znni >> Zab >> Zabi >> Zac >> Zaci >> Zan >> Zani;
      stream >> Zbc >> Zbci >> Zbn >> Zbni >> Zcn >> Zcni >> dummy >> length;
      line->length = length;
      line->Zaa.real(Zaa);
      line->Zaa.imag(Zaai);
      line->Zab.real(Zab);
      line->Zab.imag(Zabi);
      line->Zac.real(Zac);
      line->Zac.imag(Zaci);
      line->Zbb.real(Zbb);
      line->Zbb.imag(Zbbi);
      line->Zbc.real(Zbc);
      line->Zbc.imag(Zbci);
      line->Zcc.real(Zcc);
      line->Zcc.imag(Zcci);
    }

    if (!project->network->addLine(line)) {
      QMessageBox::critical(NULL, "Invalid Line.",
                            "Invalid Line at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete line;
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

  redrawGraph1(project->network);

  file.close();
  return project;
}

Project *importTxtType3(QString &fileName)
{
// Create project with file settings.
//------------------------------------------------------------------------------
  QFile file(fileName);

  Project *project = preprocessProject(file);

  if (project == NULL)
    return NULL;

  // Auxiliaty variables.
  QTextStream stream(&file); // Text stream used to parse lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  int32_t processedLines = 0; // Number of lines parsed during get methods.

// Read Number of bars.
//------------------------------------------------------------------------------
  int32_t numBar;
  processedLines = getNextLineInt(&stream, "Numero_barras", &numBar);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_barras.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Read Number of lines.
//------------------------------------------------------------------------------
  int32_t numLine;
  processedLines = getNextLineInt(&stream, "Numero_linhas", &numLine);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Numero_linhas.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Get Base Voltage.
//------------------------------------------------------------------------------
  double VBase;
  processedLines = getNextLineDouble(&stream, "Voltagem_base", &VBase);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName + " doesn't provide VBase.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  project->setVoltageBase(VBase);

// Get Length Unit.
//------------------------------------------------------------------------------
  int32_t LenUn;
  processedLines = getNextLineInt(&stream, "comprimento_das_linhas", &LenUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Length Unit.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Cast to Unit enum.
  switch (LenUn) {
  case 1:
    project->setLengthUn(Unit::kMeter);
    break;

  case 2:
    project->setLengthUn(Unit::kKiloMeter);
    break;

  case 3:
    project->setLengthUn(Unit::kFeet);
    break;

  case 4:
    project->setLengthUn(Unit::kMile);
    break;

  default:
    project->setLengthUn(Unit::kMeter);
    break;
  }

// Get Impedance Unit.
//------------------------------------------------------------------------------
  int32_t ImpUn;
  processedLines = getNextLineInt(&stream, "impedancia", &ImpUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Impedance Unit.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  switch (ImpUn) {
  case 1:
    project->setImpedanceUn(Unit::kOhm);
    break;

  case 2:
    project->setImpedanceUn(Unit::kOhmPerMeter);
    break;

  case 3:
    project->setImpedanceUn(Unit::kOhmPerKilometer);
    break;

  case 4:
    project->setImpedanceUn(Unit::kOhmPerFeet);
    break;

  case 5:
    project->setImpedanceUn(Unit::kOhmPerMile);
    break;

  default:
    project->setImpedanceUn(Unit::kOhm);
    break;
  }

// Get power Unit.
//------------------------------------------------------------------------------
  int32_t powerUn;
  processedLines = getNextLineInt(&stream, "Demanda", &powerUn);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName +
                          " doesn't provide Power Init.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;

// Save to project.
  switch (powerUn) {
  case 1:
    project->setPowerUn(Unit::kVA);
    break;

  case 2:
    project->setPowerUn(Unit::kKiloVA);
    break;

  case 3:
    project->setPowerUn(Unit::kMegaVa);
    break;

  default:
    project->setPowerUn(Unit::kKiloVA);
    break;
  }

// Get Voltage Unit.
//------------------------------------------------------------------------------
  project->setVoltageUn(Unit::kKiloVolts);

// Get Base Power.
//------------------------------------------------------------------------------
  double PBase;
  processedLines = getNextLineDouble(&stream, "kVA    kV_hihg", &PBase);

  if(processedLines == -1) {
    QMessageBox::critical(NULL, "Invalid File.",
                          "The file " + fileName + " doesn't provide SBase.",
                          QMessageBox::Ok);
    delete project;
    return NULL;
  }

  lineCnt += processedLines;
  project->setPowerBase(PBase);

// Get Bars.
//------------------------------------------------------------------------------
  QString lineTxt;

  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide bars.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
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

    bar->Sha.real(Sha);
    bar->Sha.imag(Shai);
    bar->Shb.real(Shb);
    bar->Shb.imag(Shbi);
    bar->Shc.real(Shc);
    bar->Shc.imag(Shci);
    bar->Sia.real(Sia);
    bar->Sia.imag(Siai);
    bar->Sib.real(Sib);
    bar->Sib.imag(Sibi);
    bar->Sic.real(Sic);
    bar->Sic.imag(Sici);

    if (!project->network->addBar(bar)) {
      QMessageBox::critical(NULL, "Invalid Bar.",
                            "Invalid Bar " + QString::number(id) + "at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete bar;
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

//// Get Lines.
////------------------------------------------------------------------------------
  do {
    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide lines.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  } while(!lineTxt.contains("Tipo_Cir"));

  int32_t noI, noF;
  double Zaa, Zaai;
  double Zab, Zabi;
  double Zac, Zaci;
  double Zan, Zani;
  double Zbb, Zbbi;
  double Zbc, Zbci;
  double Zbn, Zbni;
  double Zcc, Zcci;
  double Zcn, Zcni;
  double Znn, Znni;
  double length;

  Line *line;
  QString lineType;
  QString dummy;

  for(int32_t i=0; i<numLine; i++) {
    line = new Line;
    stream >> lineType >> noI >> noF;

    line->noI = noI - idBase;
    line->noF = noF - idBase;

    if (lineType.contains("CP")) {
      stream >> dummy >> dummy >> dummy >> dummy >> Zaa >> Zaai >> Zbb >> Zbbi >> Zcc;
      stream >> Zcci >> Znn >> Znni >> Zab >> Zabi >> Zac >> Zaci >> Zan >> Zani;
      stream >> Zbc >> Zbci >> Zbn >> Zbni >> Zcn >> Zcni >> dummy >> length;
      line->length = length;
      line->Zaa.real(Zaa);
      line->Zaa.imag(Zaai);
      line->Zab.real(Zab);
      line->Zab.imag(Zabi);
      line->Zac.real(Zac);
      line->Zac.imag(Zaci);
      line->Zbb.real(Zbb);
      line->Zbb.imag(Zbbi);
      line->Zbc.real(Zbc);
      line->Zbc.imag(Zbci);
      line->Zcc.real(Zcc);
      line->Zcc.imag(Zcci);
    }

    if (!project->network->addLine(line)) {
      QMessageBox::critical(NULL, "Invalid Line.",
                            "Invalid Line at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete line;
      delete project;
      return NULL;
    }

    lineTxt = stream.readLine();
    lineCnt++;
  }

  redrawGraph1(project->network);

  file.close();
  return project;
}
