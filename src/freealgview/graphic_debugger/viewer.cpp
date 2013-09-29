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

/*! \file Viewing.cpp
 *  \brief Fichero que implementa la clase fpt::Viewing.
 *
 *  \author Aarón Bueno Villares, <abv150ci@gmail.com>
 *  \date 04/2011
 *  \version 0.7
 *  \since 0.7
 */

#include <iostream>
#include <GL/glu.h>

#include "viewer.hpp"

namespace freealgview {

    Viewer::Viewer(Layouter& layouter, float minRatio, float maxRatio)
        : m_layouter(layouter),
          m_minRatio(maxRatio), m_maxRatio(maxRatio), m_border(1.2)
    {
        sizeViewport(0, 0);
    }

    VecXf Viewer::center() const
    {
        return m_center.source();
    }

    VecXf Viewer::distance() const
    {
        return m_distance.source();
    }

    bool Viewer::changed() const
    {
        return m_center.changed() || m_distance.changed();
    }

    void Viewer::sizeViewport(GLsizei width, GLsizei height)
    {
        m_width = width;
        m_height = height;

        m_changeViewport = true;
    }

    void Viewer::moveCamera(const VecXf& delta)
    {
        m_delta += delta;
        m_center.target(m_center.target() + delta);
    }

    void Viewer::centering()
    {
        moveCamera(-m_delta);
    }

    void Viewer::m_init()
    {
        m_center.source(m_layouter.center());
        m_distance.source(VecXf(2u, m_layouter.radius()));

        m_center.smooth(0.05);
        m_distance.smooth(0.05);

        m_center.init();
        m_distance.init();
    }

    void Viewer::m_step()
    {
        if (m_layouter.changed() || m_changeViewport)
            m_calcOrtho();

        m_center.step();
        m_distance.step();

        if (changed()) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            m_project();

            if (m_changeViewport) {
                glViewport(0, 0, m_width, m_height);
                m_changeViewport = false;
            }

            glMatrixMode(GL_MODELVIEW);

            m_uploadProj();
        }
    }

    void Viewer::m_calcOrtho()
    {
        VecXf center = m_layouter.center();
        float radius = m_layouter.radius();
        VecXf distance = VecXf(radius, radius);

        /*
         *
         *  Igualamos ratio de Viewport con quad.
         *
         */
        float vwRatio = (float)m_width / m_height;
        float lambda = vwRatio * radius;

        if (lambda > radius)
            distance.x(lambda);
        else
            distance.y(radius / vwRatio);

        /*
         *
         *  Reducimos el ratio coordenadas / píxel.
         *
         */
        float actualRatio = 2 * distance.x() / m_width;

        if (actualRatio < m_maxRatio) {
            distance.x(m_width * m_maxRatio * 0.5);
            distance.y(distance.x() / vwRatio);
        }
        else if (actualRatio > m_minRatio)  {
            distance.x(m_width * m_minRatio * 0.5);
            distance.y(distance.x() / vwRatio);
        }

        /*
         *
         *  Desplazamos el centro
         *
         */
        center += m_delta;

        m_center.target(center);
        m_distance.target(distance);
    }

    void Viewer::m_project()
    {
        VecXf l_center = m_center;
        VecXf l_distance = m_distance;

        VecXf inf = l_center -  l_distance;
        VecXf sup = l_center +  l_distance;

        glOrtho(inf.x(), sup.x(), inf.y(), sup.y(), -1, 1);
    }

    void Viewer::m_uploadProj()
    {
        GLdouble model[16];
        GLdouble proj[16];
        GLint viewport[4];
        GLdouble cx;
        GLdouble cy;
        GLdouble cz;

        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, viewport);

        for (auto i : m_layouter.tree()) {
            Pos& pos = i;

            gluProject(pos.x(), pos.y(), 0, model, proj, viewport,
                       &cx, &cy, &cz);

            i.get<Props>().proj(VecXf(cx, viewport[3] - cy));
        }
    }

}
