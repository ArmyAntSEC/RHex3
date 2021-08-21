#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

TaskScheduler sched;
RecurringTaskGroup<16> recurring10ms( 10 );


void setUp(void) {
    encoder.forceHomed();    
}

void tearDown(void) {
    driver.setMotorPWM(0);
    regulator.stop();
    commander.stop();    
}

void testWrapAroundLogic() {
    double lapRemainder = (double)encoder.clicksPerRevolution - 1795;

    encoder.state.raw_position = 0;
    encoder.state.position_remainder = 0;
    encoder.state.laps = 0;
    encoder.handleOverflow();
    TEST_ASSERT_EQUAL( 0, encoder.getRawPos() );    
    TEST_ASSERT_EQUAL( 0, encoder.getLaps() );
    
    encoder.state.raw_position = 1796;
    encoder.state.position_remainder = 0;    
    encoder.state.laps = 0;
    encoder.handleOverflow ( );
    TEST_ASSERT_EQUAL( 0, encoder.getRawPos() );
    TEST_ASSERT_DOUBLE_WITHIN(5e-4, 1-lapRemainder, (double)encoder.state.position_remainder );
    TEST_ASSERT_EQUAL( 1, encoder.getLaps() );

    encoder.state.raw_position = 1798;
    encoder.state.position_remainder = 0;    
    encoder.state.laps = 0;
    encoder.handleOverflow ( );
    TEST_ASSERT_EQUAL( 2, encoder.getRawPos() );
    TEST_ASSERT_DOUBLE_WITHIN(5e-4, 1-lapRemainder, (double)encoder.state.position_remainder );
    TEST_ASSERT_EQUAL( 1, encoder.getLaps() );

    encoder.state.raw_position = 1796*2;
    encoder.state.position_remainder = 0;    
    encoder.state.laps = 0;
    encoder.handleOverflow ();
    encoder.handleOverflow ();
    TEST_ASSERT_EQUAL( 0, encoder.getRawPos() );
    TEST_ASSERT_DOUBLE_WITHIN(2*5e-4, 2*(1-lapRemainder), (double)encoder.state.position_remainder );
    TEST_ASSERT_EQUAL( 2, encoder.getLaps() );

}

void testPositiveSubtraction() {
    TEST_ASSERT_EQUAL( 10, encoder.positionPositiveDifference( 10, 0 ) );
    TEST_ASSERT_EQUAL( 1795-10, encoder.positionPositiveDifference( 0, 10 ) );
}

void testSimpleMove() {        
    
    unsigned long int timeToMove = 1000;  
    unsigned long int startTime = millis();
    unsigned long int endTime =  startTime + timeToMove;        

    //First spin up the motor
    driver.setMotorPWM(128);     
    delay(timeToMove);
            
    //Reset the position and then move at constant speed.
    encoder.forceHomed();  
    delay(timeToMove);

    long int endPos = encoder.getRawPos();                
    long int endPosTarget = 2700; //Aproximately how far it should move in 1 second
    TEST_ASSERT_INT_WITHIN( 300, endPosTarget, endPos );                             
}

void testSimpleHoming() {
    unsigned long int maxTimeToMove = 5000;  
    unsigned long int endTime = millis() + maxTimeToMove;    

    driver.setMotorPWM(128);    
    encoder.unHome(); //And reset the homing flag.    

    boolean firstRound = true;
    while ( millis() < endTime ) {        
        if ( encoder.isHomed() ) {
            if ( firstRound ) {
                unsigned long int endPos = encoder.getPosAtLastHome();
                TEST_ASSERT_LESS_THAN( 1796, endPos ); //Make sure we home within one rotation            
                encoder.unHome(); //Then we unhome the encoder to keep moving
                firstRound = false;    
                Log << "First homing" << endl;        
            } else {
                driver.setMotorPWM(0);
                unsigned long int endPos = encoder.getPosAtLastHome();
                TEST_ASSERT_INT_WITHIN( 10, 1796, endPos ); //Make sure we have gone exactly 1 round            
                Log << "Second homing" << endl;
                return;
            }
        }
        sched.run();
    }    
    TEST_ASSERT_TRUE( encoder.isHomed() );    
}

void testWrapAroundAndOffset() {
    unsigned long int maxTimeToMove = 1500;  
    unsigned int startTime = millis();
    unsigned long int endTime = startTime + maxTimeToMove;        

    driver.setMotorPWM(128);    
        
    while ( millis() < endTime ) {                
        sched.run();
    }    
    TEST_ASSERT_LESS_THAN( 3593, encoder.getPosComp() );    
    TEST_ASSERT_EQUAL( 2, encoder.getLaps() );    
}


void testSimpleMoveWithSpeed() {        

    unsigned long int timeToMove = 1000;  
    unsigned long int startTime = millis();
    unsigned long int endTime =  startTime + timeToMove;        

    //First spin up the motor    
    driver.setMotorPWM(128);     
    while ( millis() < endTime ) {
        sched.run();
    }
    
    //Measure the speed
    int speed = encoder.getSpeedCPS();    
    TEST_ASSERT_INT_WITHIN( 150, 2800, speed );                    

    //Compute the distance to move
    timeToMove = 1000;  
    startTime = endTime; //We continue where we left off.
    endTime =  startTime + timeToMove;        
        
    encoder.forceHomed();    
    long int endPosTarget = (timeToMove*speed)/1000;
    int lapsTarget = endPosTarget / HomingEncoder::clicksPerRevolution.getInteger();  //Approximate.
    long int endPosTargetWrapped = endPosTarget % HomingEncoder::clicksPerRevolution.getInteger();  //Approximate.
    
    while ( millis() < endTime ) {        
        sched.run();
    }    
    long int endPos = encoder.getRawPos();            
    long int laps = encoder.getLaps();    
    TEST_ASSERT_INT_WITHIN( 100, endPosTargetWrapped, endPos );            
    TEST_ASSERT_EQUAL( lapsTarget, laps );            
}


