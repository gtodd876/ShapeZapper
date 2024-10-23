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
    bool operator!=(Vec2<T>) const;
    Vec2<T> operator+(const Vec2<T> &rhs);
    Vec2<T> operator-(const Vec2<T> &rhs);
    Vec2<T> operator*(const Vec2<T> &rhs);
    Vec2<T> operator/(const Vec2<T> &rhs);
    // Vec2<T> length() { }
    // Vec2<T> normalize() { }
};

template<typename T>
bool Vec2<T>::operator==(const Vec2<T> rhs) const {
    return (m_x == rhs.x() && m_y == rhs.y());
}

template<typename T>
bool Vec2<T>::operator!=(const Vec2<T> rhs) const {
    return (m_x != rhs.x() || m_y != rhs.y());
}

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T> &rhs) {
    Vec2<T> result{m_x + rhs.x(), m_y + rhs.y()};
    return result;
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T> &rhs) {
    Vec2<T> result{m_x - rhs.x(), m_y - rhs.y()};
    return result;
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const Vec2<T> &rhs) {
    Vec2<T> result{m_x * rhs.x(), m_y * rhs.y()};
    return result;
}

template<typename T>
Vec2<T> Vec2<T>::operator/(const Vec2<T> &rhs) {
    Vec2<T> result{m_x / rhs.x(), m_y / rhs.y()};
    return result;
}

#endif // VEC2_HPP
