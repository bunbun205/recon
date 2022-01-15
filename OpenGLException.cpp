#include "OpenGLException.h"

#include <QStringList>

OpenGLException::OpenGLException(const QString & msg) {
	mMsgStack.push_back( std::make_pair(QString(msg), QString()));
}

OpenGLException::OpenGLException(const QString & msg, const QString & where) {
	mMsgStack.push_back( std::make_pair(QString(msg), where));
}

OpenGLException::OpenGLException(OpenGLException & previous, const QString & msg) :
	mMsgStack(previous.mMsgStack)
{
	mMsgStack.push_back( std::make_pair(QString(msg), QString()));
}

OpenGLException::OpenGLException(OpenGLException & previous, const QString & msg, const QString & where)  :
	mMsgStack(previous.mMsgStack)
{
	mMsgStack.push_back( std::make_pair(QString(msg), where));
}

void OpenGLException::writeMsgStackToStream(std::ostream & strm) const {
	for (std::list<std::pair<QString, QString> >::const_iterator it = mMsgStack.begin();
		it != mMsgStack.end(); ++it)
	{
		QStringList lines = it->first.split("\n");
		QString indx("[%1] ");
		indx = indx.arg(std::distance(mMsgStack.begin(),it));
		for (const QString & l : lines) {
			if (it->second.isEmpty())
				strm << (indx + l + "\n").toStdString();
			else
				strm << (indx + it->second + " : " + l + "\n").toStdString();
		}
		// strm.flush();
	}
}
