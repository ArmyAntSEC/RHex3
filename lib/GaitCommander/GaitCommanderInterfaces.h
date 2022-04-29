#pragma once

struct TextReceiverInterface
{
    virtual void receiveText( char* str ) = 0;
};