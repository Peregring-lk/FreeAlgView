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

#include <iostream>
#include <cstdlib>

#include "layouter.hpp"

namespace freealgview {

    Layouter::Layouter(float c1, float c2, float c3, float c4, float smooth)
        : m_c1(c1), m_c2(c2), m_c3(c3), m_c4(c4), m_smooth(smooth)
    {
        srand(time(nullptr));
    }

    // fspace::fnode Layouter::phyloNode(const Name& name) const
    // {
    //     for (auto i = begin(); !i.end(); i.next()) {
    //         PhyloNode *node = i.node();

    //         if (node->name() == name)
    //             return node;
    //     }

    //     return NULL;
    // }

    // bool Layouter::changed() const
    // {
    //     return ColorTree::changed() || LocTree::changed();
    // }

    bool Layouter::changed() const
    {
        return m_changed;
    }

    void Layouter::owner(fspace::fnode owner)
    {
        m_owner = owner;
    }

    fspace::fnode Layouter::tree() const
    {
        return m_owner;
    }

    VecXf Layouter::center() const
    {
        return m_center;
    }

    float Layouter::radius() const
    {
        return m_radius;
    }

    void Layouter::mf_createPos(fspace::fnode n, fspace::fnode ancestor) const
    {
        VecXf l_ancestor = ancestor.get<Pos>().source();

        float factor = ancestor.edegree() + 1;

        l_ancestor += l_ancestor * log(1 + 0.01 * factor);

        VecXf l_rand1(l_ancestor, 1.5 * (log(factor) + 1) * m_c2);
        VecXf l_rand2(l_ancestor, log(factor) * m_c2);

        l_rand1.z(0);
        l_rand2.z(0);

        Pos& l_target_pos = n;

        l_target_pos.source(l_rand1);
        l_target_pos.target(l_rand2);
    }

    void Layouter::mf_initialize(fspace::fnode n, int deep) const
    {
        Props& l_props = n;

        if (!l_props.initialized()) {
            l_props.initialized(true);

            for (auto i : n.children()) {
                mf_createPos(i, n);
                mf_initialize(i, deep + 1);
            }

            auto l_ref = *n;

            if (l_ref) {
                mf_createPos(l_ref, n);
                mf_initialize(l_ref, deep + 1);
            }

            Pos& l_pos = n;

            l_pos.smooth(m_smooth);
            l_pos.gap(1.5);
            l_pos.init();
        }
    }

    void Layouter::m_init()
    {
        auto l_travel = m_owner.travel();

        for (auto i : l_travel) { // Initializing henfos
            if (!i.exists<Props>())
                i << Props();

            if (!i.exists<Pos>())
                i << Pos();
        }

        for (auto i : l_travel) { // Nearest ancestor

            for (auto j : i.children()) {
                Props& l_child_props = j;

                l_child_props.nearest_ancestor(i);
            }

            auto l_ref = *i;

            if (l_ref) {
                Props& l_ref_props = l_ref;

                l_ref_props.nearest_ancestor(i);
            }
        }

        mf_initialize(m_owner);

        m_calcConvexSphere();
        m_changed = true;
    }

    // void Layouter::clearNode(fspace::fnode node)
    // {
    //     LocTree::clearNode(node);
    //     ColorTree::clearNode(node);
    // }

    // void Layouter::_init()
    // {
    //     ColorTree::_init();
    //     LocTree::_init();
    // }

    void Layouter::m_step()
    {
        if (changed()) {

            m_changed = false;

            auto l_travel = m_owner.travel();

            m_Moves moves(l_travel.size(), VecXf());

            int i_idx = 0;

            for (auto i = l_travel.begin(); i != l_travel.end(); ++i_idx) {
                fspace::fnode source = *i;

                int j_idx = i_idx + 1;

                for (auto j = ++i; j != l_travel.end(); ++j, ++j_idx) {
                    fspace::fnode target = *j;

                    if (source == target.father() || *source == target ||
                        target == source.father() || *target == source) {
                        moves[j_idx] += m_fa(source, target);
                        moves[i_idx] += m_fa(target, source);
                    }
                    else {
                        moves[j_idx] += m_fr(source, target);
                        moves[i_idx] += m_fr(target, source);
                    }
                }
            }

            i_idx = 0;

            for (auto i : l_travel) {
                Pos& pos = i;

                pos.moveTarget(moves[i_idx++]);
            }

            for (auto i : l_travel) {
                Pos& pos = i;

                pos.step();

                if (pos.changed())
                    m_changed = true;
            }
        }

        if (m_changed)
            m_calcConvexSphere();
    }

    VecXf Layouter::m_fa(Pos const& source, Pos const& target) const
    {
        VecXf l_source = source;
        VecXf l_target = target;

        VecXf vu = l_source - l_target;
        VecXf fa = vu.unit() * m_c1 * log(vu.norm() / m_c2);

        return fa * m_c4;
    }

    VecXf Layouter::m_fr(Pos const& source, Pos const& target) const
    {
        VecXf l_source = source;
        VecXf l_target = target;

        VecXf uv = l_target - l_source;
        VecXf fr = uv.unit() * (m_c3 / pow(uv.norm(), 2));

        return fr * m_c4;
    }

    void Layouter::m_calcConvexSphere()
    {
        VecXf inf, sup;

        inf = sup = m_owner.get<Pos>();

        for (auto i : m_owner) {
            Pos& l_pos = i;

            inf.x(m_min(l_pos.x(), inf.x()));
            inf.y(m_min(l_pos.y(), inf.y()));

            sup.x(m_max(l_pos.x(), sup.x()));
            sup.y(m_max(l_pos.y(), sup.y()));
        }

        m_center = (inf + sup) / 2;
        m_radius = (m_center - inf).norm() + 1;
    }

    float Layouter::m_min(float a, float b)
    {
        return a < b ? a : b;
    }

    float Layouter::m_max(float a, float b)
    {
        return a > b ? a : b;
    }


}
