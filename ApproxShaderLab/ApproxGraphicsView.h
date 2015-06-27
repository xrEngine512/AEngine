#ifndef APPROXGRAPHICSVIEW_H
#define APPROXGRAPHICSVIEW_H

#include <QGraphicsView>


namespace ASL{
    enum Shader_Type;
}


namespace ApproxGUI
{
    class ApproxGraphicsView : public QGraphicsView
    {
        Q_OBJECT

    public:
        ApproxGraphicsView(QWidget *parent);
        ~ApproxGraphicsView();
    
    signals:
        void createShaderElem(ASL::Shader_Type);
		void sceneMoved();
		void viewChanged();
    public slots:

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
