#ifndef APPROXGRAPHICSVIEW_H
#define APPROXGRAPHICSVIEW_H

#include <QGraphicsView>
#include "Enums.h"

namespace ASL{

    class GraphicsView : public QGraphicsView
    {
        Q_OBJECT

    public:
        GraphicsView(QWidget *parent);
        ~GraphicsView();
    
    signals:
        void createShaderElem(Shader_Type);
		void sceneMoved();
		void viewChanged();

    private slots:
        void onContextMenu(QAction*);
    private:
        void mouseMoveEvent(QMouseEvent *)override;
        void mousePressEvent(QMouseEvent *)override;
        void mouseReleaseEvent(QMouseEvent *)override;        
        void contextMenuEvent(QContextMenuEvent*)override;
		void resizeEvent(QResizeEvent*)override;
        int m_xPrev, m_yPrev;
        bool m_dragging;        
    };
}
#endif // APPROXGRAPHICSVIEW_H
