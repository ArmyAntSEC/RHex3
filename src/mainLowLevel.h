#ifndef _MAINLOWLEVEL_H_
#define _MAINLOWLEVEL_H_

#include "miniTests/TestSimpleMove.h"
#include "miniTests/TestMoveAtGivenSpeed.h"
#include "miniTests/testMoveToPosition.h"
#include "miniTests/testMoveToPositionAtTime.h"

#include "miniTests/CommandAndControll.h"

#include "main.h"

SimpleMoveTest simpleMoveTest( &encoder, &driver, &dataLogger );
TestMoveAtGivenSpeed testMoveAtGivenSpeed( &encoder, &driver, &regulator, &dataLogger );
TestMoveToPosition testMoveToPos (  &encoder, &driver, &regulator, &dataLogger );
TestMoveToPositionAtTime testMoveToPosAtTime (  &encoder, &driver, &regulator, &dataLogger );

CommandAndControll ctr( &dataLogger );

void setupDetails()
{
    //Initialize the Command and Controll    
    ctr.init();
    ctr.registerRemoteRoutine(&simpleMoveTest,0);  
    ctr.registerRemoteRoutine(&testMoveAtGivenSpeed,1);  
    ctr.registerRemoteRoutine(&testMoveToPos,2);
    ctr.registerRemoteRoutine(&testMoveToPosAtTime,3);

    recurring10ms.add( &simpleMoveTest );      
    recurring10ms.add( &testMoveAtGivenSpeed );      
    recurring10ms.add( &testMoveToPos );      
    recurring10ms.add( &testMoveToPosAtTime ); 
    
    recurring10ms.add( &ctr );
    recurring10ms.add( &dataLogger ); //Run the data logger last.
  
}
#endif