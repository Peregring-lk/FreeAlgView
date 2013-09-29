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

/*! \file GraphicDebugger.cpp
 *  \brief Archivo que implementa a la clase \c fpt::GraphicDebugger.
 *
 *  \author Aarón Bueno Villares, <abv150ci@gmail.com>
 *  \date 04/2011
 *  \version 0.7
 *  \since 0.7
 */

#include <iostream>
#include <QApplication>

#include "../freealgview.hpp"
#include "graphic_debugger.hpp"

namespace freealgview {

    GraphicDebugger::GraphicDebugger()
        : QApplication(fspace::fcloud<fspace::argc_tp>(),
                       fspace::fcloud<fspace::argv_tp>()),
          // 3 40 200 2
          m_layouter(3, 40, 200, 2),
          m_viewer(m_layouter),
          m_mouse(m_layouter, m_viewer),
          m_scene(m_layouter, m_mouse)
    {
        owner(fspace::fcloud(FreeAlgView::class_id()));
    }

    GraphicDebugger::~GraphicDebugger()
    {
        deleteTexture(m_scene.textureNode());
        deleteTexture(m_scene.textureGlow());
        deleteTexture(m_scene.textureEdge());
        //    deleteTexture(_helpDialog->texture());
    }

    void GraphicDebugger::operator()()
    {
        // TODO: I suppose that the owner exists always.
        static bool first = true;

        if (first) {
            QApplication::startTimer(40);

            // connect(&m_qTimer, SIGNAL(timeout()), this, SLOT(&operator()()));
            // m_qTimer.start(40);

            show();

            first = false;

            auto l_exit = exec();

            return;
        }
        else
            step();
    }

    fspace::fnode GraphicDebugger::owner() const
    {
        return m_owner;
    }

    void GraphicDebugger::owner(fspace::fnode owner)
    {
        m_owner = owner;
        m_layouter.owner(m_owner);
    }

    void GraphicDebugger::m_init()
    {
        setFocus(Qt::MouseFocusReason);
        setMouseTracking(true);
        resize(600, 480);

        m_ratioKey = 20;
        m_controlKey = false;
        m_shiftKey = false;

        // _search = new Search("Search", this);
        // _helpDialog = new HelpDialog(_viewing, 0.6f);

        m_loadTextures();

        m_viewer.sizeViewport(width(), height());

        m_layouter.init();

        m_owner.get<Props>().showedName(true);

        m_scene.init();
        m_viewer.init();
        m_mouse.init();

        // _search->init();
        // _helpDialog->init();
    }

    void GraphicDebugger::m_step()
    {
        // if (_search->newSearch()) {
        //     PhyloNode *node = _tree->phyloNode(_search->actualSearch());

        //     if (node == NULL) {
        //         node = _parser->expand(_search->actualSearch());

        //         if (node != NULL) {
        //             _tree->reboot(node->name(), node);
        //             _tree->init();
        //             // _search->check(true);
        //         }
        //         else
        //             // _search->check(false);
        //     }

        //     if (node != NULL)
        //         node->showName(true);

        //     // _search->reboot();
        // }

        repaint();
    }

    void GraphicDebugger::initializeGL()
    {
        Strategy::init();
    }

    void GraphicDebugger::paintGL()
    {
        m_layouter.step();

        if (m_mouse.leftClick()) {
            VecXf move = m_mouse.mov();
            move.y(-move.y());

            m_viewer.moveCamera(move);
        }

        m_viewer.step();
        m_mouse.step();

        m_scene.step();

        // _search->step();
        // _helpDialog->step();
    }

    void GraphicDebugger::resizeGL()
    {
        m_viewer.sizeViewport(width(), height());
    }

    void GraphicDebugger::timerEvent(QTimerEvent* event)
    {
        (*this)();
    }

