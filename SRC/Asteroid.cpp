#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include "SmallAsteroid.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "BoundingSphere.h"
#include "Sprite.h"

Asteroid::Asteroid(void) : GameObject("Asteroid")
{
    mAngle = rand() % 360;
    mRotation = 0;
    mPosition.x = (rand() % 200) - 100;
    mPosition.y = (rand() % 200) - 100;
    mPosition.z = 0.0;
    mVelocity.x = 10.0 * cos(DEG2RAD * mAngle);
    mVelocity.y = 10.0 * sin(DEG2RAD * mAngle);
    mVelocity.z = 0.0;
}

Asteroid::Asteroid(char const* const type_name) : GameObject(type_name)
{
    mAngle = rand() % 360;
    mRotation = 0;
    mPosition.x = (rand() % 200) - 100;
    mPosition.y = (rand() % 200) - 100;
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
    if (o->GetType() == GameObjectType("Bullet"))
    {
        if (mBoundingShape.get() == NULL) return false;
        if (o->GetBoundingShape().get() == NULL) return false;
        return mBoundingShape->CollisionTest(o->GetBoundingShape());
    }
    if (o->GetType() == GameObjectType("Spaceship"))
    {
        if (mBoundingShape.get() == NULL) return false;
        if (o->GetBoundingShape().get() == NULL) return false;
        return mBoundingShape->CollisionTest(o->GetBoundingShape());
    }
    if (o->GetType() == GameObjectType("Asteroid"))
    {
        if (mBoundingShape.get() == NULL) return false;
        if (o->GetBoundingShape().get() == NULL) return false;
        return mBoundingShape->CollisionTest(o->GetBoundingShape());
    }
    if (o->GetType() == GameObjectType("SmallAsteroid"))
    {
        if (mBoundingShape.get() == NULL) return false;
        if (o->GetBoundingShape().get() == NULL) return false;
        return mBoundingShape->CollisionTest(o->GetBoundingShape());
    }
    return false;
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
    for (GameObjectList::const_iterator it = objects.begin(); 
         it != objects.end(); ++it)
    {
        shared_ptr<GameObject> obj = *it;

        if (obj->GetType() == GameObjectType("Bullet"))
        {
            for (int i = 0; i < 2; i++)
            {
                shared_ptr<SmallAsteroid> small = make_shared<SmallAsteroid>();
                GLVector3f pos = mPosition;
                pos.x += (i == 0) ? 5.0f : -5.0f;
                small->SetPosition(pos);
                Animation* anim_ptr = AnimationManager::GetInstance()
                    .GetAnimationByName("asteroid1");
                shared_ptr<Sprite> sprite = make_shared<Sprite>(
                    anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
                sprite->SetLoopAnimation(true);
                small->SetSprite(sprite);
                small->SetScale(0.1f);
                small->SetBoundingShape(make_shared<BoundingSphere>(
                    small->GetThisPtr(), 5.0f));
                mWorld->AddObject(small);
            }
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }

        if (obj->GetType() == GameObjectType("Spaceship"))
        {
            // Destroy spaceship, remove asteroid, no points
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }

        if (obj->GetType() == GameObjectType("Asteroid") ||
            obj->GetType() == GameObjectType("SmallAsteroid"))
        {
            // Bounce - swap velocities along collision normal
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
            return;
        }
    }
}