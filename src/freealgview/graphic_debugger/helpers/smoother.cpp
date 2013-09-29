/*
  Copyright (C) 2011 Aarón Bueno Villares <abv150ci@gmail.com>

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

#include "smoother.hpp"

namespace freealgview {

    Smoother::Smoother() : Strategy()
    {
        *this = Smoother(0.1, 1);
    }

    Smoother::Smoother(float gap, float smooth, float ssmooth)
        : m_gap(gap), m_smooth(smooth), m_ssmooth(ssmooth)
    {
        m_originalSmooth = m_smooth;
        m_changed = false;
    }

    float Smoother::x() const
    {
        return m_source.x();
    }

    float Smoother::y() const
    {
        return m_source.y();
    }

    float Smoother::z() const
    {
        return m_source.z();
    }

    float Smoother::gap() const
    {
        return m_gap;
    }

    Smoother::operator VecXf() const
    {
        return m_source;
    }

    VecXf Smoother::source() const
    {
        return m_source;
    }

    VecXf Smoother::target() const
    {
        return m_target;
    }

    VecXf Smoother::dir() const
    {
        return m_dir.unit();
    }

    float Smoother::originalSmooth() const
    {
        return m_originalSmooth;
    }

    float Smoother::actualSmooth() const
    {
        return m_smooth;
    }

    bool Smoother::changed() const
    {
        return m_changed;
    }

    void Smoother::source(const VecXf& source)
    {
        m_source = source;
        m_dir = m_target - m_source;
        m_smooth = m_originalSmooth;
    }

    void Smoother::target(const VecXf& target)
    {
        m_target = target;
        m_dir = m_target - m_source;
        m_smooth = m_originalSmooth;
    }

    void Smoother::moveTarget(const VecXf& delta)
    {
        target(target() + delta);
    }

    void Smoother::smooth(float smooth)
    {
        m_originalSmooth = smooth;
        m_smooth = m_originalSmooth;
    }

    float Smoother::gap(float gap)
    {
        m_gap = gap;
    }

    void Smoother::owner(fspace::fnode owner)
    {
        m_owner = owner;
    }

    fspace::fnode Smoother::owner() const
    {
        return m_owner;
    }

    void Smoother::m_step()
    {
        m_changed = false;

        if (m_dir.norm() > m_gap) {
            m_source += m_dir.unit() * m_dir.norm() * m_smooth;

            m_dir = m_target - m_source;

            m_smooth *= m_ssmooth;

            m_changed = true;
        }
    }

}
