//
// Created by simon on 06.03.18.
//

#include <stdio.h>
#include "Merlin.h"

Merlin::Merlin() {

}

// This is a blocking method (blocks until the new position is reached
void Merlin::aimAt(float latitude, float longitude) {
    printf("Merlin: aiming at lat %.2f long %.2f\n", latitude, longitude);
    // TODO

    bool targetReached = false;

    while(!targetReached) {
        // Check where we are currently

        // Calculate which servos to move how much to get to new position

        // Move servos

        // Check if we reached the target
        targetReached = true;
    }
}