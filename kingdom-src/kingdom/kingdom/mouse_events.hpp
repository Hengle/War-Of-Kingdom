/* $Id: mouse_events.hpp 47656 2010-11-21 13:59:10Z mordante $ */
/*
   Copyright (C) 2006 - 2010 by Joerg Hinrichs <joerg.hinrichs@alice-dsl.de>
   wesnoth playturn Copyright (C) 2003 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef MOUSE_EVENTS_H_INCLUDED
#define MOUSE_EVENTS_H_INCLUDED

#include "global.hpp"

#include "actions.hpp"
#include "game_display.hpp"
#include "pathfind/pathfind.hpp"
#include "random.hpp"
#include "unit_map.hpp"
#include "mouse_handler_base.hpp"

#include "SDL.h"

class tod_manager;
class artifical;
class hero_map;
class card;

namespace events{

class mouse_handler : public mouse_handler_base {
public:
	mouse_handler(play_controller& controller, game_display* gui, std::vector<team>& teams, unit_map& units, hero_map& heros, gamemap& map,
		tod_manager& tod_mng, game_state& gamestate, undo_list& undo_stack);
	~mouse_handler();
	static mouse_handler* get_singleton() { return singleton_ ;}
	void set_side(int side_number);

	int get_path_turns() const { return path_turns_; }

	const pathfind::paths& current_paths() const { return current_paths_; }

	const map_location& get_last_hex() const { return last_hex_; }
	map_location get_selected_hex() const { return selected_hex_; }
	bool get_undo() const { return undo_; }
	void set_path_turns(const int path_turns) { path_turns_ = path_turns; }
	void set_current_paths(pathfind::paths new_paths);
	void set_selected_hex(map_location hex) { selected_hex_ = hex; }
	void deselect_hex();
	void invalidate_reachmap() { reachmap_invalid_ = true; }

	void set_gui(game_display* gui) { gui_ = gui; }
	void set_undo(const bool undo) { undo_ = undo; }

	void set_expedite(artifical *c, unit& u);
	void set_building(artifical* bldg);
	void set_card_playing(team& t, int index);
	void set_hero_placing(hero* h);

	void set_unit_placing(unit& u);
	void set_unit_tasking(unit& u);

	unit* selected_unit();

	pathfind::marked_route get_route(unit* un, map_location go_to, team &team);

	const pathfind::marked_route& get_current_route() { return current_route_; }

	//get visible ajacent enemies of 1-based side around location loc
	std::set<map_location> get_adj_enemies(const map_location& loc, int side) const;

	void select_hex(const map_location& hex, const bool browse);

	void clear();

	//
	// moving
	//
	void begin_moving(const map_location& src, const map_location& dst);
	void end_moving();
	bool post_move_unit(unit& mover, const map_location& stop_loc);

	map_location move_unit_along_current_route(const std::vector<map_location>& caller_steps, bool check_shroud, bool attackmove = false);

	bool in_tip_state() const;
	bool in_multistep_state() const;

	void exit_building(bool ok);
	void exit_placing_unit(bool ok);
	void exit_selecting(bool ok);

	void do_right_click(const bool browse);

protected:
	/**
	 * Due to the way this class is constructed we can assume that the
	 * display* gui_ member actually points to a game_display (derived class)
	 */
	game_display& gui() { return *gui_; }
	/** Const version */
	const game_display& gui() const { return *gui_; }

	team& viewing_team() { return teams_[gui().viewing_team()]; }
	const team& viewing_team() const { return teams_[gui().viewing_team()]; }
	team &current_team() { return teams_[side_num_ - 1]; }

	int drag_threshold() const;

	/**
	 * Use update to force an update of the mouse state.
	 */
	void mouse_motion(int x, int y, const bool browse);
	void left_mouse_up(int x, int y, bool motions, const bool browse);
	bool right_click(int x, int y, const bool browse);
	void clear_undo_stack();
		
	// fill weapon choices into bc_vector
	// return the best weapon choice
	int fill_weapon_choices(std::vector<battle_context>& bc_vector, unit_map::iterator attacker, unit_map::iterator defender);
	// wrapper to catch bad_alloc so this should be called
	bool attack_enemy(unit& attacker, unit& defender, const map_location& target_loc);
	// the real function but can throw bad_alloc
	bool attack_enemy_(unit& attacker, unit& defender, const map_location& target_loc);

	bool cast_tactic(unit& tactician, const map_location& target_loc, bool browse);
	void cast_tactic_special(unit& tactician, unit* special, bool browse);

	void clear_formationed(unit& u, bool browse);
	void intervene_move(unit& u, const map_location& target_loc, bool browse);

	bool do_formation_attack(unit& attacker);

	void place_at(const map_location& at, hero& h);
	void join_in(const map_location& at, hero& h);

	// move a field troop from a to b.
	void direct_move_unit(unit& u, const map_location& from, const map_location& to);

	void do_task(unit& u, const map_location& at);

	bool interior(bool browse, unit& u);
	void build_ea(const map_location& loc);

	// the perform attack function called after a random seed is obtained
	void perform_attack(unit& attacker, unit& defender, int attacker_weapon, int defender_weapon, rand_rng::seed_t seed);

	map_location current_unit_attacks_from(const map_location& loc);
	map_location current_unit_tactic_from(const map_location& loc);
	map_location current_unit_formation_from(const map_location& loc);
	map_location current_unit_interior_from(const map_location& loc);
	map_location current_unit_alternate_from(const map_location& loc);
	map_location current_unit_select_from(const map_location& loc);
	map_location current_unit_build_from(const map_location& loc);
	const map_location& current_unit_placable_from(const map_location& loc) const ;
	const map_location& current_unit_joinable_from(const map_location& loc) const;
	map_location current_unit_taskable_from(const map_location& loc, int task) const ;
	map_location current_unit_clear_formationed_from(const map_location& loc);
	map_location current_unit_intervene_move_from(const map_location& loc);

	unit* find_unit(const map_location& hex) const;

private:
	play_controller& controller_;
	gamemap& map_;
	game_display* gui_;
	std::vector<team>& teams_;
	unit_map& units_;
	hero_map& heros_;
	tod_manager& tod_manager_;
	game_state& gamestate_;
	undo_list& undo_stack_;

	// previous highlighted hexes
	// the hex of the selected unit and empty hex are "free"
	map_location previous_hex_;
	map_location previous_free_hex_;
	map_location selected_hex_;
	map_location next_unit_;
	pathfind::marked_route current_route_;
	pathfind::paths current_paths_;
	bool enemy_paths_;
	int path_turns_;
	int side_num_;

	bool undo_;
	bool over_route_;
	bool attackmove_;
	bool reachmap_invalid_;
	bool show_partial_move_;

	//
	// recalling
	//
	artifical *expediting_city_;
	unit* expediting_unit_;

	//
	// building
	//
	artifical *building_bldg_;

	//
	// card playing
	//
	const card* playing_card_;
	int card_index_;

	//
	// placing hero
	//
	hero* placing_hero_;

	//
	// placing unit
	//
	unit* placing_unit_;

	//
	// tasking unit
	//
	unit* tasking_unit_;

	//
	// cast tactic
	//
	hero* selected_hero_;

	static mouse_handler * singleton_;
};

}

#endif
