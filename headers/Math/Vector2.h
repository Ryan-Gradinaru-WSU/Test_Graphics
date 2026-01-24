#include "./../default_includes.h"

class Vector2{
public:
    float x;
    float y;

    Vector2();
    Vector2(float n_x, float n_y);

    //sets the x and y of this vector
    void set(float n_x, float n_y);

    //vector operations
    Vector2 operator+(const Vector2& rhs) const;
    Vector2 operator-(const Vector2& rhs) const;
    //scalar operations
    Vector2 operator*(const float rhs) const;
    Vector2 operator/(const float rhs) const;

    //exactly equal (use if you want to know if the floats in both vectors are the same) (subject to ftp innacuracies)
    bool operator==(const Vector2& rhs) const;

    //outputs !(lhs == rhs)
    bool operator!=(const Vector2& rhs) const;

    //approximately equal (use if you want to know if two floats are "almost" equal)
    bool almostEquals(const Vector2& other, float epsilon) const;

    //returns the magnitude of the Vector2
    float getMagnitude() const;

    //returns the magnitude of the Vector2 squared
    float getMagnitudeSquared() const;

    //returns the distance between this vector and another
    float distanceTo(const Vector2& other) const;

    //returns the angle (rad) in between the two vectors
    float angleTo(const Vector2& other) const;


    //normalizes the vector (will change original vector)
    void normalize();

    //returns a seperate, normalized, vector2
    Vector2 getNormalized() const;

    //checks to see if the magnitude of the vector is 0
    bool isZero() const;
    
    //returns the vector perpendicular to this one (counter-clockwise)
    Vector2 perpendicular() const;

    //returns a vector2 with its magnitude clamped to a certain range with the same direction
    Vector2 clampMagnitude(float magnitude) const;

    //returns a vector2 that is in between from to and from scaled by t (t is bounded from 0 to 1)
    static Vector2 lerp(const Vector2& from, const Vector2& to, float t);

    //returns the dot product in between two vectors
    static float dot(const Vector2& lhs, const Vector2& rhs);

    std::string toString() const; 
};