//===--- Item.h - Item class definition ---===//
/// \file Item.h
/// This file contains declaration of Item class, which
/// plays an important role in game experience.

#ifndef ITEM_H_
#define ITEM_H_

#include "cocos2d.h"

/// \class Item
/// \brief Item can be used by player to make effects.
class Item {
};

class BombItem : public Item {
};

class JointItem : public Item {
};

class ResizeItem : public Item {
};

// putting a bomb is also a kind of network event?
// here I just assume three kinds of items
// 1. directly put a bomb
// 2. set a ball of your own, apply the same force to both ball
// 3. make a ball of your own larger for a round

#endif // ITEM_H_
