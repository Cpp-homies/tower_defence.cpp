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
   1. Saku
      - Game overlay arrangement
      - Graphics for
         - Rest of the projectiles
         - Start/endpoint path
      - Graphics implementation for
         - tower graphics over the map tiles
         - Leaderboard
         - Projectiles
      - Level/game design
      - Hotfixes
         - Ghost towers and enemies (Issue#6)
   2. Siim
      - Hotfixes
         - Enemies wondering off the path, 
         - Projectile fired in the wrong direction, 
         - Waves creation bug (twice), 
         - Enemies get stuck on the path, 
         - Game crashed when boss enemy is killed in the final level 
      - Local leaderboard functionality
      - Wave creating from local file
   3. Harvey
      - Added comment tower
      - Changed pathfinding implementation for comments

   4. Hung
      - Other tower types
      - Enemy targeting logic
      - Tower selling
      - Connect tower buying, selling, and upgrading to game money system
      - Fixed some coordinate bugs in Tower functions


## Challenges
   1. Many bugs in big project.

## Actions
   1. Siim
      - Hotfix for some enemies getting past a wall
      - Writing some comments for the code
      - Writing additional checks for user errors (wave creation file)
   2. Saku
      - Level/game design
      - Modify language server shooting mechanics
   3. Harvey
      - Map loading/saving
      - Documentation
      - Comment breaking
      - SFX
   4. Hung
      - Add tooltip for the Buttons
   5. Everyone
      - Adding comments to code

## Project status
The project is very close to its final form. Needs some minor bug fixes and non-essential additional features, but overall the game is playable. The deadline seems to be reachable at the moment to release a fully working implementation that fulfills the objectives of our plan. 

### TODOs
   1. Finish stage 2 implementations.
   2. Create an executable
   3. More detailed todo-list: https://miro.com/app/board/uXjVPIee4qA=/?share_link_id=19406790451



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
