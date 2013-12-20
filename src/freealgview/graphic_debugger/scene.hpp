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

/*! \file Scene.hpp
 *  \brief Fichero que define a la clase \c fpt::Scene.
 *
 *  \author Aarón Bueno Villares, <abv150ci@gmail.com>
 *  \date 04/2011
 *  \version 0.7
 *  \since 0.7
 */


#ifndef FREEALGVIEW_SCENE_HPP_INCLUDED
#define FREEALGVIEW_SCENE_HPP_INCLUDED

#include <vector>

#include <GL/gl.h>
#include <FTGL/ftgl.h>

#include "layouter.hpp"
#include "mouse.hpp"

namespace freealgview
{
    class Scene : public Strategy
    {
    public:
        Scene(Layouter& layouter, Mouse& mouse);
        ~Scene();

        bool changed() const;
        bool describedNodes() const;

        GLuint textureNode() const;
        GLuint textureGlow() const;
        GLuint textureEdge() const;

        void setTextureNode(GLuint id);
        void setTextureGlow(GLuint id);
        void setTextureEdge(GLuint id);

        void describeAllNodes(bool describe = true);

    protected:
        void m_init();
        void m_step();

    private:
        // fnode_set m_seenNodes;

        Layouter& m_layouter;
        Mouse& m_mouse;

        FTFont *m_font;
        GLuint m_textureIDnode;
        GLuint m_textureIDglow;
        GLuint m_textureIDedge;
        bool m_initTexNode;
        bool m_initTexGlow;

        bool m_initTexEdge;
        float m_radiusNode;

        float m_weightEdge;
        bool m_describeNodes;

        void m_drawTree();
        void m_drawNode(fspace::fnode node);
        void m_drawGlow(fspace::fnode node);
        void m_drawEdge(fspace::fnode source, fspace::fnode target);

        void m_drawSquare(Pos const& p, float side, GLuint tex);

        void m_drawText(fspace::fnode node);
        // void m_drawHelp();

        void m_setColor(fspace::fnode node);
        void m_setColor(float r, float g, float b);
    };
}

#endif
