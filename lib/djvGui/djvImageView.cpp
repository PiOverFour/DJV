//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
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

#include <djvImageView.h>

#include <djvGuiContext.h>

#include <djvColorUtil.h>
#include <djvDebugLog.h>
#include <djvError.h>
#include <djvErrorUtil.h>
#include <djvImage.h>
#include <djvOpenGlImage.h>
#include <djvPixelDataUtil.h>

//------------------------------------------------------------------------------
// djvImageView::Private
//------------------------------------------------------------------------------

struct djvImageView::Private
{
    Private(djvGuiContext * context) :
        context(context)
    {}
    
    const djvPixelData *  data     = nullptr;
    djvOpenGlImageOptions options;
    glm::ivec2            viewPos  = glm::ivec2(0, 0);
    float                 viewZoom = 1.f;
    bool                  viewFit  = false;
    djvOpenGlImageState   state;
    djvGuiContext *       context  = nullptr;
};

//------------------------------------------------------------------------------
// djvImageView
//------------------------------------------------------------------------------

djvImageView::djvImageView(
    djvGuiContext *   context,
    QWidget *         parent,
    Qt::WindowFlags   flags) :
    djvOpenGlWidget(parent, flags),
    _p(new Private(context))
{}

djvImageView::~djvImageView()
{}

const djvPixelData * djvImageView::data() const
{
    return _p->data;
}

const djvOpenGlImageOptions & djvImageView::options() const
{
    return _p->options;
}

const glm::ivec2 & djvImageView::viewPos() const
{
    return _p->viewPos;
}

float djvImageView::viewZoom() const
{
    return _p->viewZoom;
}

djvBox2f djvImageView::bbox() const
{
    return bbox(_p->viewPos, _p->viewZoom);
}

bool djvImageView::hasViewFit() const
{
    return _p->viewFit;
}

QSize djvImageView::sizeHint() const
{
    return QSize(200, 200);
}

void djvImageView::setData(const djvPixelData * data)
{
    if (data == _p->data)
        return;
    //DJV_DEBUG("djvImageView::setData");
    _p->data = data;
    update();
    Q_EMIT dataChanged(_p->data);
    Q_EMIT viewChanged();
}

void djvImageView::setOptions(const djvOpenGlImageOptions & options)
{
    if (options == _p->options)
        return;
    _p->options = options;
    update();
    Q_EMIT optionsChanged(_p->options);
    Q_EMIT viewChanged();
}

void djvImageView::setViewPos(const glm::ivec2 & pos)
{
    setViewPosZoom(pos, _p->viewZoom);
}

void djvImageView::setViewZoom(float zoom)
{
    //DJV_DEBUG("djvImageView::setViewZoom");
    //DJV_DEBUG_PRINT("zoom = " << zoom);
    setViewPosZoom(_p->viewPos, zoom);
}

void djvImageView::setViewZoom(float zoom, const glm::ivec2 & focus)
{
    //DJV_DEBUG("djvImageView::setViewZoom");
    //DJV_DEBUG_PRINT("zoom = " << zoom);
    //DJV_DEBUG_PRINT("focus = " << focus);
    //DJV_DEBUG_PRINT("viewZoom = " << _p->viewZoom);
    setViewPosZoom(
        focus + glm::ivec2(glm::vec2(_p->viewPos - focus) * (zoom / _p->viewZoom)),
        zoom);
}

void djvImageView::setViewPosZoom(const glm::ivec2 & pos, float zoom)
{
    const bool posSame  = pos == _p->viewPos;
    const bool zoomSame = djvMath::fuzzyCompare(zoom, _p->viewZoom);
    if (posSame && zoomSame)
        return;
    //DJV_DEBUG("djvImageView::setViewPosZoom");
    //DJV_DEBUG_PRINT("pos = " << pos);
    //DJV_DEBUG_PRINT("zoom = " << zoom);
    _p->viewPos  = pos;
    _p->viewZoom = zoom;
    _p->viewFit  = false;
    update();
    if (! posSame)
        Q_EMIT viewPosChanged(_p->viewPos);
    if (! zoomSame)
        Q_EMIT viewZoomChanged(_p->viewZoom);
    Q_EMIT viewChanged();
}

