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
#include <QObject>

#include <hydrogen/hydrogen.h>
#include <hydrogen/event_queue.h>
#include <hydrogen/basics/pattern.h>
#include <hydrogen/basics/pattern_list.h>
#include <hydrogen/LocalFileMng.h>

#include "HydrogenApp.h"
#include "SongEditor/SongEditor.h"
#include "SongEditor/SongEditorPanel.h"

#include "PatternWatcher.h"

using namespace H2Core;

/**
* @class PatternWatcher
*
* @brief 
*
* Blah

* @author wvengen
*
*/

const char* PatternWatcher::__class_name = "PatternWatcher";

PatternWatcher::PatternWatcher( QString filename, Hydrogen* hydrogen ) : QObject(), Object( __class_name ) {
	this->filename = filename;
	this->hydrogen = hydrogen;

	if ( ! filename.isEmpty() ) {
		watcher.addPath(filename);
		connect(&watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(handleFileChanged(const QString&)));
	}
}

PatternWatcher::~PatternWatcher() {
}

void PatternWatcher::handleFileChanged(const QString& filename) {
	//qDebug() << "Pattern file changed: " << filename;

	/// @todo allow to configure what pattern is replaced: selected or fixed index
	LocalFileMng fileMng;
	Pattern* newPattern = fileMng.loadPattern( filename );
	if ( newPattern == 0 ) {
		_ERRORLOG( QString("Error loading watched pattern: ") + filename );
	} else {
		/// @todo only update (set modified) if pattern is different
		this->hydrogen->getSong()->get_pattern_list()->replace( 0, newPattern );
		this->hydrogen->getSong()->__is_modified = true;
		EventQueue::get_instance()->push_event( EVENT_PATTERN_MODIFIED, 0 );
	}
}

