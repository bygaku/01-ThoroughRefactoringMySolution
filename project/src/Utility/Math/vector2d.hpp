#pragma once
#include <cmath>

namespace my_math
{
    class Vector2D final
    {
    public:
        float x_, y_;

        explicit Vector2D()                           noexcept
            : x_(0.0f), y_(0.0f)
        {
        };

        explicit Vector2D(float x, float y)           noexcept
            : x_(x), y_(y)
        {
        };

        Vector2D operator+(const Vector2D& rhs) const noexcept
        {
            return Vector2D(x_ + rhs.x_, y_ + rhs.y_);
        }

        Vector2D operator-(const Vector2D& rhs) const noexcept
        {
            return Vector2D(x_ - rhs.x_, y_ - rhs.y_);
        }

        Vector2D operator*(float scale)         const noexcept
        {
            return Vector2D(x_ * scale, y_ * scale);
        }

        float Length()                          const noexcept
        {
            return sqrt(x_ * x_ + y_ * y_);
        }

        Vector2D Normalized()                   const noexcept
        {
            float len = Length();
            if (len > 0.0f)  {
                return Vector2D(x_ / len, y_ / len);
            }
            return Vector2D(0.0f, 0.0f);
        }
    };

    //* intŒ^‚ÌVector2D
    /*
    *struct Vector2D_INT
    *{
    *    int x, y;
    *
    *    explicit Vector2D_INT()                               noexcept
    *        : x(0), y(0)
    *    {
    *    };
    *
    *    explicit Vector2D_INT(int x, int y)                   noexcept
    *        : x(x), y(y)
    *    {
    *    };
    *
    *    Vector2D_INT operator+(const Vector2D_INT& rhs) const noexcept
    *    {
    *        return Vector2D_INT(x + rhs.x, y + rhs.y);
    *    }
    *
    *    Vector2D_INT operator-(const Vector2D_INT& rhs) const noexcept
    *    {
    *        return Vector2D_INT(x - rhs.x, y - rhs.y);
    *    }
    *
    *    Vector2D_INT operator*(int scale)               const noexcept
    *    {
    *        return Vector2D_INT(x * scale, y * scale);
    *    }
    *
    *    float length()                          const noexcept
    *    {
    *        return sqrt(x * x + y * y);
    *    }
    *
    *    Vector2D_INT normalized()                   const noexcept
    *    {
    *        float len = length();
    *        if (len > 0.0f) return Vector2D_INT(x / len, y / len);
    *        return Vector2D_INT(0.0f, 0.0f);
    *    }
    *};
    */
}
