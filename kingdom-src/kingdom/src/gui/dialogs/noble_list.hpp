/* $Id: hero_list.hpp 49605 2011-05-22 17:56:24Z mordante $ */
/*
   Copyright (C) 2008 - 2011 by Jörg Hinrichs <joerg.hinrichs@alice-dsl.de>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GUI_DIALOGS_NOBLE_LIST_HPP_INCLUDED
#define GUI_DIALOGS_NOBLE_LIST_HPP_INCLUDED

#include "gui/dialogs/dialog.hpp"

#include "config.hpp"
#include <set>

class team;
class unit;
class unit_map;
class hero;
class hero_map;
class tnoble;

namespace gui2 {

class tlistbox;

class tnoble_list : public tdialog
{
public:
	explicit tnoble_list(std::vector<team>& teams, unit_map& units, hero_map& heros, std::vector<std::pair<int, unit*> >& human_pairs, int side);

protected:
	/** Inherited from tdialog. */
	void pre_show(CVideo& video, twindow& window);

	/** Inherited from tdialog. */
	void post_show(twindow& window);

private:
	/** Inherited from tdialog, implemented by REGISTER_DIALOG. */
	virtual const std::string& window_id() const;

	void treasure_selected(twindow& window);

	void fill_2list(twindow& window);

	int noble_list_index_2_noble_index() const;
private:

	std::vector<team>& teams_;
	unit_map& units_;
	hero_map& heros_;
	team& current_team_;

	std::vector<std::pair<int, unit*> >& human_pairs_;

	std::map<int, std::pair<int, unit*> > holden_;
	int leader_noble_level_;

	tlistbox* treasure_list_;

	int noble_list_index_;
};

}

#endif

