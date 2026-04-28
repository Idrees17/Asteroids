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
        o->GetType() != GameObjectType("Spaceship") &&
        o->GetType() != GameObjectType("Asteroid") &&
        o->GetType() != GameObjectType("SmallAsteroid"))
        return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void SmallAsteroid::OnCollision(const GameObjectList& objects)
{
    // Check for bullet
    for (GameObjectList::const_iterator it = objects.begin();
        it != objects.end(); ++it)
    {
        shared_ptr<GameObject> obj = *it;
        if (obj->GetType() == GameObjectType("Bullet"))
        {
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
    }

    // Handle bouncing
    for (GameObjectList::const_iterator it = objects.begin();
        it != objects.end(); ++it)
    {
        shared_ptr<GameObject> obj = *it;
        if (obj->GetType() == GameObjectType("Spaceship") ||
            obj->GetType() == GameObjectType("Asteroid") ||
            obj->GetType() == GameObjectType("SmallAsteroid"))
        {
            GLVector3f normal = mPosition - obj->GetPosition();
            float length = sqrt(normal.x * normal.x + normal.y * normal.y);
            if (length > 0)
            {
                normal.x /= length;
                normal.y /= length;
            }
            GLVector3f myVel = mVelocity;
            GLVector3f otherVel = obj->GetVelocity();
            float myDot = myVel.x * normal.x + myVel.y * normal.y;
            float otherDot = otherVel.x * normal.x + otherVel.y * normal.y;
            mVelocity.x += (otherDot - myDot) * normal.x;
            mVelocity.y += (otherDot - myDot) * normal.y;

            // Small asteroid radius is 5.0f, large is 10.0f
            float overlap = 15.0f - length;
            if (overlap > 0)
            {
                mPosition.x += normal.x * overlap * 0.5f;
                mPosition.y += normal.y * overlap * 0.5f;
            }
            return;
        }
    }
}