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

#ifndef FREEALGVIEW_PROPS_HPP_INCLUDED
#define FREEALGVIEW_PROPS_HPP_INCLUDED

#include "../../faudata.hpp"
#include "pos.hpp"

namespace freealgview
{
    using Glow = Smoother;

    class Props
    {
    public: // Queries
        bool showedName() const
        { return m_showed_name; }

        fspace::fnode nearest_ancestor() const
        { return m_nearest_ancestor; }

        fspace::fnode owner() const
        { return m_owner; }

        VecXf proj() const
        { return m_proj;  }

        Glow& glow()
        { return m_glow; }

        bool initialized() const
        { return m_initialized; }

        bool visible() const
        { return m_visible; }

    public: // Requests

        void showedName(bool show)
        { m_showed_name = show; }

        void owner(fspace::fnode node) const
        {
            m_owner = node;
        }

        void nearest_ancestor(fspace::fnode node)
        {
            if (!m_nearest_ancestor)
                m_nearest_ancestor = node;
            else if (node == m_owner.father())
                m_nearest_ancestor = node;
            // else {
            //     VecXf l_pos = m_owner.get<Pos>();
            //     VecXf l_near_pos = m_nearest_ancestor.get<Pos>();
            //     VecXf l_new_pos = node.get<Pos>();

            //     float l_actual_dist = (l_pos - l_near_pos).norm();
            //     float l_new_dist = (l_pos - l_new_pos).norm();

            //     if (l_new_dist < l_actual_dist)
            //         m_nearest_ancestor = node;
            // }
        }

        void proj(VecXf const& proj)
        { m_proj = proj; }

        void randGlow(float center)
        {
            VecXf rand(VecXf(center), center * 0.3);

            glow().source(rand.x());
        }

        void glow(Glow const& glow)
        { m_glow = glow; }

        bool initialized(bool b)
        { m_initialized = b; }

        void visible(bool b)
        { m_visible = b; }

    private:
        mutable fspace::fnode m_owner;

        fspace::fnode m_nearest_ancestor;
        bool m_showed_name = false;
        VecXf m_proj;
        Glow m_glow;
        bool m_initialized = false;
        bool m_visible = true;
    };
}

#endif
