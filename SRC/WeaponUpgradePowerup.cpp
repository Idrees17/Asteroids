#include "WeaponUpgradePowerup.h"
#include "GameUtil.h"
#include "BoundingShape.h"
#include "GameWorld.h"

WeaponUpgradePowerup::WeaponUpgradePowerup(Spaceship* spaceship)
    : GameObject("WeaponUpgradePowerup"), mSpaceship(spaceship)
{
    mPosition.x = (rand() % 200) - 100;
    mPosition.y = (rand() % 200) - 100;
    mPosition.z = 0.0;
    mVelocity.x = 2.0f * cos(DEG2RAD * (rand() % 360));
    mVelocity.y = 2.0f * sin(DEG2RAD * (rand() % 360));
    mVelocity.z = 0.0;
}

WeaponUpgradePowerup::~WeaponUpgradePowerup(void) {}

bool WeaponUpgradePowerup::CollisionTest(shared_ptr<GameObject> o)
{
    if (o->GetType() != GameObjectType("Spaceship")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void WeaponUpgradePowerup::OnCollision(const GameObjectList& objects)
{
    for (GameObjectList::const_iterator it = objects.begin();
        it != objects.end(); ++it)
    {
        shared_ptr<GameObject> obj = *it;
        if (obj->GetType() == GameObjectType("Spaceship"))
        {
            // Give 10 seconds of spread shot
            mSpaceship->SetSpreadShot(10000);
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
    }
}