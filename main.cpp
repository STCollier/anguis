#include "engine/resources.hpp"

int main() {
    Resources res{};

    while (res.gameRunning()) {
        res.beginUpdate();
        res.render();
        res.endUpdate();
    }
}