#include "BouncerPickup.h"

BouncerPickup::BouncerPickup(Vector3 position)
	: Pickup
	(
		"Bouncer",
		"PABHC",
		position,
		Vector3(0.003),
		Vector3(0, PI, 0),
		0.2
	)
{ }

BouncerPickup::~BouncerPickup()
{ }