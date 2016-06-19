//===--- Ball.h - Ball and helper classes definition ---===//
/// \file Ball.h
/// This file declares the Ball class and several helper classess.

#ifndef BALL_H_
#define BALL_H_

#include "Config.h"
#include "cocos2d.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <climits>

/// \class CheckHasMember
/// \brief Helper class used to test if a class has specific members using SFINAE tricks.
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
    
    // Take this as an example.
    // If class T doesn't have an int member named tag, the former function will
    // fail at overloading resolution, but compiler won't give error. It continues
    // to match, until find the latter function returning int, OK. Now HelpInt<T>(0)
    // is an int-typed value, sizeof(HelpInt<T>(0)) is sizeof(int) not sizeof(char).
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

/// \class BallInitializer
/// \brief A generic factory class containing main ball information.
struct BallInitializer {
    template <class BallType>
    /// Template constructor from any kinds of ball.
    BallInitializer(BallType ap) : density(ap.density),
        restitution(ap.restitution), friction(ap.friction), radius(ap.radius),
        linearDamping(ap.linearDamping), mass(ap.mass), tag(ap.tag),
        file(ap.file)
    {
        // If BallType doesn't match our requirements, compiler will give error
        // because static_assert fails.
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
    
    /// Start chain invoking at center.
    /// \param center Center where collection of balls are placed.
    /// \return A new aggregate object for chain invoking.
    BallInitializerAggregate atCenter(const cocos2d::Point& center);
    
    /// Start chain invoking with distance .
    /// \param center Distance in collection of balls are placed between.
    /// \return A new aggregate object for chain invoking.
    BallInitializerAggregate withDistance(float distance);
    
    /// Start chain invoking by line.
    /// \return A new aggregate object for chain invoking.
    BallInitializerAggregate byLine(void);
    
    /// Start chain invoking by column.
    /// \param center Center where collection of balls are placed.
    /// \return A new aggregate object for chain invoking.
    BallInitializerAggregate byColumn(void);
    
    /// Density of ball.
    float density;
    
    /// Restituion of ball.
    float restitution;
    
    /// Friction of ball.
    float friction;

    /// Radius of physicsbody.
    float radius;
    
    /// Linear damping of ball.
    float linearDamping;
    
    /// Mass of Ball.
    float mass;
    
    /// Special tag for this kind of ball.
    int tag;
    
    /// Sprite file path.
    std::string file;
};

/// \class Ball
/// \brief Main role controlled by player in game arena.
class Ball {
    friend class AIPlayer;
    friend class BallsCollection;
public:
    /// Construct a new ball.
    /// \param bi Initializer of this ball.
    /// \param position Position the ball will be placed.
    Ball(const BallInitializer& bi, cocos2d::Vec2 position);
    
    /// \note Copy constructors are DELETED.
    Ball(const Ball& b) = delete;
    
    ~Ball();
    
    /// Send a shoot to this ball.
    /// \param force Force to the ball.
    void move(const Force& force);
    
    /// Hide the ball, used when it goes out of arena.
    void depart(void);
    
    /// Get ball position.
    /// \return Position of this ball.
    cocos2d::Point position(void) const;
    
    /// Test if ball is of some kind of tag.
    /// \param tag Tag to test.
    /// \return Whether the ball is of this tag.
    bool is(int tag) const;
    
    /// Get current velocity in scalar form.
    /// \return Current velocity.
    float currentVelocity(void) const;
protected:
    /// Sprite of the ball.
    cocos2d::Sprite* _sprite;
    
    /// PhysicsBody of the ball.
    cocos2d::PhysicsBody* _ballBody;
    
    float _linearDamping;
    float _mass;
};

/// \class BallInitializerAggregate
/// \brief Class used to initialize many balls at one call.
/// \note This class can be used to make chain invokings.
class BallInitializerAggregate {
public:
    /// Construct a BallInitializerAggregate using single intializer.
    /// \param bi The initializer used in all balls.
    /// \return Current instance for chain invoking.
    BallInitializerAggregate(const BallInitializer& bi);
    
    /// Set center of balls.
    /// \param center Center to be selected.
    /// \return Current instance for chain invoking.
    BallInitializerAggregate& atCenter(const cocos2d::Point& center);
    
    /// Set distance between balls.
    /// \param distance Distance between balls to create.
    /// \return Current instance for chain invoking.
    BallInitializerAggregate& withDistance(float distance);
    
    /// Set balls generated by line. (left to right)
    /// \return Current instance for chain invoking.
    BallInitializerAggregate& byLine(void);
    
    /// Set balls generated by column. (top to bottom)
    /// \return Current instance for chain invoking.
    BallInitializerAggregate& byColumn(void);
    
    /// Set ball numbers.
    /// \param number Number to multiply current ball number with.
    /// \return Current instance for chain invoking.
    BallInitializerAggregate& operator * (unsigned number);
    
    /// Get position of all balls.
    /// \return A vector containing all positions of balls.
    std::vector<cocos2d::Point> getPositions(void) const;
    
    /// Get unified initializer.
    /// \return Initializer used to create all the balls.
    const BallInitializer& getInitializer(void) const;
private:
    /// Initializer used to create all the balls.
    BallInitializer _unitedInitializer;
    
