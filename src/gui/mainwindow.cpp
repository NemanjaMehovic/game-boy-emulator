#include "mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "cartridge.h"
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
  QString file = QFileDialog::getOpenFileName(
    this, "Select Cartridge", QDir::homePath(), "gb");
  if (file.isEmpty())
    QMessageBox::information(this, "Error", "No Cartridge selected");
  else {
    Cartridge test(file.toStdString());
  }
}
