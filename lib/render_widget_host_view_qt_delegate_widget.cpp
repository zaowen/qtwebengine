#include "render_widget_host_view_qt_delegate_widget.h"

#include "shared/backing_store_qt.h"
#include "shared/render_widget_host_view_qt.h"

#include <QResizeEvent>
#include <QPaintEvent>

RenderWidgetHostViewQtDelegateWidget::RenderWidgetHostViewQtDelegateWidget(content::RenderWidgetHostViewQt* view, QWidget *parent)
    : QWidget(parent)
    , m_painter(0)
    , m_backingStore(0)
    , m_view(view)
{
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

QRectF RenderWidgetHostViewQtDelegateWidget::screenRect() const
{
    return QRectF(x(), y(), width(), height());
}

void RenderWidgetHostViewQtDelegateWidget::show()
{
    QWidget::show();
}

void RenderWidgetHostViewQtDelegateWidget::hide()
{
    QWidget::hide();
}


bool RenderWidgetHostViewQtDelegateWidget::isVisible() const
{
    return QWidget::isVisible();
}

QWindow* RenderWidgetHostViewQtDelegateWidget::window() const
{
    return QWidget::windowHandle();
}

void RenderWidgetHostViewQtDelegateWidget::update(const QRect& rect)
{
    QWidget::update(rect);
}

void RenderWidgetHostViewQtDelegateWidget::setBackingStore(BackingStoreQt* backingStore)
{
    m_backingStore = backingStore;
    if (m_backingStore)
        m_backingStore->resize(size());
}

void RenderWidgetHostViewQtDelegateWidget::paintEvent(QPaintEvent * event)
{
    if (!m_backingStore)
        return;
    QPainter painter(this);
    m_backingStore->paintToTarget(&painter, event->rect());
}

QPainter* RenderWidgetHostViewQtDelegateWidget::painter()
{
    if (!m_painter)
        m_painter = new QPainter(this);
    return m_painter;
}

void RenderWidgetHostViewQtDelegateWidget::resizeEvent(QResizeEvent *resizeEvent)
{
    if (m_backingStore)
        m_backingStore->resize(resizeEvent->size());
    QWidget::update();
}

bool RenderWidgetHostViewQtDelegateWidget::event(QEvent *event)
{
    if (!m_view || !m_view->handleEvent(event))
        return QWidget::event(event);
    return true;
}
