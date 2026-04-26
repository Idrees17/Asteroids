#include "SmallAsteroid.h"
#include "GameUtil.h"
#include "BoundingShape.h"


SmallAsteroid::SmallAsteroid(void) : Asteroid("SmallAsteroid")
{
    // Slightly faster than large asteroids
    mVelocity.x = 15.0 * cos(DEG2RAD * mAngle);
    mVelocity.y = 15.0 * sin(DEG2RAD * mAngle);
    mVelocity.z = 0.0;
}

SmallAsteroid::~SmallAsteroid(void) {}

bool SmallAsteroid::CollisionTest(shared_ptr<GameObject> o)
{
    if (o->GetType() != GameObjectType("Bullet") &&
        o->GetType() != GameObjectType("Spaceship"))
        return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void SmallAsteroid::OnCollision(const GameObjectList& objects)
{
    mWorld->FlagForRemoval(GetThisPtr());
}