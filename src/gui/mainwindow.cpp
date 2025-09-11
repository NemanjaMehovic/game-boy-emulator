#include "mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "emulator.h"
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
  if (file.isEmpty()) {
    QMessageBox::information(this, "Error", "No Cartridge selected");
    return;
  }
  m_emulator = std::make_unique<Emulator>(file.toStdString());
  if (!m_emulator->isValid()) {
    QMessageBox::information(
      this, "Error", "Failed to create a valid emulator");
    m_emulator.reset();
  }
}

void
MainWindow::on_pushButton_2_clicked()
{
  if (m_emulator) {
    hide();
    m_emulator->mainLoop();
    show();
  }
  // if (m_emulator) {
  //   m_emulator->cycleFrame();
  // }
}
