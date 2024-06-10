# Galaxy simulation with Barnes-Hut algorithm and Verlet integration

This project is a galaxy simulation using the Barnes-Hut algorithm and Verlet integration. Currently the execution of the code is sequential but I will develop a concurrent version as well as translate and refactor the code.

This project was made during my B.Sc. at the *Haute école du paysage, d’ingénierie et d’architecture de Genève (HEPIA)* at Geneva in Switzerland.

## Author

Jean-Daniel Küenzi - jeandanielkuenzi@gmail.com

## Features

Keyboard actions :
- Key 1 -> Toggle quadtree display
- Key 2 -> Toggle super stars display
- Key r -> Init new galaxy (reset)

## Infos

To compile the project, you can use the makefile I made and you must have installed the `SDL2` library.

On Debian distributions you can do this with the commande
```
sudo apt-get install libsdl2-2.0 libsdl2-dev
```
