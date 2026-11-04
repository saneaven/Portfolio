#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes

#include "ControlFlow/C_ModifiedParallelSequencer.h"

// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_ShortDelay.h"
#include "Decorator/D_RepeatThreeTimes.h"


// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"
#include "Leaf/L_PlaySound.h"

// Student Leaf Nodes
#include "Leaf/L_CircularFormation.h"
#include "Leaf/L_FormationFollow.h"
#include "Leaf/L_Wander.h"
#include "Leaf/L_OrderClear.h"
#include "Leaf/L_OrderFollow.h"
#include "Leaf/L_CheckOrder.h"
#include "Leaf/L_Gather.h"
#include "Leaf/L_MoveToLeader.h"
#include "Leaf/L_SpawnAgent.h"
#include "Leaf/L_WaitForDeath.h"
#include "Leaf/L_Shoot.h"
#include "Leaf/L_MoveForward.h"
#include "Leaf/L_DeleteSelf.h"
#include "Leaf/L_Explode.h"
#include "Leaf/L_PlayBGM_RnR.h"
#include "Leaf/L_PlayBGM_PinkSolder.h"
#include "Leaf/L_PlaySFX_Explosion.h"
#include "Leaf/L_PlaySFX_Gun.h"