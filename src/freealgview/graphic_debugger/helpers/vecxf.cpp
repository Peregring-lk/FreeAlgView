/*
  Copyright (C) 2010 Aarón Bueno Villares <abv150ci@gmail.com>

  This file is part of FreePhyloTree.

  FreePhyloTree is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FreePhyloTree is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FreePhyloTree.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <cmath>

#include "vecxf.hpp"

namespace freealgview {

    VecXf::VecXf(const VecXf& center, float radius)
    {
        float x = radius * (2.0f * rand() / RAND_MAX - 1);
        float y = radius * (2.0f * rand() / RAND_MAX - 1);
        float z = radius * (2.0f * rand() / RAND_MAX - 1);

        *this = VecXf(center.x() + x,
                      center.y() + y,
                      center.z() + z);
    }

    VecXf::VecXf(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
    {
        m_upToDateNorm = false;
    }

    float VecXf::norm() const
    {
        if (!m_upToDateNorm)
            m_calcNorm();

        return m_norm;
    }

    VecXf VecXf::unit() const
    {
        if (!m_upToDateNorm)
            m_calcNorm();

        if (m_norm < 0.00001)
            return *this;
        else
            return *this / m_norm;
    }

    float VecXf::coord(Dim dim) const
    {
        switch(dim) {
        case 0:
            return m_x;
        case 1:
            return m_y;
        case 2:
            return m_z;
        default:
            return 0;
        }
    }

    float VecXf::x() const
    {
        return m_x;
    }

    float VecXf::y() const
    {
        return m_y;
    }

    float VecXf::z() const
    {
        return m_z;
    }

    VecXf VecXf::operator+(const VecXf& vec) const
    {
        return VecXf(*this) += vec;
    }

    VecXf VecXf::operator-() const
    {
        return *this * -1;
    }

    VecXf VecXf::operator-(const VecXf& vec) const
    {
        return VecXf(*this) -= vec;
    }

    VecXf VecXf::operator+(float k) const
    {
        return VecXf(*this) += k;
    }

    VecXf VecXf::operator-(float k) const
    {
        return VecXf(*this) -= k;
    }

    VecXf VecXf::operator*(float k) const
    {
        return VecXf(*this) *= k;
    }

    VecXf VecXf::operator/(float k) const
    {
        return VecXf(*this) /= k;
    }

    bool VecXf::inRadius(const VecXf& vec, float radius) const
    {
        return (*this - vec).norm() < radius;
    }

    VecXf& VecXf::operator+=(const VecXf& vec)
    {
        m_x += vec.x();
        m_y += vec.y();
        m_z += vec.z();

        m_upToDateNorm = false;

        return *this;
    }

    VecXf& VecXf::operator-=(const VecXf& vec)
    {
        m_x -= vec.x();
        m_y -= vec.y();
        m_z -= vec.z();

        m_upToDateNorm = false;

        return *this;
    }

    VecXf& VecXf::operator+=(float k)
    {
        m_x += k;
        m_y += k;
        m_z += k;

        m_upToDateNorm = false;

        return *this;
    }

    VecXf& VecXf::operator-=(float k)
    {
        m_x -= k;
        m_y -= k;
        m_z -= k;

        m_upToDateNorm = false;

        return *this;
    }

    VecXf& VecXf::operator*=(float k)
    {
        m_x *= k;
        m_y *= k;
        m_z *= k;

        m_upToDateNorm = false;

        return *this;
    }

    VecXf& VecXf::operator/=(float k)
    {
        m_x /= k;
        m_y /= k;
        m_z /= k;

        m_upToDateNorm = false;

        return *this;
    }

    void VecXf::clear()
    {
        *this = VecXf();

        m_upToDateNorm = false;
    }

    void VecXf::x(float value)
    {
        m_x = value;

        m_upToDateNorm = false;
    }

    void VecXf::y(float value)
    {
        m_y = value;

        m_upToDateNorm = false;
    }

    void VecXf::z(float value)
    {
        m_z = value;

        m_upToDateNorm = false;
    }

    void VecXf::coord(Dim dim, float value)
    {
        switch(dim) {
        case 0:
            x(value);
            break;
        case 1:
            y(value);
            break;
        case 2:
            z(value);
        default:
            break;
        }
    }

    void VecXf::m_calcNorm() const
    {
        m_norm = sqrt(pow(m_x, 2) + pow(m_y, 2) + pow(m_z, 2));

        m_upToDateNorm = true;
    }

}
