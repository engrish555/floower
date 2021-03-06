#include "automaton.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#define LOG_TAG ""
#else
#include "esp_log.h"
static const char* LOG_TAG = "Automaton";
#endif

#define STATE_STANDBY 0
#define STATE_OPEN_LIT 1
#define STATE_CLOSED_LIT 2
#define STATE_OPEN_RAINBOW 3
#define STATE_CLOSED_RAINBOW 4
#define STATE_REMOTE_INIT 5

Automaton::Automaton(Remote *remote, Floower *floower, Config *config)
    : remote(remote), floower(floower), config(config) {
}

void Automaton::init() {
  changeState(STATE_STANDBY);
  floower->onLeafTouch([=](FloowerTouchEvent event){ onLeafTouch(event); });
  remote->onTakeOver([=]() { onRemoteTookOver(); }); // remote controller took over
}

/**
 * Implemented behavior:
 * 
 * (Standby) - Touch -> (Open + Lit) - Touch -> (Closed + Lit) - Touch -> (Standby)
 * (Standby) - Long Touch -> (Closed + Rainbow) - Touch -> (Closed + Lit)
 * (Open + Lit) - Long Touch -> (Open + Rainbow) - Touch -> (Open + Lit)
 * (Standby) - Hold Touch -> (Connect to Remote) - Touch -> (Standby)
 */
void Automaton::onLeafTouch(FloowerTouchEvent event) {
  switch (event) {
    case TOUCH_DOWN:
      if (state == STATE_OPEN_RAINBOW) {
        floower->stopRainbowRetainColor();
        changeState(STATE_OPEN_LIT);
        disabledTouchUp = true;
      }
      else if (state == STATE_CLOSED_RAINBOW) {
        floower->stopRainbowRetainColor();
        changeState(STATE_CLOSED_LIT);
        disabledTouchUp = true;
      }
      else if (state == STATE_REMOTE_INIT) {
        remote->stopAdvertising();
        floower->setColor(colorBlack, FloowerColorMode::TRANSITION, 500);
        changeState(STATE_STANDBY);
      }
      break;

    case TOUCH_UP:
      if (disabledTouchUp) {
        disabledTouchUp = false;
      }
      else if (floower->isIdle()) {
        if (state == STATE_STANDBY) {
          // open + set color
          if (!floower->isLit()) {
            floower->setColor(nextRandomColor(), FloowerColorMode::TRANSITION, 5000);
          }
          floower->setPetalsOpenLevel(100, 5000);
          changeState(STATE_OPEN_LIT);
        }
        else if (state == STATE_OPEN_LIT) {
          // close
          floower->setPetalsOpenLevel(0, 5000);
          changeState(STATE_CLOSED_LIT);
        }
        else if (state == STATE_CLOSED_LIT) {
          // shutdown
          floower->setColor(colorBlack, FloowerColorMode::TRANSITION, 2000);
          changeState(STATE_STANDBY);
        }
      }
      break;

    case TOUCH_LONG:
      floower->startRainbow();
      changeState(state == STATE_OPEN_LIT ? STATE_OPEN_RAINBOW : STATE_CLOSED_RAINBOW);
      disabledTouchUp = true;
      break;

    case TOUCH_HOLD:
      if (state == STATE_STANDBY || state == STATE_CLOSED_RAINBOW) { // init remote when Floower closed (or rainbow started)
        floower->setColor(colorBlue, FloowerColorMode::FLASH, 1000);
        remote->init();
        remote->startAdvertising();
        changeState(STATE_REMOTE_INIT);
        disabledTouchUp = true;
      }
      break;
  }
}

void Automaton::onRemoteTookOver() {
  // emulate state of automation in case remote took over the control
  if (floower->getPetalsOpenLevel() > 0) {
    changeState(STATE_OPEN_LIT);
  }
  else {
    changeState(STATE_STANDBY);
  }
}

void Automaton::changeState(uint8_t newState) {
  if (state != newState) {
    state = newState;
    ESP_LOGD(LOG_TAG, "Changed state to %d", newState);
  }
}

RgbColor Automaton::nextRandomColor() {
  if (colorsUsed > 0) {
    unsigned long maxColors = pow(2, config->colorSchemeSize) - 1;
    if (maxColors == colorsUsed) {
      colorsUsed = 0; // all colors used, reset
    }
  }

  uint8_t colorIndex;
  long colorCode;
  int maxIterations = config->colorSchemeSize * 3;

  do {
    colorIndex = random(0, config->colorSchemeSize);
    colorCode = 1 << colorIndex;
    maxIterations--;
  } while ((colorsUsed & colorCode) > 0 && maxIterations > 0); // already used before all the rest colors

  colorsUsed += colorCode;
  return config->colorScheme[colorIndex];
}
