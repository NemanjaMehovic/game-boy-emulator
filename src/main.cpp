#include "emulator.h"

int
main(int argc, char* argv[])
{
  // Remove qt stuff doesn't work well with sdl
  // QApplication a(argc, argv);
  // MainWindow w;
  // w.show();
  // return a.exec();
  if (argc < 2) {
    log_error("No ROM file provided. Usage: %s <rom_file>", argv[0]);
    return 1;
  }
  std::string file = argv[1];
  std::unique_ptr<Emulator> m_emulator = std::make_unique<Emulator>(file);
  if (m_emulator->isValid()) {
    m_emulator->mainLoop();
  }
  return 0;
}
