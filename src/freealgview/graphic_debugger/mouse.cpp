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

#include "mouse.hpp"

namespace freealgview {

    Mouse::Mouse(Layouter& layouter, Viewer& viewer)
        : m_layouter(layouter), m_viewer(viewer)
    {}

    VecXf Mouse::pos() const
    {
        return m_pos;
    }

    VecXf Mouse::mov() const
    {
        return m_mov;
    }

    bool Mouse::leftClick() const
    {
        return m_leftClick;
    }

    bool Mouse::changedActualNode() const
    {
        return m_changedActual;
    }

    fspace::fnode Mouse::actualNode() const
    {
        return m_actual;
    }

    bool Mouse::changed() const
    {
        return m_changed;
    }

    void Mouse::setLeftClick(bool click)
    {
        m_leftClick = click;
        m_changed = true;
    }

    void Mouse::setPos(VecXf pos)
    {
        m_mov = pos - m_pos;
        m_pos = pos;

        m_changed = true;
        m_clean = false;
    }

    void Mouse::move(VecXf delta)
    {
        m_mov = delta;
        m_pos += delta;

        m_changed = true;
        m_clean = false;
    }

    void Mouse::clean()
    {
        m_actual = futil::nil;
        m_changedActual = false;
        m_changed = true;
        m_clean = true;
    }

    void Mouse::m_init()
    {
        m_actual = futil::nil;
        m_changedActual = false;
        m_changed = false;
        m_leftClick = false;
        m_clean = false;
    }

    void Mouse::m_step()
    {
        if (!m_clean) {
            fspace::fnode actual = futil::nil;

            if (m_layouter.changed() || m_viewer.changed() || m_changed)
                for (auto i : m_layouter.tree()) {
                    if ((i.get<Props>().proj() - m_pos).norm() < 10) {
                        actual = i;
                        break;
                    }
                }

            if (actual != m_actual) {
                m_changedActual = true;
                m_actual = actual;
            }

            m_changed = false;
            m_mov = VecXf(0.0f, 0.0f);
        }
    }
}
