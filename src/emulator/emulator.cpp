#include "emulator.h"

#include <SDL2/SDL.h>
#include <thread>
#include <chrono>

#include "common.h"

Emulator::Emulator(std::string file)
{
  log_info("Creating emulator with %s", file.c_str());
  m_cartridge = std::make_unique<Cartridge>(file);

  if (!m_cartridge->isValidCartridge()) {
    log_error("Failed to create a cartridge with %s", file.c_str());
  }
  m_cpu = std::make_unique<CPU>();
  m_ppu = std::make_unique<PPU>();
  m_timer = std::make_unique<Timer>();
  m_apu = std::make_unique<APU>();
  m_joypad = std::make_unique<Joypad>();
  m_mmu = std::make_unique<MMU>(
    m_cpu.get(), m_cartridge.get(), m_ppu.get(), m_timer.get(), m_apu.get(), m_joypad.get());
  m_cpu->setMMU(m_mmu.get());
  m_ppu->setMMU(m_mmu.get());
  m_timer->setMMU(m_mmu.get());
  m_joypad->setMMU(m_mmu.get());
  m_Tcycles = 0;
}

Emulator::~Emulator()
{
  if (m_gameThread != nullptr) {
    delete m_gameThread;
  }
}

bool
Emulator::isValid()
{
  return m_cartridge->isValidCartridge();
}

void
Emulator::cycleFrame()
{
  constexpr uint64 TicksPerFrame = 70224;
  if (m_Tcycles == 0) {
    while (m_ppu->getMode() == PpuMode::VBlank) {
      m_cpu->tick(m_Tcycles);
      if ((m_Tcycles % 4) == 3) {
        m_timer->M_tick();
      }
      m_ppu->tick(m_Tcycles);
      m_ppu->tick_dma(m_Tcycles);
      m_apu->tick();
      m_Tcycles++;
    }
    log_info("It took %ld ticks to get out of vblanks", m_Tcycles);
  }
  for (uint64 i = 0; i < TicksPerFrame; i++) {
    m_cpu->tick(m_Tcycles);
    if ((m_Tcycles % 4) == 3) {
      m_timer->M_tick();
    }
    m_ppu->tick(m_Tcycles);
    m_ppu->tick_dma(m_Tcycles);
    m_apu->tick();
    m_Tcycles++;
  }
}

void
Emulator::mainLoop()
{
  SDL_Window* sdlWindow;
  SDL_Renderer* sdlRenderer;
  SDL_Texture* sdlTexture;
  SDL_Surface* screen;

  constexpr int SCALE = 4;

  constexpr int SCREEN_WIDTH = SCALE * GB_WIDTH;
  constexpr int SCREEN_HEIGHT = SCALE * GB_HEIGHT;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(
    SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlWindow, &sdlRenderer);
  screen = SDL_CreateRGBSurface(0,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                32,
                                0x00FF0000,
                                0x0000FF00,
                                0x000000FF,
                                0xFF000000);
  sdlTexture = SDL_CreateTexture(sdlRenderer,
                                 SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STREAMING,
                                 SCREEN_WIDTH,
                                 SCREEN_HEIGHT);
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0) {
      if (e.type == SDL_WINDOWEVENT &&
          e.window.event == SDL_WINDOWEVENT_CLOSE) {
        running = false;
      }
    }


    cycleFrame();

    SDL_Rect rc;
    rc.x = rc.y = 0;
    rc.w = SCREEN_WIDTH;
    rc.h = SCREEN_HEIGHT;

    for (int line_num = 0; line_num < GB_HEIGHT; line_num++) {
      for (int x = 0; x < GB_WIDTH; x++) {
        rc.x = x * SCALE;
        rc.y = line_num * SCALE;
        rc.w = SCALE;
        rc.h = SCALE;

        uint32 c = m_ppu->LCD_PIXELS[(line_num * GB_WIDTH) + x];
        SDL_FillRect(screen, &rc, c);
      }
    }

    SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
    std::this_thread::sleep_for(std::chrono::milliseconds(17));
  }

  SDL_DestroyTexture(sdlTexture);
  SDL_FreeSurface(screen);
  SDL_DestroyRenderer(sdlRenderer);
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
}

void
Emulator::run()
{
  if (m_gameThread == nullptr) {
    m_gameThread = new std::thread(&Emulator::mainLoop, this);
  }
}
