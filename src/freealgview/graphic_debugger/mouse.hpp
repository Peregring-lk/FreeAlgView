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

#ifndef FREEALGVIEW_MOUSE_HPP_INCLUDED
#define FREEALGVIEW_MOUSE_HPP_INCLUDED

#include "layouter.hpp"
#include "viewer.hpp"

namespace freealgview
{
    class Mouse : public Strategy
    {
    public:
        Mouse(Layouter& layouter, Viewer& viewer);

        VecXf pos() const;
        VecXf mov() const;

        bool leftClick() const;
        bool changedActualNode() const;
        fspace::fnode actualNode() const;

        bool changed() const;

        void setLeftClick(bool click);
        void setPos(VecXf pos);
        void move(VecXf delta);

        void clean();

    protected:
        void m_init();
        void m_step();

    private:
        Layouter& m_layouter;
        Viewer& m_viewer;

        bool m_leftClick;
        bool m_clean;
        VecXf m_pos;
        VecXf m_mov;

        bool m_changed;

        fspace::fnode m_actual;
        bool m_changedActual;
    };
}

#endif
