/*
 * Task.h
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#ifndef TASK_H_
#define TASK_H_

class Task { // @suppress("Class has a virtual method and non-virtual destructor")
private:    

public:
    virtual bool canRun(unsigned long now) {
        return true;
    }

    virtual void run(unsigned long now)
    {}        
};

#endif /* TASK_H_ */
