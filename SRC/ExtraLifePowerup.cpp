#include "ExtraLifePowerup.h"
#include "GameUtil.h"
#include "BoundingShape.h"
#include "GameWorld.h"

ExtraLifePowerup::ExtraLifePowerup(Player* player, Spaceship* spaceship)
    : GameObject("ExtraLifePowerup"), mPlayer(player), mSpaceship(spaceship)
{
    mPosition.x = (rand() % 200) - 100;
    mPosition.y = (rand() % 200) - 100;
    mPosition.z = 0.0;
    mVelocity.x = 2.0f * cos(DEG2RAD * (rand() % 360));
    mVelocity.y = 2.0f * sin(DEG2RAD * (rand() % 360));
    mVelocity.z = 0.0;
}

ExtraLifePowerup::~ExtraLifePowerup(void) {}

void ExtraLifePowerup::Render(void)
{
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 0.0f);  // Green
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 16; i++)
    {
        float angle = 2.0f * M_PI * i / 16;
        glVertex2f(cos(angle) * 5.0f, sin(angle) * 5.0f);
    }
    glEnd();

    // Plus symbol
    glBegin(GL_LINES);
    glVertex2f(-3.0f, 0.0f);
    glVertex2f(3.0f, 0.0f);
    glVertex2f(0.0f, -3.0f);
    glVertex2f(0.0f, 3.0f);
    glEnd();
    glEnable(GL_LIGHTING);
}

bool ExtraLifePowerup::CollisionTest(shared_ptr<GameObject> o)
{
    if (o->GetType() != GameObjectType("Spaceship")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void ExtraLifePowerup::OnCollision(const GameObjectList& objects)
{
    for (GameObjectList::const_iterator it = objects.begin();
        it != objects.end(); ++it)
    {
        shared_ptr<GameObject> obj = *it;
        if (obj->GetType() == GameObjectType("Spaceship"))
        {
            mSpaceship->SetInvulnerable(2000);
            mPlayer->AddLife();
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
    }
}