void djvImageView::viewZero()
{
    setViewPosZoom(glm::ivec2(), 1.f);
}

void djvImageView::viewCenter()
{
    setViewPos(glm::vec2(width(), height()) / 2.f - bbox().size / 2.f);
}

void djvImageView::viewFit()
{
    if (! _p->data)
        return;
    //DJV_DEBUG("djvImageView::viewFit");
    const djvBox2i geom(width(), height());
    //DJV_DEBUG_PRINT("geom = " << geom.size);
    const djvBox2f bbox = this->bbox(glm::ivec2(), 1.f);
    //DJV_DEBUG_PRINT("bbox = " << bbox);
    const float zoom = djvVectorUtil::isSizeValid(bbox.size) ?
        djvMath::min(geom.w / bbox.size.x, geom.h / bbox.size.y) : 1.f;
    //DJV_DEBUG_PRINT("zoom = " << zoom);
    glm::ivec2 pos =
        glm::vec2(geom.size) / 2.f -
        bbox.size * zoom / 2.f -
        this->bbox(glm::ivec2(), zoom).position;
    //DJV_DEBUG_PRINT("pos = " << pos);
    setViewPosZoom(pos, zoom);
    //DJV_DEBUG_PRINT("fit");
    _p->viewFit = true;
}

void djvImageView::paintGL()
{
    djvOpenGlWidget::paintGL();

    //DJV_DEBUG("djvImageView::draw");
    //DJV_DEBUG_PRINT("valid = " << valid());
    //DJV_DEBUG_PRINT("view pos = " << _viewPos);
    //DJV_DEBUG_PRINT("view zoom = " << _viewZoom);

    const int devicePixelRatio = this->devicePixelRatio();
    //DJV_DEBUG_PRINT("devicePixelRatio = " << devicePixelRatio);
    const djvBox2i geom(
        width () * devicePixelRatio,
        height() * devicePixelRatio);
    //DJV_DEBUG_PRINT("geom = " << geom.size);

    //if (! valid())
    //{
        djvOpenGlUtil::ortho(glm::ivec2(geom.w, geom.h));
        glViewport(0, 0, geom.w, geom.h);
    //}
    djvColor background(djvPixel::RGB_F32);
    djvColorUtil::convert(_p->options.background, background);
    glClearColor(
        background.f32(0),
        background.f32(1),
        background.f32(2),
        0.f);
    //glClearColor(0, 1, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    if (! _p->data)
        return;

    //DJV_DEBUG_PRINT("data = " << *_p->data);
    djvOpenGlImageOptions options = _p->options;
    options.xform.position += _p->viewPos;
    options.xform.scale *= _p->viewZoom * devicePixelRatio;
    try
    {
        djvOpenGlImage::draw(*_p->data, options, &_p->state);
    }
    catch (const djvError & error)
    {
        DJV_LOG(_p->context->debugLog(), "djvImageView",
            djvErrorUtil::format(error).join("\n"));
    }
}

djvBox2f djvImageView::bbox(const glm::ivec2 & pos, float zoom) const
{
    if (! _p->data)
        return djvBox2f();

    //DJV_DEBUG("djvImageView::bbox");
    //DJV_DEBUG_PRINT("pos = " << pos);
    //DJV_DEBUG_PRINT("zoom = " << zoom);

    const djvPixelDataInfo & info = _p->data->info();
    //DJV_DEBUG_PRINT("info = " << info);

    djvOpenGlImageXform xform = _p->options.xform;
    xform.position += pos;
    xform.scale    *= zoom;
    //DJV_DEBUG_PRINT("xform = " << xform);

    const glm::mat3x3 m = djvOpenGlImageXform::xformMatrix(xform);
    //DJV_DEBUG_PRINT("m = " << m);

    djvBox2f box(_p->data->info().size * djvPixelDataUtil::proxyScale(info.proxy));
    //DJV_DEBUG_PRINT("box = " << box);

    box = m * box;
    //DJV_DEBUG_PRINT("box = " << box);
    return box;
}

