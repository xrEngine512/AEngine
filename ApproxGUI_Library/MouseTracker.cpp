#include "MouseTracker.h"
#include <memory>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

MouseTracker::MouseTracker(QObject *parent)
	: QObject(parent)
{
	mouseTrack.setInterval(16);
	connect(&mouseTrack, &QTimer::timeout, [&]()
	{
#ifdef WIN32		
		//if (GetAsyncKeyState(VK_LBUTTON))
		{
			POINT point;
			GetCursorPos(&point);
			if (mousePos.x() != point.x || mousePos.y() != point.y){
				mousePos.setX(point.x);
				mousePos.setY(point.y);
				emit mouseMove(mousePos);
			}
		}
#endif
	});
	mouseTrack.start();
}

MouseTracker* MouseTracker::Instance(QObject* parent)
{
	static std::shared_ptr<MouseTracker> instancePtr(nullptr);
	if (!instancePtr.get())
		instancePtr = std::shared_ptr<MouseTracker>(new MouseTracker(parent));
	return instancePtr.get();
}

MouseTracker::~MouseTracker()
{
	qDebug("MouseTracker Destroyed");
}
