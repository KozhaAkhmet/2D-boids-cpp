## Boids Algorithm
![Example](example.GIF)

## About 
Boids is an artificial life program, developed by Craig Reynolds in 1986, which simulates the flocking behaviour of birds, and related group motion. His paper on this topic was published in 1987 in the proceedings of the ACM SIGGRAPH conference.[1] The name "boid" corresponds to a shortened version of "bird-oid object", which refers to a bird-like object.[2] Reynolds' boid model is one example of a larger general concept, for which many other variations have been developed since. The closely related work of Ichiro Aoki is noteworthy because it was published in 1982 — five years before Reynolds' boids paper.[3] 
## Model details
As with most artificial life simulations, Boids is an example of emergent behavior; that is, the complexity of Boids arises from the interaction of individual agents (the boids, in this case) adhering to a set of simple rules. The rules applied in the simplest Boids world are as follows:

**separation**: steer to avoid crowding local flockmates
**alignment**: steer towards the average heading of local flockmates
**cohesion**: steer to move towards the average position (center of mass) of local 

(Reference: https://en.wikipedia.org/wiki/Boids)

## Notes and Bugs

- The fishes has a constant speed and the algorithm is based on the direction. So the boid algorithm that is implemented is not completely correct as it should been focused on the velocity vector.
- There is a bug with the direction variable in fish class which is not doing a full circle instead it does with extra 45 degrees.
  
## Compile Prerequirments

- g++
- sfml

```bash

$ sudo apt-get install g++ libsfml-dev

```

for other sfml installations: https://www.sfml-dev.org/tutorials/2.6/start-linux.php

## Compile 

```bash

$ g++ main.cpp fish.cpp fish.hpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

```
or 

```bash

$ make
```
