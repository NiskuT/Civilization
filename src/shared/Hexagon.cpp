#include <shared.hpp>

using namespace shared;

/*!
 * @brief Quentin
 */
FieldLevel Hexagon::getFieldLevel()
{
    return this->level;
}

/*!
 * @brief Quentin
 * @param field
 */
void Hexagon::setFieldType(FieldLevel field)
{
    this->level = field;
}