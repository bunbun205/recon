#ifndef DEBUGAPPLICATION_H
#define DEBUGAPPLICATION_H

#include <QApplication>
#include <iostream>

#include "OpenGLException.h"

class DebugApplication : public QApplication {
public:
	DebugApplication( int & argc, char ** argv) :  QApplication(argc, argv), mAboutToTerminate(false)
	{}

	virtual bool notify(QObject *recv, QEvent *e) override {
		try {
			return QApplication::notify( recv, e );
		}
		catch (OpenGLException &ex) {
			ex.writeMsgStackToStream(std::cerr);
			mAboutToTerminate = true;
			QApplication::exit(1);
		}
		return false;
	}

	bool mAboutToTerminate;
};

#endif // DEBUGAPPLICATION_H
