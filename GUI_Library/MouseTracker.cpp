#include "MouseTracker.h"
#include <memory>

#include <IO.h>

MouseTracker::MouseTracker(QObject *parent)
	: QObject(parent)
{
	mouseTrack.setInterval(16);
	connect(&mouseTrack, &QTimer::timeout, [&]()
	{
		auto point = IO::Mouse::get_pointer_position();
		if (mousePos.x() != point.x || mousePos.y() != point.y){
			mousePos.setX(point.x);
			mousePos.setY(point.y);
			emit mouseMove(mousePos);
		}
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
