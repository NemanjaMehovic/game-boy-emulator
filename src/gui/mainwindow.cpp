#include "mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "rom.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::on_pushButton_clicked()
{
  QString file =
    QFileDialog::getOpenFileName(this, "Select ROM", QDir::homePath(), "gb");
  if (file.isEmpty())
    QMessageBox::information(this, "Error", "No ROM selected");
  else {
    Rom test(file.toStdString());
  }
}
