#ifndef APPROXGRAPHICSVIEW_H
#define APPROXGRAPHICSVIEW_H

#include <serialization/ShaderCommon.h>
#include <QGraphicsView>

namespace ASL{

    class GraphicsView final : public QGraphicsView
    {
        Q_OBJECT

    public:
        explicit GraphicsView(QWidget *parent);
        ~GraphicsView() override;
    
    signals:
        void createShaderElem(ShaderType);
		void sceneMoved();
		void viewChanged();

    private slots:
        void onContextMenu(QAction*);
    private:
        void mouseMoveEvent(QMouseEvent *) override;
        void mousePressEvent(QMouseEvent *) override;
        void mouseReleaseEvent(QMouseEvent *) override;
        void contextMenuEvent(QContextMenuEvent*) override;
		void resizeEvent(QResizeEvent*) override;

        int m_xPrev, m_yPrev;
        bool m_dragging;        
    };
}
#endif // APPROXGRAPHICSVIEW_H
