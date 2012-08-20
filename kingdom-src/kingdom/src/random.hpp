/* $Id: random.hpp 41185 2010-02-13 13:40:11Z ilor $ */
/*
   Copyright (C) 2003 by David White <dave@whitevine.net>
   Copyright (C) 2005 - 2010 by Yann Dirson <ydirson@altern.org>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2
   or at your option any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** @file random.hpp */

#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

#define POOL_ALLOC_DATA_SIZE	4194304	// 4M
#define POOL_ALLOC_POS_SIZE		1048576 // 1M, One allocate may save 1024x1024 packet pos.
#define POOL_COMMAND_RESERVE_BYTES	100	// Assume, one command reserve 100 bytes.
class command_pool
{
public:
	enum TYPE {START = 1, INIT_SIDE, END_TURN, INIT_AI, RECRUIT, EXPEDITE, DISBAND, MOVE_HEROS, BELONG_TO, 
		BUILD, COUNTDOWN_UPDATE, MOVEMENT, ATTACK, RANDOM_NUMBER, CHOOSE, LABLE, RENAME, SPEAK, 
		EVENT, UNIT_CHECKSUM, CHECKSUM_CHECK, ADVANCEMENT_UNIT, CLEAR_LABELS, DIPLOMATISM, 
		FINAL_BATTLE, INPUT, HERO_FIELD, ADD_CARD, ERASE_CARD, ARMORY, INTERIOR};
	struct command {
		TYPE type;
		unsigned int flags;
	};

	command_pool();
	explicit command_pool(const command_pool& that);
	~command_pool();
	command_pool& operator=(const command_pool& that);

	void read(unsigned char* mem);
	void write(unsigned char* mem);

	void clear();

	unsigned char* pool_data() const { return pool_data_; }
	int pool_data_size() const { return pool_data_size_; }
	int pool_data_vsize() const { return pool_data_vsize_; }
	unsigned int* pool_pos() const { return pool_pos_; }
	int pool_pos_size() const { return pool_pos_size_; }
	int pool_pos_vsize() const { return pool_pos_vsize_; }

	command* add_command();

protected:
	unsigned char* pool_data_;
	unsigned int* pool_pos_;
	int pool_data_size_;
	int pool_data_vsize_;
	int pool_pos_size_;
	int pool_pos_vsize_;
};

class config;

int get_random();
int get_random_nocheck();
const config* get_random_results();
void set_random_results(const config& cfg);

namespace rand_rng
{

typedef unsigned int seed_t;

class rng;

struct set_random_generator {
	set_random_generator(rng* r);
	~set_random_generator();
private:
	rng* old_;
};

} // ends rand_rng namespace

#endif