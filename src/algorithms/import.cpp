#include "import.h"

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

#include "pn/network.h"
#include "algorithms/redraw.h"

Project *importTxtType1(QString &fileName)
{
// Open File.
//------------------------------------------------------------------------------
  QFile qfile(fileName);

  if(!qfile.open(QFile::ReadOnly)) {
    QMessageBox::critical(NULL, "File read error.",
                          "Can\'t read " + fileName + ".",
                          QMessageBox::Ok);
    return NULL;
  }

// Create new project.
//------------------------------------------------------------------------------
  Project *project = new Project;

// Set project name.
//------------------------------------------------------------------------------
  int32_t nameInit = fileName.lastIndexOf(QDir::separator());
  int32_t nameFinal = fileName.lastIndexOf('.');
  project->name = fileName.mid(nameInit+1, nameFinal- nameInit -1);

// Set project path.
//------------------------------------------------------------------------------
  project->filePath = fileName;

// Auxiliaty variables.
//------------------------------------------------------------------------------
  QTextStream stream(&qfile); // Text stream used to read lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  QString lineTxt; // Line text.
  QString dummy; // Use to dump useless data.

// Read Number of bars.
//------------------------------------------------------------------------------
  // Loop until NumBarras is found or EoF.
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide NumBarras.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("NumBarras"));

  // Get Number of bars.
  uint32_t numBar;
  stream >> numBar;
  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Read Number of lines.
//------------------------------------------------------------------------------
  // Loop until NumLinhas is found or EoF.
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide NumLinhas.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("NumLinhas"));

  // Get Number of lines.
  uint32_t numLine;
  stream >> numLine;
  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Base Voltage.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide VBase.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("Voltagem_base_da_rede"));

  // Get VBase.
  double VBase;
  stream >> VBase;
  // Save to project.
  project->setVoltageBase(VBase);

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Length Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Length Unit.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("comprimento_das_linhas"));

  // Get Length Unit.
  uint32_t LenUn;
  stream >> LenUn;

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

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Impedance Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Impedance Unit.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("impedancia"));

  // Get Impedance Unit.
  uint32_t ImpUn;
  stream >> ImpUn;

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

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get power Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Power Init.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("Demanda"));

  // Get power Unit.
  uint32_t powerUn;
  stream >> powerUn;

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

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Voltage Unit.
//------------------------------------------------------------------------------
  project->setVoltageUn(Unit::kKiloVolts);

// Get Base Power.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide SBase.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("kVA    kV_hihg "));

  // Get PBase.
  double PBase;
  stream >> dummy >> PBase;
  // Save.
  project->setPowerBase(PBase);

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Bars.
//------------------------------------------------------------------------------
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

  uint32_t id;
  double Sla, Slai;
  double Slb, Slbi;
  double Slc, Slci;
  double Sga, Sgai;
  double Sgb, Sgbi;
  double Sgc, Sgci;

  Bar *bar;

  for(uint32_t i=0; i<numBar; i++) {
    stream >> id >> dummy >> Sla >> Slai >> Slb >> Slbi >> Slc >> Slci;
    stream >> Sga >> Sgai >> Sgb >> Sgbi >> Sgc >>Sgci;
    bar = new Bar;
    bar->id = id - 1;
    bar->Sga.real(Sga);
    bar->Sga.imag(Sgai);
    bar->Sgb.real(Sgb);
    bar->Sgb.imag(Sgbi);
    bar->Sgc.real(Sgc);
    bar->Sgc.imag(Sgci);
    bar->Sla.real(Sla);
    bar->Sla.imag(Slai);
    bar->Slb.real(Slb);
    bar->Slb.imag(Slbi);
    bar->Slc.real(Slc);
    bar->Slc.imag(Slci);

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

// Get Lines.
//------------------------------------------------------------------------------
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

  uint32_t noI, noF;
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

  PnLine *line;

  for(uint32_t i=0; i<numLine; i++) {
    stream >> dummy >> noI >> noF >> Zaa >> Zaai >> Zab >> Zabi >> Zac;
    stream >> Zaci >> Zan >> Zani >> Zbb >> Zbbi >> Zbc >> Zbci >> Zbn >> Zbni;
    stream >> Zcc >> Zcci >> Zcn >> Zcni >> Znn >> Znni >> dummy >> length;

    line = new PnLine;
    line->noI = noI - 1;
    line->noF = noF - 1;

    line->length = length;
    line->Zaa.real(Zaa);
    line->Zaa.imag(Zaai);
    line->Zab.real(Zab);
    line->Zab.imag(Zabi);
    line->Zac.real(Zac);
    line->Zac.imag(Zaci);
    line->Zan.real(Zan);
    line->Zan.imag(Zani);
    line->Zbb.real(Zbb);
    line->Zbb.imag(Zbbi);
    line->Zbc.real(Zbc);
    line->Zbc.imag(Zbci);
    line->Zbn.real(Zbn);
    line->Zbn.imag(Zbni);
    line->Zcc.real(Zcc);
    line->Zcc.imag(Zcci);
    line->Zcn.real(Zcn);
    line->Zcn.imag(Zcni);
    line->Znn.real(Znn);
    line->Znn.imag(Znni);

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

  qfile.close();
  return project;
}

Project *importTxtType2(QString &fileName)
{
// Open File.
//------------------------------------------------------------------------------
  QFile qfile(fileName);

  if(!qfile.open(QFile::ReadOnly)) {
    QMessageBox::critical(NULL, "File read error.",
                          "Can\'t read " + fileName + ".",
                          QMessageBox::Ok);
    return NULL;
  }

// Create new project.
//------------------------------------------------------------------------------
  Project *project = new Project;

// Set project name.
//------------------------------------------------------------------------------
  int32_t nameInit = fileName.lastIndexOf(QDir::separator());
  int32_t nameFinal = fileName.lastIndexOf('.');
  project->name = fileName.mid(nameInit+1, nameFinal- nameInit -1);

// Set project path.
//------------------------------------------------------------------------------
  project->filePath = fileName;

// Auxiliaty variables.
//------------------------------------------------------------------------------
  QTextStream stream(&qfile); // Text stream used to read lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  QString lineTxt; // Line text.
  QString dummy; // Use to dump useless data.

// Read Number of bars.
//------------------------------------------------------------------------------
  // Loop until Numero_barras is found or EoF.
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Numero_barras.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("Numero_barras"));

  // Get Number of bars.
  uint32_t numBar;
  stream >> numBar;
  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Read Number of lines.
//------------------------------------------------------------------------------
  // Loop until Numero_linhas is found or EoF.
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Numero_linhas.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("Numero_linhas"));

  // Get Number of lines.
  uint32_t numLine;
  stream >> numLine;
  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Base Voltage.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide VBase.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("Voltagem_base"));

  // Get VBase.
  double VBase;
  stream >> VBase;
  // Save to project.
  project->setVoltageBase(VBase);

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Length Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Length Unit.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("comprimento"));

  // Get Length Unit.
  uint32_t LenUn;
  stream >> LenUn;

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

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Impedance Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Impedance Unit.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("impedancia"));

  // Get Impedance Unit.
  uint32_t ImpUn;
  stream >> ImpUn;

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

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get power Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName +
                            " doesn't provide Power Init.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("Demanda"));

  // Get power Unit.
  uint32_t powerUn;
  stream >> powerUn;

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

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Voltage Unit.
//------------------------------------------------------------------------------
  project->setVoltageUn(Unit::kKiloVolts);

