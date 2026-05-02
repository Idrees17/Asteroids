#ifndef __EXTRALIFEPOWERUP_H__
#define __EXTRALIFEPOWERUP_H__

#include "GameObject.h"
#include "Player.h"
#include "Spaceship.h"

class ExtraLifePowerup : public GameObject
{
public:
    ExtraLifePowerup(Player* player, Spaceship* spaceship);
    ~ExtraLifePowerup(void);

    virtual void Render(void);
    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);

private:
    Player* mPlayer;
    Spaceship* mSpaceship;
};

#endif