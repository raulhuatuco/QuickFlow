#include "datatable.h"


const double DataTable::kTableLineWidth = 1.0;
const double DataTable::kTextToLinePad = 1.0;

DataTable::DataTable(int32_t rows, int32_t colums) :
  colums_(colums),
  rows_(rows)
{
  data_ = new QString[rows*colums];
}

DataTable::~DataTable()
{
  delete[] data_;
}

void DataTable::setSize(QRectF &size)
{
  size_ = size;
}

void DataTable::setText(QString &text, int32_t row, int32_t col)
{
  data_[row*colums_ + col] = text;
}

void DataTable::drawTable(QPainter *painter)
{
  double cellWidth = size_.width()/colums_;
  double cellHeight = size_.height()/rows_;

  // Set table pen.
  painter->setPen(QPen(Qt::lightGray, kTableLineWidth, Qt::SolidLine));

  // Draw table colums.
  for(int i=1; i<colums_; i++) {
    double x = size_.x() + static_cast<double>(i)*cellWidth;
    painter->drawLine(x, size_.y(), x, size_.y() + size_.height());
  }

  // Draw table rows.
  for(int i=1; i<rows_; i++) {
    double y = size_.y() + static_cast<double>(i)*cellHeight;
    painter->drawLine(size_.x(), y, size_.x() + size_.width(), y);
  }

  // Table text is gray.
  painter->setPen(QPen(Qt::gray, kTableLineWidth, Qt::SolidLine));

  // Fill table.
  for (int i=0; i<rows_; i++) {
    for (int k=0; k<colums_; k++) {
      double x = size_.x() + static_cast<double>(k)*cellWidth + kTextToLinePad;
      double y = size_.y() + static_cast<double>(i)*cellHeight + kTextToLinePad;
      double w = cellWidth - kTextToLinePad;
      double h = cellHeight - kTextToLinePad;

      painter->drawText(x, y, w, h, Qt::AlignCenter, data_[i*colums_ + k]);
    }
  }
}