void testSimpleMoveAtConstantSpeed( unsigned int speedToMoveAt) {    

    unsigned long int timeToMove = 2000;  
    unsigned long int timeToSettle = 1000;
    unsigned long int endTime = 0;    

    
    regulator.setSetPoint( speedToMoveAt );
    regulator.init();            
    
    //Run to settle the speed.
    endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();        
    }
    
    //Measure the variance
    float n = 0;
    float S = 0;
    float S2 = 0.0;    

    endTime = millis() + timeToMove;
    unsigned long int nextRun = millis() + 10;
    while ( millis() < endTime ) {                                    
        if ( millis() > nextRun ) {
            nextRun+=10;
            unsigned long int speed = encoder.getSpeedCPS();                        
            long int speedRel = speed - speedToMoveAt;
            
            //Compute STD        
            n++;
            S += speedRel;
            S2 += speedRel*speedRel;
            //Log << "speedRel: " << speedRel << " n: " << n << " S: " << S << " S2: " << S2 << endl;
        }
    }

    unsigned long int speed = encoder.getSpeedCPS();                                    
    //Log << "Speed: " << speed << " SetPoint: " << speedToMoveAt << endl;
    TEST_ASSERT_INT_WITHIN( 200, speedToMoveAt, speed );            
    
    float stdDev = sqrt( (n*S2-S*S)/(n*(n-1)) );
    //Log << "Standard deviation: " << stdDev << endl;
    TEST_ASSERT_FLOAT_WITHIN( 100, 0, stdDev );            

}

void testSimpleMoveAtConstantSpeed7000() {
    testSimpleMoveAtConstantSpeed(7000);  
}

void testSimpleMoveAtConstantSpeed4000() {
    testSimpleMoveAtConstantSpeed(4000);  
}

void testSimpleMoveAtConstantSpeed2000() {
    testSimpleMoveAtConstantSpeed(2000);  
}

void testSimpleMoveAtConstantSpeed1000() {
    testSimpleMoveAtConstantSpeed(1000);  
}

void testRegulatorHardBreak()
{
    unsigned long int timeToSettle = 100;      
    int speedToMoveAt = 6000;
    
    regulator.setSetPoint( speedToMoveAt );
    regulator.init();                
    //Run to settle the speed.
    unsigned long int endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();        
    }
    TEST_ASSERT_GREATER_THAN_INT( 0.9*speedToMoveAt, encoder.getSpeedCPS() );            

    regulator.setSetPoint( speedToMoveAt/2 );
    regulator.hardBreak();
    
    endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();                
    }
    TEST_ASSERT_LESS_THAN_INT( 1.1*speedToMoveAt/2, encoder.getSpeedCPS() );

}

void testSimpleMoveToAPositionAtTime() {    
    unsigned long int timeToMove = 500;      
    unsigned long int posToMoveTo = 2500;
        
    regulator.init();    
    unsigned long int endTime = millis() + timeToMove;
    commander.init( endTime, posToMoveTo );  

    
    Log << millis() << " Goal pos: " << posToMoveTo << " at time " << endTime << endl;
    boolean hasArrived = false;
    while ( !hasArrived ) {
        if ( commander.hasArrived() || millis() > endTime + 5000 ) {
            unsigned long int pos = encoder.getPosComp();                
            TEST_ASSERT_INT_WITHIN( 100, posToMoveTo, pos );
            TEST_ASSERT_INT_WITHIN( 100, endTime, millis() );
            hasArrived = true;
        }
        sched.run();
    }
    
    //Now move almost one more complete round
    timeToMove = 1000;
    posToMoveTo = 2000;
    endTime = millis() + timeToMove;
    
    commander.init( endTime, posToMoveTo );  
    
    Log << millis() << " Goal pos: " << posToMoveTo << " at time " << endTime << endl;
    hasArrived = false;
    while ( !hasArrived ) {
        if ( commander.hasArrived() || millis() > endTime + 5000 ) {
            unsigned long int pos = encoder.getPosComp();                
            TEST_ASSERT_INT_WITHIN( 100, posToMoveTo, pos );
            TEST_ASSERT_INT_WITHIN( 100, endTime, millis() );
            hasArrived = true;
        }
        sched.run();
    }    
    TEST_ASSERT_TRUE( hasArrived );
}


void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    initOneLeg();

    regulator.stop(); //Don't start the regulator from the get go.

    recurring10ms.add( &encoderWrapperComputeSpeed );    
    recurring10ms.add( &regulator );
    recurring10ms.add( &commander );
    recurring10ms.init( millis() );  
    sched.add( &recurring10ms );     


    UNITY_BEGIN();        
    RUN_TEST(testWrapAroundLogic);  
    RUN_TEST(testPositiveSubtraction);    
    delay(500);
    RUN_TEST(testSimpleMove);  
    delay(500);    
    RUN_TEST(testSimpleHoming);        
    delay(500);
    RUN_TEST(testWrapAroundAndOffset); 
    delay(500);
    RUN_TEST(testSimpleMoveWithSpeed);  
    /*
    delay(500);
    RUN_TEST(testSimpleMoveAtConstantSpeed7000);    
    delay(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed4000);
    delay(500);
    RUN_TEST(testSimpleMoveAtConstantSpeed2000);
    delay(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed1000);    
    delay(500);         
    RUN_TEST(testSimpleMoveToAPositionAtTime);
    delay(500);    
    RUN_TEST(testRegulatorHardBreak);
    */
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
