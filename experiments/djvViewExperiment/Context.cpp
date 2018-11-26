//------------------------------------------------------------------------------
// Copyright (c) 2004-2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <Context.h>

#include <FileSystem.h>
#include <ISystem.h>
#include <Project.h>
#include <ProxyStyle.h>
#include <ToolSystem.h>
#include <UndoStack.h>
#include <Workspace.h>

#include <QApplication>

namespace djv
{
    namespace ViewExperiment
    {
        struct Context::Private
        {
            std::vector<QPointer<ISystem> > systems;
            QPointer<UndoStack> undoStack;
            QPointer<ProxyStyle> style;
        };

        Context::Context(int & argc, char ** argv) :
            _p(new Private)
        {
            _p->systems.push_back(new FileSystem(this));
            _p->systems.push_back(new ToolSystem(this));
            _p->systems.push_back(new WorkspaceSystem(this));

            _p->undoStack = new UndoStack(this);

            _p->style = new ProxyStyle(this);
            qApp->setStyle(_p->style);
        }

        Context::~Context()
        {}

        const std::vector<QPointer<ISystem> > & Context::getSystems() const
        {
            return _p->systems;
        }

        void Context::addSystem(const QPointer<ISystem> & system)
        {
            _p->systems.push_back(system);
        }

        const QPointer<UndoStack> & Context::getUndoStack() const
        {
            return _p->undoStack;
        }

        QPointer<QStyle> Context::getStyle() const
        {
            return _p->style.data();
        }

    } // namespace ViewExperiment
} // namespace djv

