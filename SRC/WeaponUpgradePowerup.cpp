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

void WeaponUpgradePowerup::Render(void)
{
    // Draw a yellow circle with arrow symbol
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 16; i++)
    {
        float angle = 2.0f * M_PI * i / 16;
        glVertex2f(cos(angle) * 5.0f, sin(angle) * 5.0f);
    }
    glEnd();

    // Draw 3 arrows to represent spread shot
    glBegin(GL_LINES);
    // Centre arrow
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 4.0f);
    // Left arrow
    glVertex2f(0.0f, 0.0f);
    glVertex2f(-3.0f, 3.0f);
    // Right arrow
    glVertex2f(0.0f, 0.0f);
    glVertex2f(3.0f, 3.0f);
    glEnd();
    glEnable(GL_LIGHTING);
}

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