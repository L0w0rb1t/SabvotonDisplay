#ifndef ModeManager_h
#define ModeManager_h

#include "Definitions.h"
#include "Mode.h"
#include "Storage.h"
#include <vector>

class ModeManager {
  public:
    ModeManager();

    // Manage modes
    void addMode(const Mode &mode);
    void removeMode(size_t index);
    Mode *getMode(size_t index) const;

    // Persist modes to EEPROM
    void saveCurrentMode(void);
    void saveModes(void);
    void loadModes(void);

    // Get current modes
    size_t getModeCount(void) const;

    void selectMode(int index);

    int current_mode_index = -1;
    bool current_mode_updated = false;

  private:
    std::vector<Mode> modes;
};

#endif
