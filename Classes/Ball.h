//===--- Ball.h - Ball class definition ---===//
/// \file Ball.h
/// This file declares the Ball class.

#ifndef BALL_H_
#define BALL_H_

#include "Config.h"
#include "cocos2d.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <climits>

using Force = cocos2d::Vec2;

//class Ball {
//public:
//    // What's the parameters?
//    Ball(ballType type, int id, cocos2d::Vec2 position);
//    virtual ~Ball() {}
//    void move(const Force& force) {
//        _ballBody->applyImpulse(force);
//        _moved = true;
//    }
//    void depart()
//    {
//        _sprite->setVisible(false);
//        _ballBody->setDynamic(false);
//        _ballBody->setEnabled(false);
//    }
//    int getId()
//    {
//        return this->_id;
//    }
//    float getMaxForce()
//    {
//        return _force;
//    }
//    cocos2d::Vec2 getPosition() {
//        return _sprite->getPosition();
//    }
//    cocos2d::Sprite *getSprite() {
//        return _sprite;
//    }
//    cocos2d::PhysicsBody *getBallBody() {
//        return _ballBody;
//    }
//    cocos2d::Rect getBoundingBox() {
//        return _sprite->getBoundingBox();
//    }
//protected:
//    cocos2d::Sprite *_sprite;
//    cocos2d::PhysicsBody *_ballBody;
//    bool _moved;
//    bool _missed;
//    ballType _type;
//    float _radius;
//    float _force;
//    float _mass;
//    float _linearDamping;
//    int _id;
//    //float _speed;
//};

//using BallsCollection = std::vector<Ball*>;

template <class T>
struct CheckHasMember {
    template<typename U, float U::*> struct HelpFloat;
    template<typename U> static char TestDensity(HelpFloat<U, &U::density>*);
    template<typename U> static int TestDensity(...);
    template<typename U> static char TestRestitution(HelpFloat<U, &U::restitution>*);
    template<typename U> static int TestRestitution(...);
    template<typename U> static char TestFriction(HelpFloat<U, &U::friction>*);
    template<typename U> static int TestFriction(...);
    template<typename U> static char TestRadius(HelpFloat<U, &U::radius>*);
    template<typename U> static int TestRadius(...);
    template<typename U> static char TestLinearDamping(HelpFloat<U, &U::linearDamping>*);
    template<typename U> static int TestLinearDamping(...);
    template<typename U> static char TestMass(HelpFloat<U, &U::mass>*);
    template<typename U> static int TestMass(...);
    
    template<typename U, int U::*> struct HelpInt;
    template<typename U> static char TestTag(HelpInt<U, &U::tag>*);
    template<typename U> static int TestTag(...);
    
    template<typename U, const char * U::*> struct HelpString;
    template<typename U> static char TestFile(HelpString<U, &U::file>*);
    template<typename U> static int TestFile(...);
    
    static const bool hasDensity = sizeof(TestDensity<T>(0)) == sizeof(char);
    static const bool hasRestitution = sizeof(TestRestitution<T>(0)) == sizeof(char);
    static const bool hasFriction = sizeof(TestFriction<T>(0)) == sizeof(char);
    static const bool hasRadius = sizeof(TestRadius<T>(0)) == sizeof(char);
    static const bool hasLinearDamping = sizeof(TestLinearDamping<T>(0)) == sizeof(char);
    static const bool hasMass = sizeof(TestMass<T>(0)) == sizeof(char);
    static const bool hasTag = sizeof(TestTag<T>(0)) == sizeof(char);
    static const bool hasFile = sizeof(TestFile<T>(0)) == sizeof(char);
};

class BallInitializerAggregate;

struct BallInitializer {
    template <class BallType>
    BallInitializer(BallType ap) : density(ap.density),
        restitution(ap.restitution), friction(ap.friction), radius(ap.radius),
        linearDamping(ap.linearDamping), mass(ap.mass), tag(ap.tag),
        file(ap.file)
    {
        static_assert(CheckHasMember<BallType>::hasDensity,
                      "BallType should contain a float member named density!");
        static_assert(CheckHasMember<BallType>::hasRestitution,
                      "BallType should contain a float member named restitution!");
        static_assert(CheckHasMember<BallType>::hasFriction,
                      "BallType should contain a float member named friction!");
        static_assert(CheckHasMember<BallType>::hasRadius,
                      "BallType should contain a float member named radius!");
        static_assert(CheckHasMember<BallType>::hasLinearDamping,
                      "BallType should contain a float member named linearDamping!");
        static_assert(CheckHasMember<BallType>::hasMass,
                      "BallType should contain a float member named mass!");
        static_assert(CheckHasMember<BallType>::hasTag,
                      "BallType should contain a int member named tag!");
        static_assert(CheckHasMember<BallType>::hasFile,
                      "BallType should contain a const char * member named file!");
    }
    