// Get Base Power.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(NULL, "Invalid File.",
                            "The file " + fileName + " doesn't provide SBase.",
                            QMessageBox::Ok);
      delete project;
      return NULL;
    }
  } while(!lineTxt.contains("kVA    kV_hihg"));

  // Get PBase.
  double PBase;
  stream >> dummy >> PBase;
  // Save.
  project->setPowerBase(PBase);

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Bars.
//------------------------------------------------------------------------------
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

  uint32_t id;
  double Sla, Slai;
  double Slb, Slbi;
  double Slc, Slci;
  double Sga, Sgai;
  double Sgb, Sgbi;
  double Sgc, Sgci;

  Bar *bar;

  for(uint32_t i=0; i<numBar; i++) {
    stream >> id >> dummy >> Sla >> Slai >> Slb >> Slbi >> Slc >> Slci;
    stream >> Sga >> Sgai >> Sgb >> Sgbi >> Sgc >>Sgci;
    bar = new Bar;
    bar->id = id - 1;
    bar->Sga.real(Sga);
    bar->Sga.imag(Sgai);
    bar->Sgb.real(Sgb);
    bar->Sgb.imag(Sgbi);
    bar->Sgc.real(Sgc);
    bar->Sgc.imag(Sgci);
    bar->Sla.real(Sla);
    bar->Sla.imag(Slai);
    bar->Slb.real(Slb);
    bar->Slb.imag(Slbi);
    bar->Slc.real(Slc);
    bar->Slc.imag(Slci);

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

// Get Lines.
//------------------------------------------------------------------------------
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

  uint32_t noI, noF;
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

  PnLine *line;
  QString lineType;

  for(uint32_t i=0; i<numLine; i++) {
    line = new PnLine;
    stream >> lineType >> noI >> noF;

    line->noI = noI - 1;
    line->noF = noF - 1;

    if (lineType.contains("CP")) {
      stream >> dummy >> dummy >> dummy >> dummy >>Zaa >> Zaai >> Zab >> Zabi >> Zac;
      stream >> Zaci >> Zan >> Zani >> Zbb >> Zbbi >> Zbc >> Zbci >> Zbn >> Zbni;
      stream >> Zcc >> Zcci >> Zcn >> Zcni >> Znn >> Znni >> dummy >> length;
      line->length = length;
      line->Zaa.real(Zaa);
      line->Zaa.imag(Zaai);
      line->Zab.real(Zab);
      line->Zab.imag(Zabi);
      line->Zac.real(Zac);
      line->Zac.imag(Zaci);
      line->Zan.real(Zan);
      line->Zan.imag(Zani);
      line->Zbb.real(Zbb);
      line->Zbb.imag(Zbbi);
      line->Zbc.real(Zbc);
      line->Zbc.imag(Zbci);
      line->Zbn.real(Zbn);
      line->Zbn.imag(Zbni);
      line->Zcc.real(Zcc);
      line->Zcc.imag(Zcci);
      line->Zcn.real(Zcn);
      line->Zcn.imag(Zcni);
      line->Znn.real(Znn);
      line->Znn.imag(Znni);
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

  qfile.close();
  return project;
}
