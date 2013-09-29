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

/*! \file Scene.cpp
 *  \brief Fichero que implementa a la clase \c fpt::Scene.
 *
 *  \author Aarón Bueno Villares, <abv150ci@gmail.com>
 *  \date 04/2011
 *  \version 0.7
 *  \since 0.7
 */

#include <vector>

#include "scene.hpp"

// using namespace std;

namespace freealgview {

    Scene::Scene(Layouter& layouter, Mouse& mouse)
        : m_layouter(layouter), m_mouse(mouse)
    {
        m_initTexNode = false;
        m_initTexGlow = false;
        m_initTexEdge = false;

        m_radiusNode = 5;
        m_weightEdge = 1.5;

        m_font = new FTGLTextureFont
            ("freealgview/graphic_debugger/resources/FreeSans.ttf");

        m_font->FaceSize(12);
    }

    Scene::~Scene()
    {
        delete m_font;
    }

    bool Scene::changed() const
    {
        return m_layouter.changed();
    }

    bool Scene::describedNodes() const
    {
        return m_describeNodes;
    }

    GLuint Scene::textureNode() const
    {
        return m_textureIDnode;
    }

    GLuint Scene::textureGlow() const
    {
        return m_textureIDglow;
    }

    GLuint Scene::textureEdge() const
    {
        return m_textureIDedge;
    }

    void Scene::setTextureNode(GLuint id)
    {
        m_textureIDnode = id;
        m_initTexNode = true;
    }

    void Scene::setTextureGlow(GLuint id)
    {
        m_textureIDglow = id;
        m_initTexGlow = true;
    }

    void Scene::setTextureEdge(GLuint id)
    {
        m_textureIDedge = id;
        m_initTexEdge = true;
    }

    void Scene::describeAllNodes(bool describe)
    {
        m_describeNodes = describe;
    }

    void Scene::m_init()
    {
        m_describeNodes = false;

        glEnable(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (auto i : m_layouter.tree()) {
            auto factor = i.edegree() + i.idegree() + 1;

            Props& l_props = i;

            l_props.randGlow(200 * factor);
            l_props.glow().target(50 * factor);
            l_props.glow().smooth(0.005);

            l_props.glow().init();
        }
    }

    void Scene::m_step()
    {
        bool init = m_initTexNode && m_initTexGlow &&
            m_initTexEdge;

        if (init) {
            glClear(GL_COLOR_BUFFER_BIT);

            for (auto i : m_layouter.tree())
                i.get<Props>().glow().step();

            m_drawTree();
        }
    }

    void Scene::m_drawTree()
    {
        std::vector<fspace::fnode> showedNames;

        for (auto i : m_layouter.tree())
            m_drawGlow(i);

        for (auto i : m_layouter.tree()) {
            for (auto j : i.children())
                m_drawEdge(i, j);

            auto l_ref = *i;

            if (l_ref) {
                m_setColor(1, 0, 0);
                m_drawEdge(i, l_ref);
            }
        }

        for (auto i : m_layouter.tree()) {
            m_drawNode(i);

            if (!describedNodes() && i.get<Props>().showedName())
                showedNames.push_back(i);
        }

        if (describedNodes())
            for (auto i : m_layouter.tree())
                m_drawText(i);
        else {
            fspace::fnode node = m_mouse.actualNode();

            if (node and !node.get<Props>().showedName())
                showedNames.push_back(node);

            for (auto i : showedNames)
                m_drawText(i);
        }
    }

    void Scene::m_drawNode(fspace::fnode node)
    {
        m_setColor(node);

        if (node.idegree() == 0) // for root.
            m_drawSquare(node, 1.5 * m_radiusNode, m_textureIDnode);
        else
            m_drawSquare(node, m_radiusNode, m_textureIDnode);
    }

    void Scene::m_drawGlow(fspace::fnode node)
    {
        m_setColor(node);

        m_drawSquare(node, node.get<Props>().glow().x(), m_textureIDglow);
    }

    void Scene::m_drawEdge(fspace::fnode source, fspace::fnode target)
    {
        glBindTexture(GL_TEXTURE_2D, m_textureIDedge);

        VecXf l_source = source.get<Pos>();
        VecXf l_target = target.get<Pos>();

        VecXf dir = l_target - l_source;
        VecXf weight = VecXf(-dir.y(), dir.x()).unit() * m_weightEdge;

        VecXf point;

        glBegin(GL_QUADS); {
            point = l_source + weight;

            if (*source == target)
                m_setColor(1, 0, 0);
            else
                m_setColor(source);

            glTexCoord2f(1, 0);
            glVertex2f(point.x(), point.y());

            point = l_source - weight;
            glTexCoord2f(0, 0);
            glVertex2f(point.x(), point.y());

            m_setColor(target);

            point = l_target - weight;
            glTexCoord2f(0, 0);
            glVertex2f(point.x(), point.y());

            point = l_target + weight;
            glTexCoord2f(1, 0);
            glVertex2f(point.x(), point.y());
        } glEnd();
    }

    void Scene::m_drawSquare(Pos const& p, float side, GLuint tex)
    {
        glBindTexture(GL_TEXTURE_2D, tex);

        float x = p.x();
        float y = p.y();

        glBegin(GL_QUADS); {
            glTexCoord2f(1, 1);
            glVertex2f(x + side, y + side);

            glTexCoord2f(1, 0);
            glVertex2f(x + side, y - side);

            glTexCoord2f(0, 0);
            glVertex2f(x - side, y - side);

            glTexCoord2f(0, 1);
            glVertex2f(x - side, y + side);

        } glEnd();
    }

    void Scene::m_setColor(fspace::fnode node)
    {
        if (node.idegree() == 0)
            m_setColor(0.7, 1, 0.7);
        else if (node.leaf() == 0)
            m_setColor(0.5, 0.8, 1);
        else
            m_setColor(1, 1, 1);
    }

    void Scene::m_setColor(float r, float g, float b)
    {
        glColor3f(r, g, b);
    }

    void Scene::m_drawText(fspace::fnode node)
    {
        if (node) {
            FTBBox box = m_font->BBox(node.id().c_str());
            float heightBox = box.Upper().Y() - box.Lower().Y();

            VecXf pos = static_cast<VecXf>(node.get<Pos>()) +
                VecXf(7.0f, - heightBox / 2);

            glColor3f(1, 1, 0);

            // m_setColor(m_nodeMouse);

            glTranslatef(pos.x(), pos.y(), 0);

            m_font->Render(node.id().c_str());

            glTranslatef(-pos.x(), -pos.y(), 0);
        }
    }

}