    BallInitializerAggregate atCenter(const cocos2d::Point& center);
    BallInitializerAggregate withDistance(float distance);
    BallInitializerAggregate byLine(void);
    BallInitializerAggregate byColumn(void);
    
    float density;
    float restitution;
    float friction;

    float radius;
    float linearDamping;
    float mass;
    int tag;
    std::string file;
};

class Ball {
    friend class AIPlayer;
    friend class BallsCollection;
public:
    Ball(const BallInitializer& bi, cocos2d::Vec2 position);
    Ball(const Ball& b) = delete;
    ~Ball();
    void move(const Force& force);
    void depart(void);
    cocos2d::Point position(void) const;
    bool is(int tag) const;
    float currentVelocity(void) const;
protected:
    cocos2d::Sprite* _sprite;
    cocos2d::PhysicsBody* _ballBody;
    float _linearDamping;
    float _mass;
};

class BallInitializerAggregate {
public:
    BallInitializerAggregate(const BallInitializer& bi);
    BallInitializerAggregate& atCenter(const cocos2d::Point& center);
    BallInitializerAggregate& withDistance(float distance);
    BallInitializerAggregate& byLine(void);
    BallInitializerAggregate& byColumn(void);
    BallInitializerAggregate& operator * (unsigned number);
    std::vector<cocos2d::Point> getPositions(void) const;
    const BallInitializer& getInitializer(void) const;
private:
    BallInitializer _unitedInitializer;
    unsigned _number;
    float _spacing;
    cocos2d::Point _center;
    enum { ByLine, ByColumn } _layout;
};

class BallsCollection {
    friend class Player;
public:
    struct BallId {
        BallId() : id(UINT_MAX) {}
        BallId(unsigned i) : id(i) {}
        bool isNil(void) const
        {
            return id == UINT_MAX;
        }
        
        static BallId nil(void)
        {
            return BallId(UINT_MAX);
        }
        
        unsigned id;
    };
    
    class BallsCollectionIterator {
        friend class BallsCollection;
    public:
        BallId operator * () const
        {
            return BallId(_mapIter->first);
        }
        
        const BallsCollectionIterator& operator ++ (int n)
        {
            ++_mapIter;
            return *this;
        }
        
        BallsCollectionIterator operator ++ (void)
        {
            _mapIter++;
            return *this;
        }
        
        bool operator != (const BallsCollectionIterator& rhs) const
        {
            // TODO when no ball, it will crash?
            return _mapIter != rhs._mapIter;
        }
    private:
        using InnerIter = std::unordered_map<unsigned, std::unique_ptr<Ball>>::const_iterator;
        BallsCollectionIterator(const InnerIter& iter) : _mapIter(iter) {}
        InnerIter _mapIter;
    };
    
    BallsCollectionIterator begin(void) const
    {
        return BallsCollectionIterator(_balls.cbegin());
    }
    
    BallsCollectionIterator end(void) const
    {
        return BallsCollectionIterator(_balls.cend());
    }
    
    BallsCollection() : _vernier(0) {}
    BallsCollection(const BallsCollection& abc) = delete;
    ~BallsCollection() = default;
    BallsCollection& operator += (const BallInitializerAggregate& ballInit);
    
    bool empty(void) const;
    bool rest(void) const;
    void shootBall(BallId id, const Force& force);
    BallId inWhichBall(const cocos2d::Point& cursor) const;
    cocos2d::Point getPosition(BallId id) const;
    void addBallsToNode(cocos2d::Node* parent);
    
    template<typename BallOutPredicate>
    void filter(BallOutPredicate pre)
    {
        for (auto i = std::begin(_balls); i != std::end(_balls);) {
            if (pre(i->second)) {
                i = _balls.erase(i);
            } else {
                ++i;
            }
        }
    }
private:
    // By default, mark of each ball is an integer.
    void adjustBallPosition(BallId id, cocos2d::Point newPosition);
    std::unordered_map<unsigned, std::unique_ptr<Ball>> _balls;
    unsigned _vernier;
};

#endif // BALL_H_