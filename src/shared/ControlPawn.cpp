#include <shared.hpp>

namespace shared
{
    void ControlPawn::kill()
    {
        if (this->reinforced)
        {
            this->reinforced = false;
        }
        else
        {
            // TODO : delete pawn
        }
    }
}