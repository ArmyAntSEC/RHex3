#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

TaskScheduler sched;
RecurringTaskGroup<16> recurring10ms( 10 );


void setUp(void) {
    encoder.forceHomed();
    encoderWrapperHoming.init( millis() ); //Start the homing
}

void tearDown(void) {
    driver.setMotorPWM(0);
    regulator.stop();
    commander.stop();    
}


void testSimpleMove() {        

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
    TEST_ASSERT_INT_WITHIN( 300, 6500, speed );                    

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
    long int endPos = encoder.getPosComp();            
    long int laps = encoder.getLaps();    
    TEST_ASSERT_INT_WITHIN( 100, endPosTargetWrapped, endPos );            
    TEST_ASSERT_EQUAL( lapsTarget, laps );            
}

void testSimpleMoveBackwards() {    
    unsigned long int timeToMove = 1000;  
    unsigned long int endTime = 0;    

    
    endTime = millis() + timeToMove;        
    driver.setMotorPWM(-128);     

    while ( true ) {
        if ( millis() > endTime ) {
            long int endPos = encoder.getPosComp();            
            long int laps = encoder.getLaps();
            TEST_ASSERT_INT_WITHIN( 400, -6000+3592, endPos );            
            TEST_ASSERT_EQUAL( -1, laps );            
            return;
        }
        sched.run();
    }
}

void testSimpleHoming() {
    unsigned long int maxTimeToMove = 5000;  
    unsigned long int endTime = millis() + maxTimeToMove;
    
    encoderWrapperHoming.init( millis() ); //Start the homing

    driver.setMotorPWM(128);    
    encoder.unHome(); //And reset the homing flag.    

    boolean firstRound = true;
    while ( millis() < endTime ) {        
        if ( encoder.isHomed() ) {
            if ( firstRound ) {
                unsigned long int endPos = encoder.getPosAtLastHome();
                TEST_ASSERT_LESS_THAN( 3592, endPos ); //Make sure we home within one rotation            
                encoder.unHome(); //Then we unhome the encoder to keep moving
                firstRound = false;    
                //Log << "First homing" << endl;        
            } else {
                driver.setMotorPWM(0);
                unsigned long int endPos = encoder.getPosAtLastHome();
                TEST_ASSERT_INT_WITHIN( 10, 3592, endPos ); //Make sure we have gone exactly 1 round            
                //Log << "Second homing" << endl;
                return;
            }
        }
        sched.run();
    }    
    TEST_ASSERT_TRUE( encoder.isHomed() );    
}

void testWrapAroundLogic() {
    double lapRemainder = (double)encoder.clicksPerRevolution - 3591;

    long int raw_position = 0;
    SQ1x14 remainder = 0;
    long int laps = 0;
    encoder.handleOverflow ( raw_position, remainder, laps );
    TEST_ASSERT_EQUAL( 0, raw_position );
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, 0 );
    TEST_ASSERT_EQUAL( 0, laps );

    raw_position = 3592;
    remainder = 0;    
    laps = 0;
    encoder.handleOverflow ( raw_position, remainder, laps );
    TEST_ASSERT_EQUAL( 0, raw_position );
    TEST_ASSERT_DOUBLE_WITHIN(5e-4, 1-lapRemainder, (double)remainder );
    TEST_ASSERT_EQUAL( 1, laps );

    raw_position = -3592;
    remainder = 0;    
    laps = 0;
    encoder.handleOverflow ( raw_position, remainder, laps );
    TEST_ASSERT_EQUAL( -1, raw_position );
    TEST_ASSERT_DOUBLE_WITHIN(5e-4, lapRemainder, (double)remainder );
    TEST_ASSERT_EQUAL( -1, laps );

    raw_position = 3592*2;
    remainder = 0;    
    laps = 0;
    encoder.handleOverflow ( raw_position, remainder, laps );
    encoder.handleOverflow ( raw_position, remainder, laps );
    TEST_ASSERT_EQUAL( 0, raw_position );
    TEST_ASSERT_DOUBLE_WITHIN(2*5e-4, 2*(1-lapRemainder), (double)remainder );
    TEST_ASSERT_EQUAL( 2, laps );

    raw_position = -3592*2;
    remainder = 0;    
    laps = 0;
    encoder.handleOverflow ( raw_position, remainder, laps );
    encoder.handleOverflow ( raw_position, remainder, laps );
    TEST_ASSERT_EQUAL( -1, raw_position );
    TEST_ASSERT_DOUBLE_WITHIN(2*5e-4, 2*lapRemainder-1, (double)remainder );
    TEST_ASSERT_EQUAL( -2, laps );

}

void testPositiveSubtraction() {
    TEST_ASSERT_EQUAL( 10, encoder.positionPositiveDifference( 10, 0 ) );
    TEST_ASSERT_EQUAL( 3591-10, encoder.positionPositiveDifference( 0, 10 ) );
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

void testSimpleMoveAtConstantSpeed( unsigned int speedToMoveAt) {    

    unsigned long int timeToMove = 2000;  
    unsigned long int timeToSettle = 1000;
    unsigned long int endTime = 0;    

    endTime = millis() + timeToMove;
    regulator.setSetPoint( speedToMoveAt );
    regulator.init();            

    float n = 0;
    float S = 0;
    float S2 = 0.0;    

    unsigned long int nextRun = millis() + timeToSettle;
    while ( true ) {                
        if ( millis() > endTime ) {
            unsigned long int speed = encoder.getSpeedCPS();                                    
            //Log << "Speed: " << speed << " SetPoint: " << speedToMoveAt << endl;
            TEST_ASSERT_INT_WITHIN( 200, speedToMoveAt, speed );            
            
            float stdDev = sqrt( (n*S2-S*S)/(n*(n-1)) );
            //Log << "Standard deviation: " << stdDev << endl;
            TEST_ASSERT_FLOAT_WITHIN( 100, 0, stdDev );            
            return;
        } else if ( millis() > nextRun ) {
            nextRun += 10;
            unsigned long int speed = encoder.getSpeedCPS();                        
            long int speedRel = speed - speedToMoveAt;
            
            //Compute STD        
            n++;
            S += speedRel;
            S2 += speedRel*speedRel;
            //Log << "speedRel: " << speedRel << " n: " << n << " S: " << S << " S2: " << S2 << endl;
        }

        sched.run();
    }
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
    sched.add( &encoderWrapperHoming );


    UNITY_BEGIN();    
    RUN_TEST(testWrapAroundLogic);  
    RUN_TEST(testPositiveSubtraction);    
    delay(500);
    RUN_TEST(testWrapAroundAndOffset);  
    delay(500);
    RUN_TEST(testSimpleMove);  
    delay(500);
    RUN_TEST(testSimpleMoveBackwards);
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
    RUN_TEST(testSimpleHoming);        
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
