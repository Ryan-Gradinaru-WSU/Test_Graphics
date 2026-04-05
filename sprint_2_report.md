# Sprint 2 Report (3/02/2026 - 4/04/2026)

## What's New (User Facing)
 * GPU accelerated rendering now functional (textures successfully displayed on screen)
 * Basic sprite rendering pipeline operational
 * Game window initializes with GPU device and shaders
 * Player sprite and test textures load correctly

## Work Summary (Developer Facing)
  During this sprint, the team focused heavily on completing the GPU rendering pipeline using SDL3’s modern GPU API. We implemented shader loading for both SPIR V and DXIL formats, created GPU textures from PNG files, and verified GPU memory transfers using staging and download buffers. A full graphics pipeline was constructed, including vertex buffers, samplers, and render passes. We also validated the rendering path by drawing textured quads on screen. Alongside engine development, we prepared the testing framework for WA4, including Unit, Integration, and System Testing plans. Significant time was spent debugging GPU initialization issues, shader compatibility, and texture upload correctness. The sprint concluded with a stable rendering pipeline and documentation updates.

## Unfinished Work
  * Enemy spawning logic not yet implemented
  * Collision system not fully implemented
  * Player movement system not yet implemented beyond basic rendering
  * Render scaling adjustments not yet integrated into the GPU pipeline
  
These items were discussed during team communication and identified as tasks to be completed in Sprint 3. They were not finished in Sprint 2 due to the time required to stabilize the rendering pipeline and resolve GPU-related issues.

## Completed Issues/User Stories
Here are links to the issues that we completed in this sprint:
 * [Finish Sprint 2 Report](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/5)
 * [Finish Sprint 2 Demo Video](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/6)

GitHub
  * Each issue was assigned story points using labels, and contribution comments were recorded directly in the comment section of each GitHub issue to indicate how many points each team member completed.
  * The Sprint Report task was assigned to YoungUk, for drafting and formatting and Ryan provided code-related information.
  * The demo video task was assigned to Ryan, and I coordinated the task by informing him of the requirements and delegating the recording responsibility.

## Incomplete Issues/User Stories
 Here are links to issues we worked on but did not complete in this sprint:
 
 * [Incomplete Issue: Implement Enemy Spawning Logic](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/7) <<Enemy spawning logic was not implemented as rendering tasks were prioritized.>>
 * [Incomplete Issue: Implement Collision System](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/8) <<Collision system work was delayed due to limited time after resolving GPU-related issues.>>
 * [Incomplete Issue: Implement Player Movement System](https://github.com/Ryan-Gradinaru-WSU/Test_Graphics/issues/9) <<Player movement was not completed due to the time required to stabilize the rendering pipeline.>>
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
  * Team communication improved during debugging
  * Documentation and WA4 testing sections completed early
 
Here's what we'd like to improve:
  * More accurate time estimation for GPU debugging tasks.
  * Earlier integration of gameplay logic into the rendering pipeline.
  * Clearer separation between rendering responsibilities and gameplay responsibilities.
  * More consistent use of GitHub issues and progress tracking.
  
Here are changes we plan to implement in the next sprint:
  * Implement enemy spawning and movement 
  * Connect player movement to rendered sprite
  * Add collision detection
  * Implement HUD rendering
  * Begin building full gameplay loop
  * Add audio (optional)