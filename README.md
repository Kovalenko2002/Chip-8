# Requirements
This project requires the SDL2 library. To install, use the command.
```bash
sudo apt-get install libsdl2-dev
```

# Compilation
To compile this project, 
In the folder, call ``make all``

To play a game use this command :
```bash
binary/emulator game_rom/<gameName>
```

To translate a game rom, use this command :
```bash
binary/translator game_rom/<gameName> > translatedGame.txt
```
# Controls
The chip-8 controls has 16 keys, simply associated to their correspondin value on a keyboard 1,2,3,4,5,6,7,8,9,0,a,b,c,d,e,f
So it may be hard to play the game and find the right controls.
