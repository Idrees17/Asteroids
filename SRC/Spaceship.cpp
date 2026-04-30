#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"

using namespace std;

Spaceship::Spaceship()
    : GameObject("Spaceship"), mThrust(0), mInvulnerableTime(0)
{
}

Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
    : GameObject("Spaceship", p, v, a, h, r), mThrust(0), mInvulnerableTime(0)
{
}

Spaceship::Spaceship(const Spaceship& s)
    : GameObject(s), mThrust(0), mInvulnerableTime(0)
{
}

Spaceship::~Spaceship(void) {}

void Spaceship::Update(int t)
{
    GameObject::Update(t);
    // Count down invulnerability timer
    if (mInvulnerableTime > 0)
    {
        mInvulnerableTime -= t;
        if (mInvulnerableTime < 0) mInvulnerableTime = 0;
    }
}

void Spaceship::Render(void)
{
    // Flash the ship when invulnerable
    if (mInvulnerableTime > 0)
    {
        // Only render every other 200ms to create flashing effect
        if ((mInvulnerableTime / 200) % 2 == 0) return;
    }

    if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();
    if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
        mThrusterShape->Render();
    }
    GameObject::Render();
}

void Spaceship::Thrust(float t)
{
    mThrust = t;
    mAcceleration.x = mThrust * cos(DEG2RAD * mAngle);
    mAcceleration.y = mThrust * sin(DEG2RAD * mAngle);
}

void Spaceship::Rotate(float r)
{
    mRotation = r;
}

void Spaceship::Shoot(void)
{
    if (!mWorld) return;
    GLVector3f spaceship_heading(cos(DEG2RAD * mAngle), sin(DEG2RAD * mAngle), 0);
    spaceship_heading.normalize();
    GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
    float bullet_speed = 30;
    GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
    shared_ptr<GameObject> bullet
    (new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
    bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
    bullet->SetShape(mBulletShape);
    mWorld->AddObject(bullet);
}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
    // No collisions while invulnerable
    if (mInvulnerableTime > 0) return false;
    if (o->GetType() != GameObjectType("Asteroid") &&
        o->GetType() != GameObjectType("SmallAsteroid")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList& objects)
{
    for (GameObjectList::const_iterator it = objects.begin();
        it != objects.end(); ++it)
    {
        shared_ptr<GameObject> obj = *it;
        if (obj->GetType() == GameObjectType("Asteroid"))
        {
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
        if (obj->GetType() == GameObjectType("SmallAsteroid"))
        {
            GLVector3f normal = mPosition - obj->GetPosition();
            float length = sqrt(normal.x * normal.x + normal.y * normal.y);
            if (length > 0)
            {
                normal.x /= length;
                normal.y /= length;
            }
            float dot = mVelocity.x * normal.x + mVelocity.y * normal.y;
            mVelocity.x = mVelocity.x - 2.0f * dot * normal.x;
            mVelocity.y = mVelocity.y - 2.0f * dot * normal.y;
            return;
        }
    }
}