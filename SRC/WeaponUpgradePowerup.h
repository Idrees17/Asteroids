#ifndef __WEAPONUPGRADEPOWERUP_H__
#define __WEAPONUPGRADEPOWERUP_H__

#include "GameObject.h"
#include "Spaceship.h"

class WeaponUpgradePowerup : public GameObject
{
public:
    WeaponUpgradePowerup(Spaceship* spaceship);
    ~WeaponUpgradePowerup(void);

    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);

private:
    Spaceship* mSpaceship;
};

#endif