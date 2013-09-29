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

#ifndef FREEALGVIEW_SMOOTHER_HPP_INCLUDED
#define FREEALGVIEW_SMOOTHER_HPP_INCLUDED

#include "../../faudata.hpp"

#include "strategy.hpp"
#include "vecxf.hpp"

namespace freealgview
{
    class Smoother : public Strategy
    {
    public:
        Smoother();
        Smoother(float gap, float smooth, float ssmoth = 1);

        float x() const;
        float y() const;
        float z() const;

        float gap() const;

        operator VecXf() const;

        VecXf source() const;
        VecXf target() const;
        VecXf dir() const;

        float originalSmooth() const;
        float actualSmooth() const;

        bool changed() const;

        void source(const VecXf& source);
        void target(const VecXf& target);
        void moveTarget(const VecXf& delta);
        void smooth(float smooth);

        float gap(float gap);

    public: // owner
        void owner(fspace::fnode owner);
        fspace::fnode owner() const;

    protected:
        void m_step();

    private:
        fspace::fnode m_owner;

        VecXf m_source;
        VecXf m_target;

        VecXf m_dir;
        float m_gap;

        float m_originalSmooth;
        float m_smooth;
        float m_ssmooth;

        bool m_changed;
    };
}

#endif
