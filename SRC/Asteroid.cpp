#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include "SmallAsteroid.h"

Asteroid::Asteroid(void) : GameObject("Asteroid")
{
    mAngle = rand() % 360;
    mRotation = 0;
    mPosition.x = rand() / 2;
    mPosition.y = rand() / 2;
    mPosition.z = 0.0;
    mVelocity.x = 10.0 * cos(DEG2RAD * mAngle);
    mVelocity.y = 10.0 * sin(DEG2RAD * mAngle);
    mVelocity.z = 0.0;
}

Asteroid::Asteroid(char const* const type_name) : GameObject(type_name)
{
    mAngle = rand() % 360;
    mRotation = 0;
    mPosition.x = rand() / 2;
    mPosition.y = rand() / 2;
    mPosition.z = 0.0;
    mVelocity.x = 10.0 * cos(DEG2RAD * mAngle);
    mVelocity.y = 10.0 * sin(DEG2RAD * mAngle);
    mVelocity.z = 0.0;
}

Asteroid::~Asteroid(void)
{
}

bool Asteroid::CollisionTest(shared_ptr<GameObject> o)
{
    if (GetType() == o->GetType()) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
    // Check what we collided with
    for (GameObjectList::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        shared_ptr<GameObject> obj = *it;
        if (obj->GetType() == GameObjectType("Bullet"))
        {
            // Spawn 2 small asteroids at this position
            for (int i = 0; i < 2; i++)
            {
                shared_ptr<SmallAsteroid> small = make_shared<SmallAsteroid>();
                small->SetPosition(mPosition);
                mWorld->AddObject(small);
            }
        }
    }
    mWorld->FlagForRemoval(GetThisPtr());
}