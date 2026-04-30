#ifndef __EXTRALIFEPOWERUP_H__
#define __EXTRALIFEPOWERUP_H__

#include "GameObject.h"
#include "Player.h"

class ExtraLifePowerup : public GameObject
{
public:
    ExtraLifePowerup(Player* player);
    ~ExtraLifePowerup(void);

    virtual void Render(void);
    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);

private:
    Player* mPlayer;
};

#endif