    /// Number of balls.
    unsigned _number;
    
    /// Distance between balls.
    float _spacing;
    
    /// Center of balls collection.
    cocos2d::Point _center;
    
    /// The way balls are placed.
    enum { ByLine, ByColumn } _layout;
};

/// \class BallsCollection
/// \brief Object to contain a number of balls.
class BallsCollection {
    friend class Player;
public:
    /// \class BallId
    /// \brief BallId is used to mark a ball.
    struct BallId {
        /// Default constructor, set id to nil.
        BallId() : id(UINT_MAX) {}
        
        /// Set ball id to an unsigned number.
        BallId(unsigned i) : id(i) {}
        
        /// Get whether the BallId is nil.
        /// \return This id is nil or not.
        bool isNil(void) const
        {
            return id == UINT_MAX;
        }
        
        /// Get a nil BallId.
        /// \return A nil BallId.
        static BallId nil(void)
        {
            return BallId(UINT_MAX);
        }
        
        /// Inner representation of ball id.
        unsigned id;
    };
    
    /// \class BallsCollectionIterator
    /// \brief Iterator class used to walk over a collection by accessing BallIds.
    class BallsCollectionIterator {
        friend class BallsCollection;
    public:
        /// Dereference the iterator.
        /// \return BallId the current iterator pointing to.
        BallId operator * () const
        {
            return BallId(_mapIter->first);
        }
        
        /// Move the iterator forward(front operator).
        /// \return Current iterator.
        const BallsCollectionIterator& operator ++ (int n)
        {
            ++_mapIter;
            return *this;
        }
        
        /// Move the iterator forward(back operator).
        /// \return Current iterator.
        // Return iterator before move will cause memory error.
        BallsCollectionIterator operator ++ (void)
        {
            _mapIter++;
            return *this;
        }
        
        /// Compare whether two iterators are different.
        /// \param rhs Iterator to compare with.
        /// \return Result of comparison.
        bool operator != (const BallsCollectionIterator& rhs) const
        {
            return _mapIter != rhs._mapIter;
        }
    private:
        using InnerIter = std::unordered_map<unsigned, std::unique_ptr<Ball>>::const_iterator;
        
        /// Copy constructor.
        BallsCollectionIterator(const InnerIter& iter) : _mapIter(iter) {}
        
        /// Inner representation is implemented by const iterator to unordered_map.
        InnerIter _mapIter;
    };
    
    /// Get begin iterator of this ball collection.
    /// \return An iterator to first element id.
    BallsCollectionIterator begin(void) const
    {
        return BallsCollectionIterator(_balls.cbegin());
    }
    
    /// Get end iterator of this ball collection.
    /// \return Iterator after the last element.
    BallsCollectionIterator end(void) const
    {
        return BallsCollectionIterator(_balls.cend());
    }
    
    /// Default constructor, no number so set vernier to zero.
    BallsCollection() : _vernier(0) {}
    
    /// Copy constructors are DELETED.
    BallsCollection(const BallsCollection& abc) = delete;
    
    ~BallsCollection() = default;
    
    /// Add several balls using one initializer to this collection.
    /// \param ballInit The initializer aggregate to add.
    /// \return Reference to current collection.
    BallsCollection& operator += (const BallInitializerAggregate& ballInit);
    
    /// Get whether current collection is empty.
    /// \return If no ball exist, it's true.
    bool empty(void) const;
    
    /// Get whether all balls are stopped.
    /// \return If all balls are at rest, it's true.
    bool rest(void) const;
    
    /// Shoot a ball by its ball id.
    /// \param id Id of the ball to shoot.
    /// \param force Force used to shoot.
    void shootBall(BallId id, const Force& force);
    
    /// Know if a position is in any ball, if so, which ball it's in.
    /// \param cursor Position to query.
    /// \return Ball id of the cursor is in. If no, it's nil.
    BallId inWhichBall(const cocos2d::Point& cursor) const;
    
    /// Get position of a specific ball.
    /// \param id Id of ball to select.
    /// \return Position of that ball. If id is invalid, return (0,0).
    cocos2d::Point getPosition(BallId id) const;
    
    /// Get if the ball is in this tag.
    /// \param id Id of the ball to select.
    /// \param tag Tag to check.
    /// \return If the ball is with the tag, return true; if ball not exist, also false.
    bool ballWithTag(BallId id, int tag) const;
    
    /// Add sprite of current balls to a node.
    /// \param parent The node to add to.
    void addBallsToNode(cocos2d::Node* parent);
    
    /// Erase balls that matches some specific condition.
    /// \param pre Predicate, if predicate on a ball is true, the ball will be removed.
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
    /// Fix position of a ball.
    /// \param id Id of ball to adjust.
    /// \param newPosition New position the ball will be move to.
    void adjustBallPosition(BallId id, cocos2d::Point newPosition);
    
    /// Inner mapping table from id to Ball objects.
    std::unordered_map<unsigned, std::unique_ptr<Ball>> _balls;
    
    /// The largest id so far, this just increments but will not decrease.
    unsigned _vernier;
};

#endif // BALL_H_