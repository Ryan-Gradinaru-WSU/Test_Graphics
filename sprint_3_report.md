# Sprint 3 Report (4/05/2026 - 5/02/2026)

## What's New (User Facing)
  * Player movement implemented with smooth real‑time WASD controls
  * Mouse aim rotation added, allowing the player to always face the cursor
  * Player sprite rendering completed with the final character image
  * Background rendering added to visually define the gameplay area
  * Collision system implemented for multiple purposes (player and walls)
  * Overall gameplays feel significantly improved, making the prototype fully playable

## Work Summary (Developer Facing)
  During this sprint, the team focused on implementing core gameplay systems on top of the previously completed GPU rendering pipeline. Development centered on integrating real time player control, sprite rendering, and scene composition into the engine. We implemented a fully functional WASD based movement system with smooth velocity updates and frame based position changes, along with mouse aim rotation that dynamically aligns the player sprite with the cursor. Rendering work continued with the addition of a dedicated background layer to visually define the gameplay area, and the player sprite was integrated using the existing texture and shader pipeline with updated rotation logic. We also implemented the initial collision framework, designed to support interactions such as wall boundaries, ensuring the system is ready for upcoming gameplay features.

## Unfinished Work
  * Enemy spawning logic not yet implemented
  * Bullets and health bar for enemy and player
  * Game sounds such as enemy and bullet are not implemented yet
  
These items were not completed in Sprint 3 due to the focus on implementing core gameplay features such as movement, rotation, rendering, and the collision framework. Since Sprint 3 is the final sprint for this course, these features will not be completed within the class timeline but may be continued as part of future personal development of the project.

## Completed Issues/User Stories
Here are links to the issues that we completed in this sprint:
 * [Finish Sprint 2 Report](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/5)
 * [Complete Issue: Implement Collision System](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/8)
 * [Completed Issue: Implement Player Movement System](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/9)
 * [Finish Sprint 3 Demo Video](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/12)
 * [Finish Sprint 3 Report](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/13)

GitHub
  * Each issue was assigned story points using labels, and contribution comments were recorded directly in the comment section of each GitHub issue to indicate how many points each team member completed.
  * The Sprint Report task was assigned to YoungUk, for drafting and formatting and Ryan provided code-related information.
  * The demo video task was assigned to Ryan, and I coordinated the task by informing him of the requirements and delegating the recording responsibility.

## Incomplete Issues/User Stories
 Here are links to issues we worked on but did not complete in this sprint:
 
 * [Incomplete Issue: Sprint 2 Demo Video](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/6) <<Recording sprint 2 demo video is not completed yet because of lack of time issue.>>
 * [Incomplete Issue: Implement Enemy Spawning Logic](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/7) <<Enemy spawning logic was not implemented as rendering tasks were prioritized.>>
 * [Incomplete Issue: Integrate Render Scaling](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/10) <<Render scaling was not integrated because GPU debugging took longer than expected.>>

## Code Files for Review
Please review the following code files, which were actively developed during this sprint, for quality:
 * [BaseSDL.h](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/blob/8cf3ac4e8f665a4a6f82d9be2fa3373667ec0da8/headers/Window/BaseSDL.h)
 * [BaseSDL.cpp](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/blob/8cf3ac4e8f665a4a6f82d9be2fa3373667ec0da8/src/Window/BaseSDL.cpp)
 
## Retrospective Summary
Here's what went well:
  * GPU pipeline successfully implemented
  * Texture upload and rendering validated
  * Shader loading stable across SPIR V and DXIL
  * Team communication improved during whole project
  * Implement player movement and aiming with mouse curser
  * Add collision system for multiple purposes (player, walls, enemy, bullet, etc.)
  * Documentation and finishing WA final
 
Here's what we'd like to improve:
  * More accurate time estimation for GPU debugging tasks.
  * Earlier integration of gameplay logic into the rendering pipeline.
  * Clearer separation between rendering responsibilities and gameplay responsibilities.
  * More consistent use of GitHub issues and progress tracking.
  * Begin building full gameplay loop

Here are changes we plan to implement in the next sprint:
  * Implement enemy spawning
  * Add bullets and health bar for enemy and player
  * Add audio for the game
  * Insert effect or image for hit or damage
  * Leaderboard to compete with each player’s score