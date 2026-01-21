#pragma once
#include <string>
#include "vms.h"

namespace ui {
    std::string login(VMS& vms);
    void createDefaultPasswordFiles();
}