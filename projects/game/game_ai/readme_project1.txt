Student Name: Minjae Kyung

Project Name: Centrifuge-Mingle Game

What I implemented: I recreated the mingle game scene from Squid Game 2.

Directions (if needed): When you click the mouse, the participants line up and move forward, then circle around. If you click again, they pair up in twos, and one participant who is not paired is eliminated by the staff.

What I liked about the project and framework: The code was structured in an easy-to-understand way, making it easy to analyze.

What I disliked about the project and framework: When exiting ParallelSequencer, the on_exit function of the remaining nodes in the child is not called (I think this could be considered a bug). Also, when moving nodes up and down in the node tree editor, the parent-child relationship of the tree nodes becomes messed up. And I wish there was a refresh button for the node list in the node tree editor.

Any difficulties I experienced while doing the project: Unfortunately, I caught the flu last Thursday and was lying in bed all day until the weekend. Since it was my first time creating a behavior tree, I wandered around trying to come up with ideas and structure. And also, it's a small thing, but it's been almost 5 years since I last used Visual Studio, and using it again after so long felt quite refreshing.

Hours spent: 16 hours

New selector node (name): C_ModifiedParallelSequencer

New decorator nodes (names): D_ShortDelay, D_RepeatThreeTimes (I know. But this was necessary to create a burst shot.)

10 total nodes (names): C_ModifiedParallelSequencer, D_ShortDelay, D_RepeatThreeTimes, L_CircularFormation, L_FormationFollow, L_Wander, L_OrderClear, L_OrderFollow, L_CheckOrder, L_Gather, L_MoveToLeader, L_SpawnAgent, L_WaitForDeath, L_Shoot, L_MoveForward, L_DeleteSelf, L_Explode, L_PlayBGM_RnR, L_PlayBGM_PinkSolder, L_PlaySFX_Explosion, L_PlaySFX_Gun

4 Behavior trees (names): AgentTriangle, Bullet, ParticipantFollower, ParticipantLeader

Extra credit: Formation