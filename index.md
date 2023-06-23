## NESolver - Github Pages

Note: This is my first project in 'modern' C++ (as long as you count C++14 as modern). Furthermore, most of my experience with the C family of languages is with C for coursework. If you find any bad practices with my code, either in implementation or syntax, for modern C++, please make an issue ticket and I'll fix it. The goal of this project was to get better at C++ and apply it to a subject that I find interesting (and painful).

While this solver uses extraordinarily little theory learned from ECON 351 - Mathematical Economics: Game Theory, I would like to give credit to Johannes Hörner and the teaching staff, David and Alex, for a great semester. This course was, by far, the most rigorous and difficult course I have taken at Yale, but I did learn a vast amount of epic game theory.

This nash equilibria solver, which finds [pure and mixed strategies](https://en.wikipedia.org/wiki/Strategy_(game_theory)) for two player, matrix (normal form) games. I plan to continue expanding this project to 3+ player, normal form games, but will need to carefully approach these features due to the vastly increased time complexities of solving 3+ player games.

Furthermore, this project will not solve for extensive form (tree) games, as its purpose is to guide my learning of C++; capturing extensive form games would require a GUI, which deviates from this project's purpose. However, if you are trying to solve an extensive form game of perfect information, you can easily convert it to normal form and plug it into my solver!

### Languages and Technologies
- C++
- Eigen
- CMake




