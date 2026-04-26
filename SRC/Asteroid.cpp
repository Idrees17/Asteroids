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
    if (GetType() == o->GetType()) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
    for (GameObjectList::const_iterator it = objects.begin(); it != objects.end(); ++it)
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
                Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
                shared_ptr<Sprite> sprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
                sprite->SetLoopAnimation(true);
                small->SetSprite(sprite);
                small->SetScale(0.1f);
                small->SetBoundingShape(make_shared<BoundingSphere>(small->GetThisPtr(), 5.0f));
                mWorld->AddObject(small);
            }
        }
    }
    mWorld->FlagForRemoval(GetThisPtr());
}