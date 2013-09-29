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

/*! \file GraphicEngine.hpp
 *  \brief Archivo que define a la clase \c fpt::GraphicEngine.
 *
 *  \author Aarón Bueno Villares, <abv150ci@gmail.com>
 *  \date 04/2011
 *  \version 0.7
 *  \since 0.7
 */

#ifndef FREEALGVIEW_GRAPHIC_DEBUGGER_HPP_INCLUDED
#define FREEALGVIEW_GRAPHIC_DEBUGGER_HPP_INCLUDED

#include "../faudata.hpp"

#include "helpers/strategy.hpp"
#include "layouter.hpp"

#include "viewer.hpp"
#include "scene.hpp"
#include "mouse.hpp"

#include <QApplication>

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>

#include <string>

// #include "Search.hpp"
// #include "HelpDialog.hpp"

namespace freealgview
{
    class GraphicDebugger : public QApplication,
                            public QGLWidget, public Strategy
    {
    public:
        GraphicDebugger();
        ~GraphicDebugger();

    public:
        void operator()();
        fspace::fnode owner() const;

    public:
        void owner(fspace::fnode owner);

    protected:
        void m_init();
        void m_step();

    private:
        fspace::fnode m_owner;

        Layouter m_layouter;
        // ParserTree *_parser;

        Viewer m_viewer;
        Scene m_scene;
        Mouse m_mouse;

        // Search *_search;
        // HelpDialog *_helpDialog;

        bool m_controlKey;
        bool m_shiftKey;
        float m_ratioKey;

        void initializeGL();
        void paintGL();
        void resizeGL();

        void timerEvent(QTimerEvent* event);
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void resizeGL(int width, int height);

        void m_loadTextures();
    };
}

#endif