    void GraphicDebugger::keyPressEvent(QKeyEvent *event)
    {
        if (m_controlKey) {
            if (event->key() == Qt::Key_F) ; /*
                                               if (_search->isVisible()) {
                                               _search->setFocus();
                                               _search->selectAll();
                                               }
                                               else
                                               _search->reactivate(); */
            else if (event->key() == Qt::Key_S)
                grabFrameBuffer().save("capture.png");
            else if (event->key() == Qt::Key_A)
                m_scene.describeAllNodes(!m_scene.describedNodes());
        }
        // else if (event->key() == Qt::Key_F4)
        //     if (_search->isVisible())
        //         _search->hide();
        //     else
        //         _search->reactivate();
        // else if (event->key() == Qt::Key_F1)
        //     _helpDialog->setShow(!_helpDialog->isVisible());
        else if (event->key() == Qt::Key_Escape)
            QApplication::quit();
        else if (event->key() == Qt::Key_Space)
            m_viewer.centering();
        else if (event->key() == Qt::Key_Up)
            m_viewer.moveCamera(VecXf(0, m_ratioKey));
        else if (event->key() == Qt::Key_Down)
            m_viewer.moveCamera(VecXf(0, -m_ratioKey));
        else if (event->key() == Qt::Key_Right)
            m_viewer.moveCamera(VecXf(m_ratioKey, 0));
        else if (event->key() == Qt::Key_Left)
            m_viewer.moveCamera(VecXf(-m_ratioKey, 0));
        else if (event->key() == Qt::Key_Control)
            m_controlKey = true;
        else if (event->key() == Qt::Key_Shift)
            m_shiftKey = true;
    }

    void GraphicDebugger::keyReleaseEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Control)
            m_controlKey = false;
        else if (event->key() == Qt::Key_Shift)
            m_shiftKey = false;
    }

    void GraphicDebugger::mousePressEvent(QMouseEvent *event)
    {
        // _search->clearFocus();

        auto l_node = m_mouse.actualNode();

        if (l_node && m_controlKey) {
            Props& l_props = l_node;

            l_props.showedName(!l_props.showedName());

            if (!l_props.showedName())
                m_mouse.clean();
        }
    }

    void GraphicDebugger::mouseDoubleClickEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            auto l_node = m_mouse.actualNode();

            if (l_node) {
                Props& l_props = l_node;

                if (m_controlKey) {
                    // _tree->crib(node);
                    // _tree->prepareColor(NULL);

                    l_props.showedName(true);
                }
                else if (m_shiftKey) {
                    // node->clear();
                    // _tree->clearNode(node);
                    // node->showName(false);
                }
                else {
                    // _parser->expand(node);
                    // _tree->prepareLoc(node);
                    // _tree->prepareColor(node);
                    // node->showName(true);
                }
            }
        }
    }

    void GraphicDebugger::mouseMoveEvent(QMouseEvent *event)
    {
        QPointF pos = event->posF();

        m_mouse.setPos(VecXf(pos.x(), pos.y()));
        m_mouse.setLeftClick(event->buttons() == Qt::LeftButton);
    }

    void GraphicDebugger::resizeGL(int width, int height)
    {
        m_viewer.sizeViewport(width, height);
    }

    void GraphicDebugger::m_loadTextures()
    {
        QImage textureNode("freealgview/graphic_debugger/resources/file.png");
        QImage textureGlow("freealgview/graphic_debugger/resources/bloom.png");
        QImage textureEdge("freealgview/graphic_debugger/resources/beam.png");
        // QImage textureHelp("Resources/help.png");

        m_scene.setTextureNode(bindTexture(textureNode, GL_TEXTURE_2D,
                                           QGLContext::MipmapBindOption));
        m_scene.setTextureGlow(bindTexture(textureGlow, GL_TEXTURE_2D,
                                           QGLContext::MipmapBindOption));
        m_scene.setTextureEdge(bindTexture(textureEdge, GL_TEXTURE_2D,
                                           QGLContext::MipmapBindOption));
        // _helpDialog->setTexture(bindTexture(textureHelp, GL_TEXTURE_2D,
        // QGLContext::MipmapBindOption));
    }

}
