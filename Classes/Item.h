//
//  Item.hpp
//  ball
//
//  Created by Dcalsky on 16/5/19.
//
//

#ifndef Item_hpp
#define Item_hpp

#include "cocos2d.h"
using namespace cocos2d;

class Item : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(Item);
};

#endif /* Item_hpp */
