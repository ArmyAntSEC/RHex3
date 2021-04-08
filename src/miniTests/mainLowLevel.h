#ifndef _MAINLOWLEVEL_H_
#define _MAINLOWLEVEL_H_

#include "miniTests/TestSimpleMove.h"
#include "miniTests/TestMoveRamp.h"
#include "miniTests/TestMoveAtGivenSpeed.h"
#include "miniTests/testMoveToPosition.h"
#include "miniTests/testMoveToPositionAtTime.h"

//#include "miniTests/CommandAndControll.h"

#include "main.h"

//TestSimpleMove testSimpleMove( &encoder, &driver, &dataLogger );
TestMoveRamp testMoveRamp( &encoder, &driver, &dataLogger );
/*TestMoveAtGivenSpeed testMoveAtGivenSpeed( &encoder, &driver, &regulator, &dataLogger );
TestMoveToPosition testMoveToPos (  &encoder, &driver, &regulator, &dataLogger );
TestMoveToPositionAtTime testMoveToPosAtTime (  &encoder, &driver, &regulator, &dataLogger );*/

//CommandAndControll ctr( &dataLogger );

void setupDetails()
{
    testMoveRamp.init(millis());
    recurring10ms.add( &testMoveRamp );          
}
#endif