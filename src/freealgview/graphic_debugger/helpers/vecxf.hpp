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

#ifndef FREEALGVIEW_VECXF_HPP_INCLUDED
#define FREEALGVIEW_VECXF_HPP_INCLUDED

#include <iostream>

namespace freealgview
{
    typedef unsigned Dim;

    class VecXf
    {
    public:
        VecXf(const VecXf& center, float radius);
        VecXf(float x = 0, float y = 0, float z = 0);

        float norm() const;
        VecXf unit() const;

        float coord(Dim dim) const;

        float x() const;
        float y() const;
        float z() const;

        VecXf operator-() const;

        VecXf operator+(const VecXf& vec) const;
        VecXf operator-(const VecXf& vec) const;

        VecXf operator+(float k) const;
        VecXf operator-(float k) const;
        VecXf operator*(float k) const;
        VecXf operator/(float k) const;

        bool inRadius(const VecXf& vec, float radius) const;

        VecXf& operator+=(const VecXf& vec);
        VecXf& operator-=(const VecXf& vec);

        VecXf& operator+=(float k);
        VecXf& operator-=(float k);
        VecXf& operator*=(float k);
        VecXf& operator/=(float k);

        void clear();

        void x(float value);
        void y(float value);
        void z(float value);

        void coord(Dim dim, float value);

    protected:
        float m_x;
        float m_y;
        float m_z;

        mutable float m_norm;

    private:
        mutable bool m_upToDateNorm;

        void m_calcNorm() const;
    };
}

#endif
