#ifndef WEAPONPICKUP_BOUNCER_H
#define WEAPONPICKUP_BOUNCER_H

#include "Pickup.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class BouncerPickup : public Pickup
{
public:
	BouncerPickup(Vector3 position);
	~BouncerPickup();

protected:

private:

};

#endif
