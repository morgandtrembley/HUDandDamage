The Pools folder contains three pools. One that heals the player, one that gives the player shields, and one that damages the player

DamageableObject is an actor that has 100 health. If it's health is reduced to 0, it will destroy itself.

The character has a line trace that draws from the camera in the direction it is pointing. If it hits a damageable object, a widget animation 
    will play over the crosshair displaying the amount of damage done to the object.
    
The playerHUD contains a compass that displays the direction the player is facing at all times and progress bars that reflect the player's health and shields.
