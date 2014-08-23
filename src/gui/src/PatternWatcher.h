/*
 * Hydrogen
 * Copyright(c) 2014 by wvengen [dev-music@willem.engen.nl]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef PATTERN_WATCHER_H
#define PATTERN_WATCHER_H

#include <QObject>
#include <QFileSystemWatcher>

#include <hydrogen/hydrogen.h>
#include <hydrogen/object.h>
#include <hydrogen/basics/pattern_list.h>
#include <cassert>

namespace H2Core
{

///
/// Pattern watcher, which watches a file and reloads the current pattern when it's changed
///
class PatternWatcher : public QObject, public H2Core::Object
{
	Q_OBJECT
	H2_OBJECT
public:
	PatternWatcher( QString filename, Hydrogen* hydrogen );
	~PatternWatcher();


private slots:
	void handleFileChanged(const QString& filename);

private:
	QString filename;  /// < Filename to watch, or empty if none
	Hydrogen* hydrogen; /// < Hydrogen instance

	QFileSystemWatcher watcher;
};

};

#endif

