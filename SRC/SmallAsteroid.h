#ifndef __SMALLASTEROID_H__
#define __SMALLASTEROID_H__

#include "Asteroid.h"

class SmallAsteroid : public Asteroid
{
public:
    SmallAsteroid(void);
    ~SmallAsteroid(void);

    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);
};

#endif