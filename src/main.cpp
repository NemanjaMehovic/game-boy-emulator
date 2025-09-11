#include <QApplication>

#include "emulator.h"
#include "mainwindow.h"

int
main(int argc, char* argv[])
{
  // QApplication a(argc, argv);
  // MainWindow w;
  // w.show();
  // return a.exec();
  std::string file = "/home/nemanja/Downloads/dmg-acid2.gb";
  // file = "/home/nemanja/Downloads/Tetris (JUE) (V1.1) [!].gb";
  // file = "/home/nemanja/Downloads/Legend of Zelda, The - Link's Awakening "
  //        "(USA, Europe) (Rev 2).gb";
  // file = "/home/nemanja/Downloads/Blargg test roms/01-special.gb";
  std::unique_ptr<Emulator> m_emulator = std::make_unique<Emulator>(file);
  m_emulator->mainLoop();
  return 0;
}
