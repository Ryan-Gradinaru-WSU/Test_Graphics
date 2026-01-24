#include "./../../headers/Math/Vector2.h"

Vector2::Vector2(){
    x = 0;
    y = 0;
}
Vector2::Vector2(float n_x, float n_y){
    x = n_x;
    y = n_y;
}

//sets the x and y of this vector
void Vector2::set(float n_x, float n_y){
    x = n_x;
    y = n_y;
}

//vector operations
Vector2 Vector2::operator+(const Vector2& rhs) const{
    return Vector2(x + rhs.x, y + rhs.y);
}
Vector2 Vector2::operator-(const Vector2& rhs) const{
    return Vector2(x - rhs.x, y - rhs.y);
}
    //scalar operations
Vector2 Vector2::operator*(const float rhs) const{
    return Vector2(x * rhs, y * rhs);
}

Vector2 Vector2::operator/(const float rhs) const{
    if(rhs == 0)
        return Vector2(0, 0);

    return Vector2(x / rhs, y / rhs);
}

//exactly equal (use if you want to know if the floats in both vectors are the same) (subject to ftp innacuracies)
bool Vector2::operator==(const Vector2& rhs) const{
    return x == rhs.x && y == rhs.y;
}

//exactly equal (use if you want to know if the floats in both vectors are the same) (subject to ftp innacuracies)
bool Vector2::operator!=(const Vector2& rhs) const{
    return x != rhs.x || y != rhs.y;
}

//approximately equal (use if you want to know if two floats are "almost" equal)
bool Vector2::almostEquals(const Vector2& other, float epsilon = 0.0001) const{
    return (std::abs(x - other.x)  <= epsilon) && (std::abs(y - other.y) <= epsilon);
}

//returns the magnitude of the Vector2
float Vector2::getMagnitude() const{
    return std::sqrt(x * x + y * y);
}

//returns the magnitude of the Vector2 squared
float Vector2::getMagnitudeSquared() const{
    return x * x + y * y;
}

//returns the distance between this vector and another
float Vector2::distanceTo(const Vector2& other) const{
    return (*this - other).getMagnitude();
}

//returns the angle (rad) in between the two vectors
float Vector2::angleTo(const Vector2& other) const{
    return std::atan2(other.y - y, other.x - x);
}


//normalizes the vector (will change original vector)
void Vector2::normalize(){
    float magSq = getMagnitudeSquared();
    if(magSq < 1e-8f) 
        return;
    float invMag = rsqrt(magSq);
    x *= invMag;
    y *= invMag;
}

//returns a seperate, normalized, vector2
Vector2 Vector2::getNormalized() const{
    float magSq = getMagnitudeSquared();
    if(magSq < 0.000001) 
        return *this;
    float invMag = rsqrt(magSq);
    return Vector2(x * invMag, y * invMag);
}

//checks to see if the magnitude of the vector is 0
bool Vector2::isZero() const{
    return (x * x + y * y) < 0.000001;
}
    
//returns the vector perpendicular to this one (counter-clockwise)
Vector2 Vector2::perpendicular() const{
    return Vector2(-y, x);
}

//returns a vector2 with its magnitude clamped to a certain range with the same direction
Vector2 Vector2::clampMagnitude(float magnitude) const{
    if(getMagnitudeSquared() <= magnitude * magnitude){
        return *this;
    }

    return getNormalized() * magnitude;
}

//returns a vector2 that is in between from to and from scaled by t (t is bounded from 0 to 1)
static Vector2 lerp(const Vector2& from, const Vector2& to, float t){
    if(t < 0)
        t = 0;
    if(t > 1)
        t = 1;
    
    return Vector2(from.x + (from.x - to.x) * t, from.y + (from.y - to.y) *  t);
}

//returns the dot product in between two vectors
static float dot(const Vector2& lhs, const Vector2& rhs){
    return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

std::string Vector2::toString() const {
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}
