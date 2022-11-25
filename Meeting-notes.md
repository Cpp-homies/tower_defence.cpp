# Meeting Notes
In this file, you are required to take notes for your weekly meetings. 
In each meeting, you are required to discuss:

1. What each member has done during the week?
2. Are there challenges or problems? Discuss the possible solutions
3. Plan for the next week for everyone
4. Deviations and changes to the project plan, if any

# Meeting 23.11.2022 14:00

**Participants**:
   1. Siim Kasela - 1006443
   2. Saku Kovanen - 1024713
   3. Harvey Lim - 1026986
   4. Hung Vu - 1006058

## Summary of works
   1. Individual work (Siim, Saku, Harvey, Hung)
      - Mess around with Qt to get comfortable with Qt
   2. Saku
      - Graphics for
         - Path
      - Graphics implementation for
         - Tower
            - Facing direction
         - Regular tile
            - Orientation randomization
   3. Siim 
      - Created Qt project
      - Base class for Game
         - QGraphicsGridLayout
      - Base class for Square
      - Base class for Projectile
   4. Harvey
      - Added enums for Square (Path, Tile, Blank)
      - Added map value member to game
   5. Hung
      - Initial tower implementation
      - Tower attack area

   

## Challenges
   1. Merging branches is challenging

## Actions
   1. Siim
      - Basic code for audio
      - High-score scene
      - File generation for high-score
      - Basic enemy
      - Scene switching
      - Further development of Game class
   2. Saku
      - Main menu
         - Graphics
         - QScene
      - Game overlay
   3. Harvey
      - Pathfinding method
      - Change enum implementation of path to classes implementation
   4. Hung
      - Basic tower implementation


## Project status
Development is ongoing. A basic implementation with one type of enemy, tower and path is scheduled to work by the end of the week. This leaves a full week to implement a working demo of the final game. This leaves us a little bit behind the original schedule but still on track for a fully working game.

### TODOs
   1. Finish stage 1 implementations.
   2. Start stage 2 implementations.



<!--
# Meeting dd.mm.2021 HH::MM

**Participants**: 
1. Member 1
2. Member 2
3. Member 3
4. Member 4 

## Summary of works
1. Member 1 
   
   Implementing the class XX. Tested the class XX. 
   Results are in `tests/<class-xx-tests>`. Resolved the identified problems.

2. Member 2

   Same as above

3. ...

## Challenges

1. The integration of UI with the monsters requires an abstract interface.
2. ...

## Actions
1. Member 1 is going to look into defining an abstract interface for monsters 
   to enable easy UI integration.
2. Member 2 is going to work with Member 1 to use abstract interface in derived 
   monster classes.
3. Member 2 is going to test the interface.
4. Member 3 is going to use ...

> Please reflect these action decisions in your git commit messages so that 
> your group members and advisor can follow the progress.

## Project status 
Short summary of current project status. 

### TODOs
1. Member 1: Write an action.
2. ...
-->
