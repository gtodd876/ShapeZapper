//
// Created by George Matthews on 10/22/24.
//

#ifndef VEC2_HPP
#define VEC2_HPP

template<typename T>
class Vec2 {
    T m_x{};
    T m_y{};

public:
    Vec2() = default;
    Vec2(const T &x, const T &y) : m_x{x}, m_y{y} {};
    T x() const { return m_x; }
    T y() const { return m_y; }
    bool operator==(Vec2<T>) const;
};

template<typename T>
bool Vec2<T>::operator==(const Vec2<T> rhs) const {
    return (m_x == rhs.x() && m_y == rhs.y());
}
#endif // VEC2_HPP
