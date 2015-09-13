#ifndef MOUSETRACKER_H
#define MOUSETRACKER_H

#include <QObject>
#include <QTimer>
#include <QPoint>

class MouseTracker : public QObject
{
	Q_OBJECT;
	QPoint mousePos;
	QTimer mouseTrack;
	MouseTracker(QObject *parent);
public:
	static MouseTracker* Instance(QObject *parent = nullptr);
	~MouseTracker();
signals:
	void mouseMove(const QPoint& globalPos);
private:
	
};

#endif // MOUSETRACKER_H
