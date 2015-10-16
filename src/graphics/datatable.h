#ifndef DATATABLE_H
#define DATATABLE_H

#include <QPainter>
#include <QRectF>
#include <QVector>

class DataTable
{
public:

  // Table options
  static const double kTableLineWidth;
  static const double kTextToLinePad;

  DataTable(int32_t rows, int32_t colums);
  ~DataTable();

  void setSize(QRectF &size);
  void setText(QString &text, int32_t row, int32_t col);

  void drawTable(QPainter *painter);

private:
  int32_t colums_;
  int32_t rows_;
  QRectF size_;
  QString *data_;


};

#endif // DATATABLE_H
