#pragma once

#include "components/components.hpp"
#include "scripts/script.hpp"
#include "scripts/general/buttons.hpp"

class primaryAttack : public buttonHandler {
public:

    void buttonRelease() override {
    }

    virtual ~primaryAttack() = default;
};