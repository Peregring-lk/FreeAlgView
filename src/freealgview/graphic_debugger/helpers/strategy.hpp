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

/*! \file Strategy.hpp
 *  \brief Fichero que define a la clase \c fpt::Strategy.
 *
 *  \author Aarón Bueno Villares, <abv150ci@gmail.com>
 *  \date 04/2011
 *  \version 0.7
 *  \since 0.7
 */

#ifndef FREEALGVIEW_STRATEGY_HPP_INCLUDED
#define FREEALGVIEW_STRATEGY_HPP_INCLUDED

namespace freealgview
{
    class Strategy
    {
    public:
        virtual ~Strategy() = default;

        bool initialized() const
        { return m_initialized; }

        virtual bool changed() const
        { return false; }

        void init() { m_init(); }
        void step() { m_step(); }

    protected:
        virtual void m_init() {}
        virtual void m_step() {}

    private:
       	bool m_initialized = false;
    };
}

#endif
