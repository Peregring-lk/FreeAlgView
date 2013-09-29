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

#ifndef FREEALGVIEW_LAYOUTER_HPP_INCLUDED
#define FREEALGVIEW_LAYOUTER_HPP_INCLUDED

#include "../faudata.hpp"
#include "helpers/pos.hpp"
#include "helpers/props.hpp"

namespace freealgview {

    class Layouter : public Strategy
    {
    public:
        Layouter(float c1, float c2, float c3, float c4, float smooth = 0.15);

        // PhyloNode* root() const;
        // PhyloNode* phyloNode(const Name& name) const;

        // bool changed() const;

        // void clearNode(PhyloNode *node);

        void owner(fspace::fnode owner);

        fspace::fnode tree() const;

        VecXf center() const;
        float radius() const;

        bool changed() const;

    protected:
        void m_init();
        void m_step();

    private:
        fspace::fnode m_owner;

        float m_radiusNode;
        float m_radiusBloom;

        float m_smooth;

        float m_c1;
        float m_c2;
        float m_c3;
        float m_c4;

        VecXf m_center;
        float m_radius;

        bool m_changed;

        typedef std::vector<VecXf> m_Moves;

        VecXf m_fa(Pos const& source, Pos const& target) const;
        VecXf m_fr(Pos const& source, Pos const& target) const;

        void m_calcConvexSphere();
        float m_min(float a, float b);
        float m_max(float a, float b);

        void mf_createPos(fspace::fnode n, fspace::fnode ancestor) const;
        void mf_initialize(fspace::fnode n, int deep = 0) const;
    };
}

#endif
