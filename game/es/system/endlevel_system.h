#ifndef ENDLEVEL_SYSTEM_H
#define ENDLEVEL_SYSTEM_H

#include "../system_enums.h"

typedef struct EndLevelSystem {
	SystemId system_id;
} EndLevelSystem;

#include "../engine.h"

#define EXIT_ANIMATION_DURATION_MS 1500

void system_endlevel_init(EndLevelSystem*, Engine*);
EndLevelSystem* system_endlevel_alloc(Engine*);
void system_endlevel_update(EndLevelSystem*, Engine*);
void system_endlevel_free(EndLevelSystem*);
void next_bot(Engine*, EndLevelSystem*);


#endif //ENDLEVEL_SYSTEM_H
