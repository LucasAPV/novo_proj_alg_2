#ifndef __ACTIONS__
#define __ACTIONS__

#include "basic_imports.h"
#include "beasts.h"
#include "context.h"
#include "adventure.h"
#include "filter.h"
#include "item.h"
#include "party.h"

void actions_for_items(CharacterDB *c_db, ItemDB *i_db);
void actions_for_characters(CharacterDB *c_db);
void actions_for_monsters(BeastDB *b_db);
void actions_for_candidates(ContextEncounter c, BeastsCandidates *b, BeastDB b_db);
void actions_for_party(Party *party, CharacterDB c_db);
void actions_for_context(ContextEncounter *context, Party party);
#endif