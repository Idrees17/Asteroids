#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"

using namespace std;

Spaceship::Spaceship()
    : GameObject("Spaceship"), mThrust(0), mInvulnerableTime(0), mSpreadShotTime(0)
{
}

Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
    : GameObject("Spaceship", p, v, a, h, r), mThrust(0), mInvulnerableTime(0), mSpreadShotTime(0)
{
}

Spaceship::Spaceship(const Spaceship& s)
    : GameObject(s), mThrust(0), mInvulnerableTime(0), mSpreadShotTime(0)
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
    // Count down spread shot timer
    if (mSpreadShotTime > 0)
    {
        mSpreadShotTime -= t;
        if (mSpreadShotTime < 0) mSpreadShotTime = 0;
    }
}

void Spaceship::Render(void)
{
    // Flash when invulnerable
    if (mInvulnerableTime > 0)
    {
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

    // Main bullet - always fired straight ahead
    GLVector3f heading(cos(DEG2RAD * mAngle), sin(DEG2RAD * mAngle), 0);
    heading.normalize();
    GLVector3f bullet_position = mPosition + (heading * 4);
    float bullet_speed = 30;
    GLVector3f bullet_velocity = mVelocity + heading * bullet_speed;

    shared_ptr<GameObject> bullet(new Bullet(
        bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
    bullet->SetBoundingShape(make_shared<BoundingSphere>(
        bullet->GetThisPtr(), 2.0f));
    bullet->SetShape(mBulletShape);
    mWorld->AddObject(bullet);

    // If spread shot active fire 2 extra bullets at angles
    if (mSpreadShotTime > 0)
    {
        // Left bullet - 20 degrees left
        float left_angle = mAngle + 20.0f;
        GLVector3f left_heading(cos(DEG2RAD * left_angle),
            sin(DEG2RAD * left_angle), 0);
        left_heading.normalize();
        GLVector3f left_position = mPosition + (left_heading * 4);
        GLVector3f left_velocity = mVelocity + left_heading * bullet_speed;
        shared_ptr<GameObject> left_bullet(new Bullet(
            left_position, left_velocity, mAcceleration, left_angle, 0, 2000));
        left_bullet->SetBoundingShape(make_shared<BoundingSphere>(
            left_bullet->GetThisPtr(), 2.0f));
        left_bullet->SetShape(mBulletShape);
        mWorld->AddObject(left_bullet);

        // Right bullet - 20 degrees right
        float right_angle = mAngle - 20.0f;
        GLVector3f right_heading(cos(DEG2RAD * right_angle),
            sin(DEG2RAD * right_angle), 0);
        right_heading.normalize();
        GLVector3f right_position = mPosition + (right_heading * 4);
        GLVector3f right_velocity = mVelocity + right_heading * bullet_speed;
        shared_ptr<GameObject> right_bullet(new Bullet(
            right_position, right_velocity, mAcceleration, right_angle, 0, 2000));
        right_bullet->SetBoundingShape(make_shared<BoundingSphere>(
            right_bullet->GetThisPtr(), 2.0f));
        right_bullet->SetShape(mBulletShape);
        mWorld->AddObject(right_bullet);
    }
}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
    if (mInvulnerableTime > 0) return false;
    if (o->GetType() != GameObjectType("Asteroid") &&
        o->GetType() != GameObjectType("SmallAsteroid")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList& objects)
{
    if (mInvulnerableTime > 0) return;

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