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

/*! \file Viewing.hpp
 *  \brief Fichero que define la clase fpt::Viewing.
 *
 *  \author Aarón Bueno Villares, <abv150ci@gmail.com>
 *  \date 04/2011
 *  \version 0.7
 *  \since 0.7
 */


#ifndef FREEALGVIEW_VIEWER_HPP_INCLUDED
#define FREEALGVIEW_VIEWER_HPP_INCLUDED

#include "layouter.hpp"

#include "helpers/strategy.hpp"
#include "helpers/smoother.hpp"

#include <GL/gl.h>

namespace freealgview
{
    class Viewer : public Strategy
    {
    public:
        Viewer(Layouter& layouter,
               float minRatio = 10.0f / 7, float maxRatio = 10.0f / 12);

        VecXf center() const;
        VecXf distance() const;
        bool changed() const;

        void sizeViewport(GLsizei width, GLsizei height);
        void moveCamera(const VecXf& delta);
        void centering();

    protected:
        void m_init();
        void m_step();

    private:
        Layouter& m_layouter;

        GLsizei m_width;
        GLsizei m_height;

        VecXf m_delta;

        Smoother m_center;
        Smoother m_distance;

        const float m_minRatio;
        const float m_maxRatio;
        const float m_border;

        bool m_changeViewport;

        void m_calcOrtho();
        void m_project();
        void m_uploadProj();
    };
}

#endif
