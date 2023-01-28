#include <shared.hpp>

using namespace shared;

void Resource::picked()
{
    this->isAlive = false;
}

ResourceEnum Resource::getType()
{
    return type;
}

Resource::Resource(ResourceEnum resource)
{
    type = resource;
    isAlive = true;
}
